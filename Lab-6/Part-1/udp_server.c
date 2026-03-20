#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>

#define BUFLEN 512
#define PORT 8882

void die(char *s)
{
    perror(s);
    exit(1);
}

typedef struct packet1{
    int sq_no;
} ACK_PKT;

typedef struct packet2{
    int sq_no;
    char data[BUFLEN];
} DATA_PKT;

int main(void)
{
    struct sockaddr_in si_me, si_other;
    int s, slen = sizeof(si_other), recv_len;

    DATA_PKT rcv_pkt;
    ACK_PKT ack_pkt;

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        die("socket");

    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(s , (struct sockaddr*)&si_me, sizeof(si_me)) == -1)
        die("bind");

    int state = 0;

    while(1)
    {
        switch(state)
        {
            case 0:
                printf("Waiting for packet 0...\n");

                if ((recv_len = recvfrom(s, &rcv_pkt, sizeof(rcv_pkt), 0,
                    (struct sockaddr *) &si_other, &slen)) == -1)
                    die("recvfrom()");

                if (rcv_pkt.sq_no == 0)
                {
                    printf("Received seq 0: %s", rcv_pkt.data);

                    ack_pkt.sq_no = 0;

                    if (sendto(s, &ack_pkt, sizeof(ack_pkt), 0,
                        (struct sockaddr*) &si_other, slen) == -1)
                        die("sendto()");

                    state = 1;
                }
                break;

            case 1:
                printf("Waiting for packet 1...\n");

                if ((recv_len = recvfrom(s, &rcv_pkt, sizeof(rcv_pkt), 0,
                    (struct sockaddr *) &si_other, &slen)) == -1)
                    die("recvfrom()");

                if (rcv_pkt.sq_no == 1)
                {
                    printf("Received seq 1: %s", rcv_pkt.data);

                    ack_pkt.sq_no = 1;

                    if (sendto(s, &ack_pkt, sizeof(ack_pkt), 0,
                        (struct sockaddr*) &si_other, slen) == -1)
                        die("sendto()");

                    state = 0;
                }
                break;
        }
    }

    close(s);
    return 0;
}