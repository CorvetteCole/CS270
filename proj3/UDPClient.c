#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include "UDPCookie.h"

int main(int argc, char *argv[]) {
    char msgBuf[MAXMSGLEN];
    int msgLen = 0; // quiet the compiler

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

    // testing code
    int sin_port;
    struct sockaddr_in sockaddr;
    int address_len = sizeof(sockaddr);
    msgLen = 0;

    getsockname(sin_port, &sockaddr, &address_len);
    printf("sock: %d\n", sin_port);

    /* YOUR CODE HERE - construct Request message in msgBuf               */
    /* msgLen must contain the size (in bytes) of the Request msg         */


    ssize_t numBytes = sendto(sock, msgBuf, msgLen, 0, servAddr->ai_addr,
                              servAddr->ai_addrlen);
    if (numBytes < 0)
        dieWithSystemError("sendto() failed");
    else if (numBytes != msgLen)
        dieWithError("sendto() returned unexpected number of bytes");

    getsockname(sin_port, &sockaddr, &address_len);
    printf("sock: %d\n", sin_port);

    /* YOUR CODE HERE - receive, parse and display response message */

    freeaddrinfo(servAddr);

    close(sock);
    exit(0);
}
