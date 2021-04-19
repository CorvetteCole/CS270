#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netdb.h>
#include "UDPCookie.h"

typedef void (*sighandler_t)(int);

const int TIMEOUT = 3;
const int MAGIC_NUM = 270;
const int HEADER_SIZE = 12;

int hasAlarm = 0;

void sigalarm_handler(int signum) {
    /* condition is 0 initially */
    /* Signal handler will set condition to be 1 */
    sigset_t unblock, blockAll, oldMask;
    if (sigfillset(&blockAll) < 0) { dieWithError("All signal bits set"); }   /* all signal bits set */
    if (sigemptyset(&unblock) < 0) { dieWithError("No signal bits set"); } /* no signal bits set */
    if (sigprocmask(SIG_BLOCK, &blockAll, &oldMask) < 0) {/* block everything */
        dieWithError("failed during block everything");
    }
    hasAlarm = 1;
    /* condition is nonzero – restore original signal mask */
    if (sigprocmask(SIG_SETMASK, &oldMask, NULL) < 0) { dieWithError("failed to restore mask"); }
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

void printPacket(char msgBuffer[MAXMSGLEN]){
    header_t *msgptr = (header_t *) msgBuffer;
    printf("magic=%d\n", ntohs(msgptr->magic));
    printf("length=%d\n", ntohs(msgptr->length));
    printf("xid=%x\n", msgptr->xactionid);
    printf("version=%d\n", ((msgptr->flags >> 4) & 0b1111));
    printf("flags=%#x\n", msgptr->flags & 0b1111);
    printf("result=%d\n", msgptr->result);
    printf("port=%d\n", msgptr->port);
    printf("variable part=");
    for (int i = HEADER_SIZE; i < ntohs(msgptr->length); i++) {
        printf("%c", msgBuffer[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    char msgBuf[MAXMSGLEN];
    int msgLen = 0; // quiet the compiler

    setSignalHandler(SIGALRM, sigalarm_handler); // set the alarm signal handler

    if (argc != 3) // Test for correct number of arguments
        dieWithError("Parameter(sin_port): <Server Address/Name> <Server Port/Service>");

    char *server = argv[1];     // First arg: server address/name
    char *servPort = argv[2];

    // Tell the system what kind(sin_port) of address info we want
    struct addrinfo addrCriteria;                   // Criteria for address match
    memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
    addrCriteria.ai_family = AF_INET;               // IPv4 only
    // For the following fields, a zero value means "don't care"
    addrCriteria.ai_socktype = SOCK_DGRAM;          // Only datagram sockets
    addrCriteria.ai_protocol = IPPROTO_UDP;         // Only UDP protocol

    // Get address(es)
    struct addrinfo *servAddr;      // List of server addresses
    int rtnVal = getaddrinfo(server, servPort, &addrCriteria, &servAddr);
    if (rtnVal != 0) {
        char error[MAXMSGLEN];
        snprintf(error, MAXMSGLEN, "getaddrinfo() failed: %sin_port", gai_strerror(rtnVal));
        dieWithError(error);
    }
    /* Create a datagram socket */
    int sock = socket(servAddr->ai_family, servAddr->ai_socktype,
                      servAddr->ai_protocol); // Socket descriptor for client
    if (sock < 0)
        dieWithSystemError("socket() failed");


    char *messageText = "cjge227";
    msgLen = HEADER_SIZE + strlen(messageText);

    header_t *msgptr = (header_t *) msgBuf;
    msgptr->magic = htons(MAGIC_NUM);
    msgptr->length = htons(msgLen);
    msgptr->xactionid = 0x13377331;
    msgptr->flags = 0b00100010;
//    msgptr->flags = 0b00101010; // test flag set
    msgptr->result = 0;
    msgptr->port = 0;
    strncpy(msgBuf + HEADER_SIZE, messageText, strlen(messageText));


    do {
        if (hasAlarm) {
            printf("response not received after %d seconds, retrying...\n", TIMEOUT);
            hasAlarm = 0;
        }

        ssize_t numBytes = sendto(sock, msgBuf, msgLen, msgptr->flags, servAddr->ai_addr,
                                  servAddr->ai_addrlen);
        if (numBytes < 0)
            dieWithSystemError("sendto() failed");
        else if (numBytes != msgLen)
            dieWithError("sendto() returned unexpected number of bytes");

        alarm(TIMEOUT);
        siginterrupt(SIGALRM, 1);
        ssize_t numBytesRcvd = recvfrom(sock, msgBuf, MAXMSGLEN, 0, servAddr->ai_addr, &servAddr->ai_addrlen);
    } while (hasAlarm != 0);

    // after we know we have a packet, print that.
    printPacket(msgBuf);

    freeaddrinfo(servAddr);

    close(sock);
    exit(0);
}
