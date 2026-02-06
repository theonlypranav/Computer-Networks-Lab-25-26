#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAXPENDING 5
#define BUFFERSIZE 32

void handleClient(int clientSocket, struct sockaddr_in clientAddress) {
    char msg[BUFFERSIZE];
    printf("Handling Client %s\n", inet_ntoa(clientAddress.sin_addr));

    int temp2 = recv(clientSocket, msg, BUFFERSIZE, 0);
    if (temp2 < 0) {
        perror("Error receiving from client");
        close(clientSocket);
        exit(1);
    }
    msg[temp2] = '\0'; // Null-terminate received string
    printf("Client says: %s\n", msg);

    printf("ENTER MESSAGE FOR CLIENT: ");
    fgets(msg, BUFFERSIZE, stdin);
    msg[strcspn(msg, "\n")] = '\0'; // Remove newline

    int bytesSent = send(clientSocket, msg, strlen(msg), 0);
    if (bytesSent != strlen(msg)) {
        perror("Error while sending message to client");
        close(clientSocket);
        exit(1);
    }

    close(clientSocket);
    exit(0); // End child process
}

int main() {
    /*CREATE A TCP SOCKET*/
    int serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket < 0) {
        perror("Error while server socket creation");
        exit(1);
    }
    printf("Server Socket Created\n");

    /*CONSTRUCT LOCAL ADDRESS STRUCTURE*/
    struct sockaddr_in serverAddress, clientAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12396);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Error while binding");
        exit(1);
    }
    printf("Binding successful\n");

    if (listen(serverSocket, MAXPENDING) < 0) {
        perror("Error in listen");
        exit(1);
    }
    printf("Now Listening\n");

    while (1) {
        int clientLength = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientLength);
        if (clientSocket < 0) {
            perror("Error in client socket");
            continue;
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            close(clientSocket);
        } else if (pid == 0) {
            // Child process handles client
            close(serverSocket); // Child doesn't need server socket
            handleClient(clientSocket, clientAddress);
        } else {
            // Parent process continues to accept new clients
            close(clientSocket);
        }
    }

    close(serverSocket);
    return 0;
}
