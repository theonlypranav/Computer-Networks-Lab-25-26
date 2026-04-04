#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define PORT1 8881  // TCP port for c1
#define PORT2 8882  // TCP port for c2
#define BUFLEN 512
#define PDR 10      // Packet Drop Rate in %

typedef struct {
    int size;
    int seq_no;
    char TYPE; // 'D' for DATA, 'A' for ACK
    char data[BUFLEN];
} PACKET;

// Simulate packet drop: returns 1 if packet should be dropped
int drop_packet() {
    int r = rand() % 100;
    return (r < PDR);
}

int main() {
    srand(time(NULL));

    int s1_sock, s2_sock, new_sock1, new_sock2;
    struct sockaddr_in server1, server2;
    socklen_t len1, len2;

    // 1. Create listening sockets for c1 and c2
    s1_sock = socket(AF_INET, SOCK_STREAM, 0);
    s2_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(s1_sock < 0 || s2_sock < 0){ perror("socket"); exit(1); }

    server1.sin_family = AF_INET;
    server1.sin_addr.s_addr = INADDR_ANY;
    server1.sin_port = htons(PORT1);

    server2.sin_family = AF_INET;
    server2.sin_addr.s_addr = INADDR_ANY;
    server2.sin_port = htons(PORT2);

    if(bind(s1_sock, (struct sockaddr*)&server1, sizeof(server1)) < 0) { perror("bind1"); exit(1); }
    if(bind(s2_sock, (struct sockaddr*)&server2, sizeof(server2)) < 0) { perror("bind2"); exit(1); }

    listen(s1_sock, 1);
    listen(s2_sock, 1);

    printf("Waiting for clients c1 and c2 to connect...\n");
    len1 = sizeof(server1); len2 = sizeof(server2);

    new_sock1 = accept(s1_sock, (struct sockaddr*)&server1, &len1);
    new_sock2 = accept(s2_sock, (struct sockaddr*)&server2, &len2);

    printf("Both clients connected.\n");

    FILE *fp = fopen("list.txt","w");
    if(fp == NULL){ perror("fopen"); exit(1); }

    int done1 = 0, done2 = 0; // Track end of files
    PACKET pkt_name, pkt_id, ack;

    int exp_seq_c1 = 0, exp_seq_c2 = 0; // Expected seq_no for stop-and-wait

    while(!done1 || !done2){
        // --- Receive from c1 (name) ---
        if(!done1){
            int n = recv(new_sock1, &pkt_name, sizeof(pkt_name), 0);
            if(n <= 0){ done1 = 1; continue; }

            if(drop_packet()){
                printf("DROP PKT: Seq. No. = %d from c1\n", pkt_name.seq_no);
            } else if(pkt_name.seq_no == exp_seq_c1){
                printf("RCVD PKT: Seq. No. = %d, Size = %d Bytes from c1\n", pkt_name.seq_no, pkt_name.size);

                // Send ACK
                ack.seq_no = pkt_name.seq_no;
                ack.TYPE = 'A';
                send(new_sock1, &ack, sizeof(ack), 0);
                printf("SENT ACK: Seq. No. = %d to c1\n", ack.seq_no);

                exp_seq_c1 += pkt_name.size;

            } else {
                printf("OUT-OF-ORDER PKT: Seq. No. = %d from c1\n", pkt_name.seq_no);
            }

            // Check end-of-file marker
            if(pkt_name.data[0]=='.'){ done1 = 1; continue; }
        }

        // --- Receive from c2 (ID) ---
        if(!done2){
            int n = recv(new_sock2, &pkt_id, sizeof(pkt_id), 0);
            if(n <= 0){ done2 = 1; continue; }

            if(drop_packet()){
                printf("DROP PKT: Seq. No. = %d from c2\n", pkt_id.seq_no);
            } else if(pkt_id.seq_no == exp_seq_c2){
                printf("RCVD PKT: Seq. No. = %d, Size = %d Bytes from c2\n", pkt_id.seq_no, pkt_id.size);

                // Send ACK
                ack.seq_no = pkt_id.seq_no;
                ack.TYPE = 'A';
                send(new_sock2, &ack, sizeof(ack), 0);
                printf("SENT ACK: Seq. No. = %d to c2\n", ack.seq_no);

                exp_seq_c2 += pkt_id.size;

            } else {
                printf("OUT-OF-ORDER PKT: Seq. No. = %d from c2\n", pkt_id.seq_no);
            }

            // Check end-of-file marker
            if(pkt_id.data[0]=='.'){ done2 = 1; continue; }
        }

        // --- Write to list.txt alternately ---
        if(pkt_name.data[0] != '.' && pkt_id.data[0] != '.'){
            fprintf(fp, "%s,%s,", pkt_name.data, pkt_id.data);
        }
    }

    fclose(fp);
    close(new_sock1); close(new_sock2);
    close(s1_sock); close(s2_sock);
    printf("File list.txt created successfully.\n");
    return 0;
}