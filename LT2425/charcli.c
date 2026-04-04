#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUF 1024

int main() {
    int sock;
    struct sockaddr_in serverAddr;
    char buf[BUF], name[50];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    printf("Enter username: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    send(sock, name, strlen(name), 0);

    fd_set readfds;

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(0, &readfds);      // stdin
        FD_SET(sock, &readfds);   // socket

        select(sock + 1, &readfds, NULL, NULL, NULL);

        // User input
        if (FD_ISSET(0, &readfds)) {
            fgets(buf, BUF, stdin);
            buf[strcspn(buf, "\n")] = '\0';

            send(sock, buf, strlen(buf), 0);

            if (strcmp(buf, "quit") == 0)
                break;
        }

        // Server message
        if (FD_ISSET(sock, &readfds)) {
            int n = recv(sock, buf, BUF, 0);
            if (n <= 0) break;

            buf[n] = '\0';
            printf("%s\n", buf);
        }
    }

    close(sock);
    return 0;
}