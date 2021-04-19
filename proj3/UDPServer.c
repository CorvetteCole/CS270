#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "UDPCookie.h"

#define ERRLEN 128

const int MAGIC_NUM = 270;
const int HEADER_SIZE = 12;

void printPacket(char msgBuffer[MAXMSGLEN]) {
    header_t *msgptr = (header_t *) msgBuffer;
    printf("===================\n");
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
    printf("===================\n");
}

void generateCookie(const char msgBuffer[MAXMSGLEN], char *cookie, int length){
    uint32_t hash, i;
    for(hash = i = HEADER_SIZE; i < length; ++i)
    {
        hash += msgBuffer[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    sprintf(cookie, "%x", hash);
}

// return size
int formResponse(char msgBuffer[MAXMSGLEN]) {
    header_t *msgptr = (header_t *) msgBuffer;
    int flags = 0b00100000;
    int result = 15;
    int msglen = ntohs(msgptr->length);
    flags |= (0b1000 & msgptr->flags); // extract bit 3

    char *errorText = malloc(31);
    if (ntohs(msgptr->length < 16)) { // header is malformed
        flags |= 0b1;
        char headerError[31];
        msglen += sprintf(headerError, "Length too small, length: %d ", ntohs(msgptr->length));
        strcat(errorText, headerError);
    }
    if (ntohs(msgptr->magic)  != MAGIC_NUM) { // intentionally buggy
        flags |= 0b1;
        if (msglen > HEADER_SIZE) {
            realloc(errorText, msglen - HEADER_SIZE + 30);
        }
        char magicError[30];
        msglen += sprintf(magicError, "Incorrect magic number: %d ", ntohs(msgptr->magic));
        strcat(errorText, magicError);
    }
    if (((msgptr->flags >> 4) & 0b1111) != 2) {
        flags |= 0b1;
        if (msglen > HEADER_SIZE) {
            realloc(errorText, msglen - HEADER_SIZE + 29);
        }
        char protocolError[29];
        msglen += sprintf(protocolError, "Unknown protocol version: %d ", ((msgptr->flags >> 4) & 0b1111));
        strcat(errorText, protocolError);
    }
    if (ntohs(msgptr->result) != 0) {
        flags |= 0b1;
        if (msglen > HEADER_SIZE) {
            realloc(errorText, msglen - HEADER_SIZE + 22);
        }
        char resultError[22];
        msglen += sprintf(resultError, "Result is not 0: %d ", ntohs(msgptr->result));
        strcat(errorText, resultError);
    }
    if (msgptr->flags & 0b0100) {
        flags |= 0b1;
        if (msglen > HEADER_SIZE) {
            realloc(errorText, msglen - HEADER_SIZE + 16);
        }
        char typeError[16];
        msglen += sprintf(typeError, "Wrong type: %d ", ntohs(msgptr->result));
        strcat(errorText, typeError);
    }

    // check if user id is valid
    for (int i = HEADER_SIZE; i < ntohs(msgptr->length); i++) {
        if ((msgBuffer[i] < 48 || msgBuffer[i] > 57) && (msgBuffer[i] < 65 || msgBuffer[i] > 90) &&
            (msgBuffer[i] < 97 || msgBuffer[i] > 122)) { // check if user id is outside alphanumeric range
            flags |= 0b1;
            char *iDError = malloc(
                    19 + HEADER_SIZE - ntohs(msgptr->length)); // allocate so we can fit whole user id in char array
            msglen += sprintf(iDError, "Invalid user ID: %.*s ", HEADER_SIZE - ntohs(msgptr->length), msgBuffer + HEADER_SIZE);
            strcat(errorText, iDError);
            break;
        }
    }

    if ((flags & 0b1)) {
        // append error message
        strncpy(msgBuffer + HEADER_SIZE, errorText, msglen - HEADER_SIZE);
        //msglen = HEADER_SIZE + strlen(errorText);
        result = 0;
    } else if (!(flags & 0b1000)) {
        // generate and append cookie if test flag is not set
        char cookie[32];
        generateCookie(msgBuffer, cookie, ntohs(msgptr->length));
        msglen = HEADER_SIZE + strlen(cookie);
        strncpy(msgBuffer + HEADER_SIZE, cookie, strlen(cookie));
    }


    // set up msg buffer for response
    msgptr->magic = htons(MAGIC_NUM);
    msgptr->length = htons(msglen);
    msgptr->flags = flags;
    msgptr->result = result;

    return msglen;
}

int main(int argc, char *argv[]) {

    if (argc != 2) // Test for correct number of arguments
        dieWithError("Parameter(s): <Server Port #>");

    char *service = argv[1]; // First arg:  local port/service

    // Construct the server address structure
    struct addrinfo addrCriteria;                   // Criteria for address
    memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
    addrCriteria.ai_family = AF_INET;               // We want IPv4 only
    addrCriteria.ai_flags = AI_PASSIVE;             // Accept on any address/port
    addrCriteria.ai_socktype = SOCK_DGRAM;          // Only datagram socket
    addrCriteria.ai_protocol = IPPROTO_UDP;         // UDP socket

    struct addrinfo *servAddr; // List of server addresses
    int rtnVal = getaddrinfo(NULL, service, &addrCriteria, &servAddr);
    if (rtnVal != 0) {
        char error[ERRLEN];
        if (snprintf(error, ERRLEN, "getaddrinfo() failed: %s",
                     gai_strerror(rtnVal)) < 0) // recursive?!
            dieWithSystemError("snprintf() failed");
        dieWithError(error);
    }

    // Create socket for incoming connections
    int sock = socket(servAddr->ai_family, servAddr->ai_socktype,
                      servAddr->ai_protocol);
    if (sock < 0)
        dieWithSystemError("socket() failed");

    // Bind to the local address/port
    if (bind(sock, servAddr->ai_addr, servAddr->ai_addrlen) < 0)
        dieWithSystemError("bind() failed");

    // Free address list allocated by getaddrinfo()
    freeaddrinfo(servAddr);

    for (;;) { // Run forever
        struct sockaddr_storage clntAddr; // Client address
        // Set Length of client address structure (in-out parameter)
        socklen_t clntAddrLen = sizeof(clntAddr);

        // Block until receive message from a client
        char buffer[MAXMSGLEN]; // I/O buffer
        // Size of received message
        ssize_t numBytesRcvd = recvfrom(sock, buffer, MAXMSGLEN, 0,
                                        (struct sockaddr *) &clntAddr, &clntAddrLen);
        printf("packet of size %zd received\n", numBytesRcvd);
        fflush(stdout);
        if (numBytesRcvd < 0)
            dieWithSystemError("recvfrom() failed");

        /* YOUR CODE HERE:  parse & display incoming request message */
        printPacket(buffer);

        /* YOUR CODE HERE:  construct Response message in buffer, display it */
        int responseLength = formResponse(buffer);

        printf("sending response packet of size %d\n", responseLength);
        printPacket(buffer);
        ssize_t numBytesSent = sendto(sock, buffer, responseLength, 0,
                                      (struct sockaddr *) &clntAddr, sizeof(clntAddr));
        if (numBytesSent < 0)
            dieWithSystemError("sendto() failed)");
    }
    // NOT REACHED
}
