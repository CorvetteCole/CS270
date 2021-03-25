#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

typedef void (*sighandler_t)(int);

//void dieWithError() {
//    perror("Error: ");
//};

extern void dieWithError();

#define LIMIT 20
#define PERIOD 5

/* global variables for communication between handlers and main */
int sigalarm_count = 0;
int sigint_count = 0;

/* YOUR CODE HERE - Signal handler for SIGALRM */
void sigalarm_handler(int signum) {
/* condition is 0 initially */
    /* Signal handler will set condition to be 1 */
    sigset_t unblock, blockAll, oldMask;
    if (sigfillset(&blockAll) < 0) { dieWithError(); }   /* all signal bits set */
    if (sigemptyset(&unblock) < 0) { dieWithError(); } /* no signal bits set */
    if (sigprocmask(SIG_BLOCK, &blockAll, &oldMask) < 0) {/* block everything */
        dieWithError();
    }
    sigalarm_count++;
    alarm(PERIOD);
    /* condition is nonzero – restore original signal mask */
    if (sigprocmask(SIG_SETMASK, &oldMask, NULL) < 0) { dieWithError(); }
}

/* YOUR CODE HERE - Signal handler for SIGINT */
void sigint_handler(int signum) {
    /* condition is 0 initially */
    /* Signal handler will set condition to be 1 */
    sigset_t unblock, blockAll, oldMask;
    if (sigfillset(&blockAll) < 0) { dieWithError(); }   /* all signal bits set */
    if (sigemptyset(&unblock) < 0) { dieWithError(); } /* no signal bits set */
    if (sigprocmask(SIG_BLOCK, &blockAll, &oldMask) < 0) {/* block everything */
        dieWithError();
    }
    sigint_count++;
    /* condition is nonzero – restore original signal mask */
    if (sigprocmask(SIG_SETMASK, &oldMask, NULL) < 0) { dieWithError(); }
}

pid_t Fork(void) {
    pid_t retval;
    if ((retval = fork()) < 0)
        exit(1);
    return retval;
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

int main() {
    /* YOUR CODE HERE - the textbook and slides have examples
     * of how to install signal handlers, block signals, etc.
     */
    pid_t forkValue = Fork();
    if (forkValue == 0) {
        pid_t parentID = getppid();
        getchar();
        for (int i = 0; i < LIMIT; i++) {
            kill(parentID, SIGINT);
            usleep(1000); // signals aren't received unless there is a small sleep
        }
        printf("Child: finished sending SIGINT LIMIT times.\n");
        fflush(stdout);
        exit(0);
    } else {
        setSignalHandler(SIGALRM, sigalarm_handler);
        setSignalHandler(SIGINT, sigint_handler);
        alarm(PERIOD);

        /* condition is 0 initially */
        /* Signal handler will set condition to be 1 */
        sigset_t unblock, blockAll, oldMask;
        if (sigfillset(&blockAll) < 0) { dieWithError(); }   /* all signal bits set */
        if (sigemptyset(&unblock) < 0) { dieWithError(); } /* no signal bits set */
        if (sigprocmask(SIG_BLOCK, &blockAll, &oldMask) < 0) {/* block everything */
            dieWithError();
        }
        while (sigalarm_count <= LIMIT && sigint_count <= LIMIT) {
            //printf(". sigalarm_count %d && sigint_count %d\n", sigalarm_count, sigint_count);
            printf(".");
            fflush(stdout);
            sigsuspend(&unblock);
            if (sigprocmask(SIG_BLOCK, &blockAll, &oldMask) < 0) {/* block everything */
                dieWithError();
            }
        }
        /* condition is nonzero – restore original signal mask */
        if (sigprocmask(SIG_SETMASK, &oldMask, NULL) < 0) { dieWithError(); }
        printf("sigalarm_count: %d, sigint_count: %d\n", sigalarm_count, sigint_count);
    }
    exit(0);
}
