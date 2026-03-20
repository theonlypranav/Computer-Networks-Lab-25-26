#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>

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

    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        die("socket");

    memset(&si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    si_other.sin_addr.s_addr = inet_addr("127.0.0.1");

    int seq = 0;

    while (1) {
        printf("Enter message for seq %d: ", seq);
        fgets(send_pkt.data, BUFLEN, stdin);

        send_pkt.sq_no = seq;

        // SEND
        if (sendto(s, &send_pkt, sizeof(send_pkt), 0,
            (struct sockaddr*)&si_other, slen) == -1)
            die("sendto");

        printf("Packet %d sent\n", seq);

        // WAIT FOR ACK (blocking)
        if (recvfrom(s, &rcv_ack, sizeof(rcv_ack), 0,
            (struct sockaddr*)&si_other, &slen) == -1)
            die("recvfrom");

        printf("Received ACK %d\n", rcv_ack.sq_no);

        if (rcv_ack.sq_no == seq) {
            seq = 1 - seq; // alternate
        }
    }

    close(s);
    return 0;
}