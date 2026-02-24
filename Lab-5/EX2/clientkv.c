#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8898
#define BUFFER_SIZE 1024

int main() {

    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock,
            (struct sockaddr *)&server_addr,
            sizeof(server_addr));

    printf("Connected to Key-Value Server\n");

    while (1) {

        printf("Enter command: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        write(sock, buffer, strlen(buffer));

        if (strncmp(buffer, "Bye", 3) == 0)
            break;

        int bytes = read(sock, buffer, BUFFER_SIZE - 1);
        buffer[bytes] = '\0';

        printf("Server: %s", buffer);
    }

    close(sock);
    return 0;
}
