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

    int bytes_received;

    // STEP 1: Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // STEP 2: Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Connect to localhost
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // STEP 3: Connect to server
    if (connect(sock,
                (struct sockaddr *)&server_addr,
                sizeof(server_addr)) == -1) {

        perror("Connection failed");
        close(sock);
        exit(1);
    }

    printf("Connected to server.\n");

    while (1) {

        printf("Enter message (type 'exit' to quit): ");

        fgets(buffer, BUFFER_SIZE, stdin);

        // exit condition
        if (strncmp(buffer, "exit", 4) == 0)
            break;

        // STEP 4: Send message
        write(sock, buffer, strlen(buffer));

        // STEP 5: Receive echo
        bytes_received = read(sock, buffer, BUFFER_SIZE);

        if (bytes_received <= 0) {
            printf("Server disconnected.\n");
            break;
        }

        buffer[bytes_received] = '\0';

        printf("Server reply: %s\n", buffer);
    }

    // STEP 6: Close socket
    close(sock);

    return 0;
}
