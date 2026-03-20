#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<time.h>

#define BUFLEN 512
#define PORT 8882

typedef struct {
    int sq_no;
} ACK_PKT;

typedef struct {
    int sq_no;
    char data[BUFLEN];
} DATA_PKT;

void die(char *s) {
    perror(s);
    exit(1);
}

int main() {
    struct sockaddr_in si_me, si_other;
    int s, slen = sizeof(si_other), recv_len;

    DATA_PKT rcv_pkt;
    ACK_PKT ack_pkt;

    srand(time(0));

    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        die("socket");

    memset(&si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(s, (struct sockaddr*)&si_me, sizeof(si_me)) == -1)
        die("bind");

    int expected_seq = 0;

    while (1) {
        printf("\nWaiting for packet %d...\n", expected_seq);

        if ((recv_len = recvfrom(s, &rcv_pkt, sizeof(rcv_pkt), 0,
            (struct sockaddr*)&si_other, &slen)) == -1)
            die("recvfrom");

        // 🔴 RANDOM DATA LOSS
        if (rand() % 3 == 0) {
            printf("Packet %d LOST intentionally\n", rcv_pkt.sq_no);
            continue;
        }

        printf("Received packet %d: %s", rcv_pkt.sq_no, rcv_pkt.data);

        if (rcv_pkt.sq_no == expected_seq) {
            ack_pkt.sq_no = expected_seq;

            if (sendto(s, &ack_pkt, sizeof(ack_pkt), 0,
                (struct sockaddr*)&si_other, slen) == -1)
                die("sendto");

            printf("ACK %d sent\n", expected_seq);

            expected_seq = 1 - expected_seq; // toggle
        } 
        else {
            // 🔴 DUPLICATE PACKET
            ack_pkt.sq_no = 1 - expected_seq;

            sendto(s, &ack_pkt, sizeof(ack_pkt), 0,
                (struct sockaddr*)&si_other, slen);

            printf("Duplicate packet → Resent ACK %d\n", ack_pkt.sq_no);
        }
    }

    close(s);
    return 0;
}