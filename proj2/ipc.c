#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <wait.h>

typedef void (*sighandler_t)(int);

volatile sig_atomic_t sigC;
volatile sig_atomic_t sigD;

volatile sig_atomic_t pidC = -1;
volatile sig_atomic_t pidD = -1;

extern void dieWithError();

void greet(char myName) {
    printf("Process %c, PID %d greets you.\n", myName, getpid());
    fflush(stdout);
}

void goAway(char myName) {
    printf("Process %c exits normally\n", myName);
    fflush(stdout);
    exit(1);
}

void printExitStatus(char myName, char targetName, int status) {
    printf("Process %c here: Process %c exited with status %#04x\n", myName, targetName, status);
}

pid_t Fork(void) {
    pid_t retval;
    if ((retval = fork()) < 0)
        dieWithError();
    return retval;
}

void siguser1_handler(int signum) {
    sigC = 1;
}

void siguser2_handler(int signum) {
    sigD = 1;
}

// special handlers that forward signals to a pid set by flag, otherwise to the existing handlers
void siguser1_handler_fwd(int signum) {
    if (pidD == -1) {
        siguser1_handler(signum);
    } else {
        kill(pidD, SIGUSR1);
    }
}

void siguser2_handler_fwd(int signum) {
    if (pidC == -1) {
        siguser2_handler(signum);
    } else {
        kill(pidC, SIGUSR2);
    }
}

sighandler_t setSignalHandler(int signum, sighandler_t sighandler) {
    struct sigaction action, old_action;
    action.sa_handler = sighandler;
    sigemptyset(&action.sa_mask); /* Block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* Restart syscallsif possible */
    if (sigaction(signum, &action, &old_action) < 0) {
        dieWithError("Signal error");
    }
    return (old_action.sa_handler);
}

void setupSignals() {
    setSignalHandler(SIGUSR1, siguser1_handler);
    setSignalHandler(SIGUSR2, siguser2_handler);
}

void waitForSignal(int checkC){
    /* condition is 0 initially */
    /* Signal handler will set condition to be 1 */
    sigset_t unblock, blockAll, oldMask;
    if (sigfillset(&blockAll) < 0) { dieWithError(); }   /* all signal bits set */
    if (sigemptyset(&unblock) < 0) { dieWithError(); } /* no signal bits set */
    if (sigprocmask(SIG_BLOCK, &blockAll, &oldMask) < 0) {/* block everything */
        dieWithError();
    }
    if (checkC) {
        while (sigC == 0) {
            sigsuspend(&unblock);
        }
        sigC = 0;
    } else {
        while (sigD == 0) {
            sigsuspend(&unblock);
        }
        sigD = 0;
    }
    /* condition is nonzero – restore original signal mask */
    if (sigprocmask(SIG_SETMASK, &oldMask, NULL) < 0) { dieWithError(); }
}

void spawnD(char myName, int forkValueB) {
    pid_t forkValueD = Fork();
    if (forkValueD == 0) {
        setupSignals();
        myName = 'D';

        //
        // wait for signal that C has greeted
        waitForSignal(1);
        greet(myName);

        // send signal that D has greeted
        kill(getppid(), SIGUSR2);

        // wait for signal that C has exited
        waitForSignal(1);
        goAway(myName);
    } else {
        // this is the last part that runs on the original process until waiting,
        // so we set up our signal forwarding here.

        // calculate PID value of C
        int forkValueC = forkValueB + 1 == forkValueD ? forkValueB + 2 : forkValueB + 1;
        pidC = forkValueC;
        // setup signal forwarding handlers
        setSignalHandler(SIGUSR1, siguser1_handler_fwd);
        setSignalHandler(SIGUSR2, siguser2_handler_fwd);
        // wait for initial signal from C and then handshake to confirm readiness
        waitForSignal(1);
        pidD = forkValueD;
        kill(forkValueC, SIGUSR2);

        // handle exit (normal stuff)
        int childStatusD;
        if (waitpid(forkValueD, &childStatusD, 0) != -1) {
            printExitStatus(myName, 'D', childStatusD);
        } else {
            dieWithError();
        }
    }
}

void branch(char myName, int controlValue, pid_t originalPID) {
    if (controlValue > 0) {
        pid_t forkValue = Fork();
        if (forkValue == 0) {
            myName++;
            controlValue--;
            greet(myName);
            if (controlValue == 0) { // do some special things if this is the last branch set by controlValue
                setupSignals();
                // continually send signal to parent until the handshake is complete, indicating readiness
                while (sigD == 0) {
                    kill(originalPID, SIGUSR1);
                    sleep(1);
                }
                sigD = 0;

                // send signal, C has greeted
                kill(originalPID, SIGUSR1);

                // wait for signal that D has greeted
                waitForSignal(0);

                // send signal, C has exited
                kill(originalPID, SIGUSR1);
            } else {
                // continue branching until our controlValue is completely depleted
                branch(myName, controlValue, originalPID);
            }
            goAway(myName);
        } else {
            if (getpid() == originalPID) {
                // spawn the special D branch after the B branch. Not an elegant solution but I didn't have time
                // to make a one size fits all solution
                spawnD(myName, forkValue);
            }
            int childStatus;
            if (waitpid(forkValue, &childStatus, 0) != -1) {
                printExitStatus(myName, myName + 1, childStatus);
                goAway(myName);
            } else {
                dieWithError();
            }
        }

    }
}

int main() {
    char myName = 'A';
    int controlValue = 2;
    pid_t originalPID = getpid();

    greet(myName);
    branch(myName, controlValue, originalPID);
    goAway(myName);
    return 0;
}
