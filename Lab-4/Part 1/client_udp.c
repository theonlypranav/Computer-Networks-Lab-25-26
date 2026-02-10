/* =========================================================
   UDP ECHO CLIENT
   - Sends a message to server
   - Receives the same message back (echo)
   ========================================================= */

#include <stdio.h>      // printf(), fgets()
#include <string.h>     // memset(), strlen()
#include <stdlib.h>     // exit()
#include <arpa/inet.h>  // inet_addr(), htons()
#include <sys/socket.h> // socket(), sendto(), recvfrom()
#include <unistd.h>     // close()

#define BUFLEN 512      // Maximum size of message buffer
#define PORT 8888       // Server port number

/* Utility function for error handling */
void die(char *s) {
    perror(s);
    exit(1);
}

int main() {
    int sockfd;                         // Socket file descriptor
    struct sockaddr_in server_addr;     // Server address structure
    socklen_t slen = sizeof(server_addr);

    char message[BUFLEN];               // Buffer to send data
    char buffer[BUFLEN];                // Buffer to receive data

    /* Create UDP socket
       AF_INET     -> IPv4
       SOCK_DGRAM  -> UDP socket
       IPPROTO_UDP -> UDP protocol */
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == -1)
        die("socket");

    /* Initialize server address structure */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;               // IPv4
    server_addr.sin_port = htons(PORT);             // Convert port to network byte order
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost

    /* Infinite loop for continuous communication */
    while (1) {
        printf("Enter message: ");
        fgets(message, BUFLEN, stdin);   // Read user input safely

        /* Send message to server
           sendto() is used in UDP because there is no connection */
        if (sendto(sockfd, message, strlen(message), 0,
                   (struct sockaddr*)&server_addr, slen) == -1)
            die("sendto");

        /* Clear receive buffer */
        memset(buffer, 0, BUFLEN);

        /* Receive echoed message from server
           recvfrom() blocks until data arrives */
        if (recvfrom(sockfd, buffer, BUFLEN, 0,
                     (struct sockaddr*)&server_addr, &slen) == -1)
            die("recvfrom");

        printf("Server reply: %s\n", buffer);
    }

    close(sockfd); // Close socket
    return 0;
}

