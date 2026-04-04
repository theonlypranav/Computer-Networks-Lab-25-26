#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUF 1024

int main() {
    int serverSocket, client1, client2;
    struct sockaddr_in serverAddr;
    char buf[BUF], name1[50], name2[50];

    FILE *log = fopen("log.txt", "w");

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 2);

    printf("Waiting for 2 clients...\n");

    client1 = accept(serverSocket, NULL, NULL);
    recv(client1, name1, sizeof(name1), 0);

    client2 = accept(serverSocket, NULL, NULL);
    recv(client2, name2, sizeof(name2), 0);

    printf("Clients connected: %s and %s\n", name1, name2);

    fd_set readfds;

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(client1, &readfds);
        FD_SET(client2, &readfds);

        select((client1 > client2 ? client1 : client2) + 1, &readfds, NULL, NULL, NULL);

        // Client 1 → Client 2
        if (FD_ISSET(client1, &readfds)) {
            int n = recv(client1, buf, BUF, 0);
            buf[n] = '\0';

            if (strcmp(buf, "quit") == 0) {
                printf("%s sent quit\n", name1);
                break;
            }

            char msg[BUF];
            sprintf(msg, "%s: %s", name1, buf);

            send(client2, msg, strlen(msg), 0);
            fprintf(log, "%s: %s\n", name1, buf);
        }

        // Client 2 → Client 1
        if (FD_ISSET(client2, &readfds)) {
            int n = recv(client2, buf, BUF, 0);
            buf[n] = '\0';

            if (strcmp(buf, "quit") == 0) {
                printf("%s sent quit\n", name2);
                break;
            }

            char msg[BUF];
            sprintf(msg, "%s: %s", name2, buf);

            send(client1, msg, strlen(msg), 0);
            fprintf(log, "%s: %s\n", name2, buf);
        }
    }

    close(client1);
    close(client2);
    close(serverSocket);
    fclose(log);

    return 0;
}