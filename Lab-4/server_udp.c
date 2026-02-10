/* =========================================================
   UDP ECHO SERVER
   - Receives message from client
   - Sends the same message back
   ========================================================= */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 512
#define PORT 8888

void die(char *s) {
    perror(s);
    exit(1);
}

int main() {
    int sockfd;                             // Server socket
    struct sockaddr_in server_addr;         // Server address
    struct sockaddr_in client_addr;         // Client address
    socklen_t slen = sizeof(client_addr);
    char buffer[BUFLEN];

    /* Create UDP socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == -1)
        die("socket");

    /* Configure server address */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Accept from any client

    /* Bind socket to port
       Required for server to receive data */
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
        die("bind");

    /* Server runs forever */
    while (1) {
        printf("Waiting for data...\n");

        /* Receive data from client */
        memset(buffer, 0, BUFLEN);
        if (recvfrom(sockfd, buffer, BUFLEN, 0,
                     (struct sockaddr*)&client_addr, &slen) == -1)
            die("recvfrom");

        printf("Received from %s:%d -> %s\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port),
               buffer);

        /* Echo the same data back to client */
        if (sendto(sockfd, buffer, strlen(buffer), 0,
                   (struct sockaddr*)&client_addr, slen) == -1)
            die("sendto");
    }

    close(sockfd);
    return 0;
}
