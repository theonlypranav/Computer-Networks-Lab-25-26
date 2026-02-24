#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8890
#define BUFFER_SIZE 1024

int main() {

    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int bytes_received;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(sock);
        exit(1);
    }

    printf("Connected to server.\n");

    while (1) {
        printf("Enter message (type Bye to exit): ");
        fgets(buffer, BUFFER_SIZE, stdin);

        write(sock, buffer, strlen(buffer));

        if (strncmp(buffer, "Bye", 3) == 0)
            break;

        bytes_received = read(sock, buffer, BUFFER_SIZE);
        buffer[bytes_received] = '\0';

        printf("Server reply: %s\n", buffer);
    }

    close(sock);
    return 0;
}
