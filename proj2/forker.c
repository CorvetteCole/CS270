#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <wait.h>

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

void spawnD(char myName){
    pid_t forkValueD = Fork();
    if (forkValueD == 0) {
        myName = 'D';
        greet(myName);
        goAway(myName);
    } else {
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
            branch(myName, controlValue, originalPID);
            goAway(myName);
        } else {
            if (getpid() == originalPID) {
                spawnD(myName);
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
