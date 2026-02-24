#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8898
#define BUFFER_SIZE 1024
#define DB_FILE "database.txt"

pthread_mutex_t db_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int client_sock;
} client_data_t;


/* -------- Utility Functions -------- */

int key_exists(int key, char *value_out) {

    FILE *fp = fopen(DB_FILE, "r");
    if (!fp) return 0;

    int k;
    char v[BUFFER_SIZE];

    while (fscanf(fp, "%d %s\n", &k, v) != EOF) {
        if (k == key) {
            strcpy(value_out, v);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}


int put_key(int key, char *value) {

    char temp[BUFFER_SIZE];

    if (key_exists(key, temp))
        return 0;

    FILE *fp = fopen(DB_FILE, "a");
    if (!fp) return 0;

    fprintf(fp, "%d %s\n", key, value);
    fclose(fp);
    return 1;
}


int delete_key(int key) {

    FILE *fp = fopen(DB_FILE, "r");
    if (!fp) return 0;

    FILE *temp = fopen("temp.txt", "w");

    int k;
    char v[BUFFER_SIZE];
    int found = 0;

    while (fscanf(fp, "%d %s\n", &k, v) != EOF) {
        if (k == key)
            found = 1;
        else
            fprintf(temp, "%d %s\n", k, v);
    }

    fclose(fp);
    fclose(temp);

    remove(DB_FILE);
    rename("temp.txt", DB_FILE);

    return found;
}


/* -------- Client Handler -------- */

void *handle_client(void *arg) {

    client_data_t *data = (client_data_t *)arg;
    int sock = data->client_sock;
    free(data);

    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    while (1) {

        int bytes = read(sock, buffer, BUFFER_SIZE - 1);
        if (bytes <= 0) break;

        buffer[bytes] = '\0';

        if (strncmp(buffer, "Bye", 3) == 0) {
            write(sock, "Goodbye\n", 8);
            break;
        }

        char command[10];
        int key;
        char value[BUFFER_SIZE];

        sscanf(buffer, "%s %d %s", command, &key, value);

        pthread_mutex_lock(&db_mutex);

        if (strcmp(command, "put") == 0) {

            if (put_key(key, value))
                strcpy(response, "OK\n");
            else
                strcpy(response, "ERROR: Key exists\n");
        }

        else if (strcmp(command, "get") == 0) {

            if (key_exists(key, value)) {
                sprintf(response, "%s\n", value);
            } else {
                strcpy(response, "ERROR: Key not found\n");
            }
        }

        else if (strcmp(command, "del") == 0) {

            if (delete_key(key))
                strcpy(response, "OK\n");
            else
                strcpy(response, "ERROR: Key not found\n");
        }

        else {
            strcpy(response, "Invalid Command\n");
        }

        pthread_mutex_unlock(&db_mutex);

        write(sock, response, strlen(response));
    }

    close(sock);
    pthread_exit(NULL);
}


/* -------- Main Server -------- */

int main() {

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pthread_t thread_id;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Socket error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_sock, 10);

    printf("Key-Value Server running on port %d...\n", PORT);

    while (1) {

        client_sock = accept(server_sock,
                             (struct sockaddr *)&client_addr,
                             &client_len);

        printf("Client connected: %s:%d\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));

        client_data_t *data = malloc(sizeof(client_data_t));
        data->client_sock = client_sock;

        pthread_create(&thread_id, NULL, handle_client, data);
        pthread_detach(thread_id);
    }

    close(server_sock);
    return 0;
}
