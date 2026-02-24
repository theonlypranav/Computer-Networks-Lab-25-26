#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8890
#define MAX_CONN 10
#define BUFFER_SIZE 1024

int use_thread_mode = 0;

typedef struct {
    int client_sock;
    struct sockaddr_in client_addr;
} client_data_t;


/* -------- PROCESS VERSION -------- */

void handle_client_process(int client_sock, struct sockaddr_in client_addr) {

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    printf("[PROCESS %d] Handling client %s:%d\n",
           getpid(),
           inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));

    while ((bytes_read = read(client_sock, buffer, sizeof(buffer))) > 0) {

        write(client_sock, buffer, bytes_read);
    }

    printf("[PROCESS %d] Client disconnected %s:%d\n",
           getpid(),
           inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));

    close(client_sock);
}


/* -------- THREAD VERSION -------- */

void *handle_client_thread(void *arg) {

    client_data_t *data = (client_data_t *)arg;

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    printf("[THREAD %lu] Handling client %s:%d\n",
           pthread_self(),
           inet_ntoa(data->client_addr.sin_addr),
           ntohs(data->client_addr.sin_port));

    while ((bytes_read = read(data->client_sock, buffer, sizeof(buffer))) > 0) {

        write(data->client_sock, buffer, bytes_read);
    }

    printf("[THREAD %lu] Client disconnected %s:%d\n",
           pthread_self(),
           inet_ntoa(data->client_addr.sin_addr),
           ntohs(data->client_addr.sin_port));

    close(data->client_sock);

    free(data);

    pthread_exit(NULL);
}


/* -------- MAIN SERVER -------- */

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: %s fork | thread\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "thread") == 0)
        use_thread_mode = 1;
    else if (strcmp(argv[1], "fork") == 0)
        use_thread_mode = 0;
    else {
        printf("Invalid mode. Use 'fork' or 'thread'\n");
        return 1;
    }


    int server_sock, client_sock;

    struct sockaddr_in server_addr, client_addr;

    socklen_t client_addr_len = sizeof(client_addr);

    pthread_t thread_id;


    /* STEP 1: CREATE SOCKET */

    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (server_sock == -1) {
        perror("Socket creation failed");
        exit(1);
    }


    /* STEP 2: SET ADDRESS */

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);


    /* STEP 3: BIND */

    if (bind(server_sock,
             (struct sockaddr *)&server_addr,
             sizeof(server_addr)) == -1) {

        perror("Bind failed");
        close(server_sock);
        exit(1);
    }


    /* STEP 4: LISTEN */

    if (listen(server_sock, MAX_CONN) == -1) {

        perror("Listen failed");
        close(server_sock);
        exit(1);
    }


    printf("Server running in %s mode...\n",
           use_thread_mode ? "THREAD" : "FORK");


    /* STEP 5: ACCEPT CLIENTS */

    while (1) {

        client_sock = accept(server_sock,
                             (struct sockaddr *)&client_addr,
                             &client_addr_len);

        if (client_sock == -1) {
            perror("Accept failed");
            continue;
        }


        /* PRINT CLIENT CONNECTED */

        printf("Client connected from %s:%d\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));


        /* THREAD MODE */

        if (use_thread_mode) {

            client_data_t *data =
                (client_data_t *)malloc(sizeof(client_data_t));

            data->client_sock = client_sock;
            data->client_addr = client_addr;

            if (pthread_create(&thread_id,
                               NULL,
                               handle_client_thread,
                               data) != 0) {

                perror("Thread creation failed");

                close(client_sock);

                free(data);
            }

            pthread_detach(thread_id);
        }


        /* FORK MODE */

        else {

            if (fork() == 0) {

                close(server_sock);

                handle_client_process(client_sock, client_addr);

                exit(0);
            }

            close(client_sock);
        }
    }


    close(server_sock);

    return 0;
}
