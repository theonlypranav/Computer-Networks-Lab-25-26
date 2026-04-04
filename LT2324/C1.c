#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8881
#define BUFLEN 512
#define TIMEOUT 2  // seconds

typedef struct {
    int size;
    int seq_no;
    char TYPE;  // 'D' for DATA, 'A' for ACK
    char data[BUFLEN];
} PACKET;

void die(char *s){ perror(s); exit(1); }

int main() {
    int sock;
    struct sockaddr_in server;
    socklen_t slen = sizeof(server);
    PACKET pkt, rcv_ack;

    // Read file
    FILE *fp = fopen("name.txt", "r");
    if(!fp){ perror("fopen"); exit(1); }

    char buffer[BUFLEN];
    int seq_no = 0;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) die("socket");

    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = inet_addr(SERVER_IP);

    if(connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) die("connect");

    while(fgets(buffer, sizeof(buffer), fp)){
        char *token = strtok(buffer, ",\n");
        while(token){
            int size = strlen(token);
            pkt.seq_no = seq_no;
            pkt.size = size;
            pkt.TYPE = 'D';
            strcpy(pkt.data, token);

            int ack_received = 0;
            while(!ack_received){
                send(sock, &pkt, sizeof(pkt), 0);
                printf("SENT PKT: Seq. No. = %d, Size = %d Bytes\n", pkt.seq_no, pkt.size);

                // Wait for ACK with timeout
                struct timeval tv;
                tv.tv_sec = TIMEOUT;
                tv.tv_usec = 0;
                fd_set readfds;
                FD_ZERO(&readfds);
                FD_SET(sock, &readfds);

                int rv = select(sock+1, &readfds, NULL, NULL, &tv);
                if(rv > 0 && FD_ISSET(sock, &readfds)){
                    recv(sock, &rcv_ack, sizeof(rcv_ack), 0);
                    if(rcv_ack.seq_no == pkt.seq_no){
                        printf("RCVD ACK: Seq. No. = %d\n", rcv_ack.seq_no);
                        ack_received = 1;
                    }
                } else {
                    printf("TIMEOUT, RE-TRANSMIT PKT: Seq. No. = %d, Size = %d Bytes\n", pkt.seq_no, pkt.size);
                }
            }

            seq_no += pkt.size;
            token = strtok(NULL, ",\n");
        }
    }

    // Send end-of-file marker
    pkt.seq_no = seq_no;
    pkt.size = 1;
    pkt.TYPE = 'D';
    strcpy(pkt.data, ".");
    send(sock, &pkt, sizeof(pkt), 0);

    fclose(fp);
    close(sock);
    return 0;
}