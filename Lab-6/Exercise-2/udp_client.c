#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/select.h>
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
    struct sockaddr_in si_other;
    int s, slen = sizeof(si_other);

    DATA_PKT send_pkt;
    ACK_PKT rcv_ack;

    srand(time(0));

    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        die("socket");

    memset(&si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    si_other.sin_addr.s_addr = inet_addr("127.0.0.1");

    int seq = 0;

    while (1) {
        printf("\nEnter message for seq %d: ", seq);
        fgets(send_pkt.data, BUFLEN, stdin);

        send_pkt.sq_no = seq;

        while (1) {
            // SEND PACKET
            if (sendto(s, &send_pkt, sizeof(send_pkt), 0,
                (struct sockaddr*)&si_other, slen) == -1)
                die("sendto");

            printf("Sent packet %d\n", seq);

            // 🔴 TIMEOUT USING SELECT
            fd_set readfds;
            struct timeval tv;

            FD_ZERO(&readfds);
            FD_SET(s, &readfds);

            tv.tv_sec = 3;
            tv.tv_usec = 0;

            int rv = select(s + 1, &readfds, NULL, NULL, &tv);

            if (rv == 0) {
                printf("Timeout → Retransmitting packet %d\n", seq);
                continue;
            }

            if (recvfrom(s, &rcv_ack, sizeof(rcv_ack), 0,
                (struct sockaddr*)&si_other, &slen) == -1)
                die("recvfrom");

            // 🔴 RANDOM ACK LOSS
            if (rand() % 3 == 0) {
                printf("ACK LOST intentionally\n");
                continue;
            }

            printf("Received ACK %d\n", rcv_ack.sq_no);

            if (rcv_ack.sq_no == seq) {
                seq = 1 - seq; // toggle
                break;
            }
        }
    }

    close(s);
    return 0;
}