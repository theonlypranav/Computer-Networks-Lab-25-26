#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/time.h>

#define BUFLEN 512
#define PORT 8882
#define TIMEOUT 2   // Timeout = 2 seconds

typedef struct packet{
    int seq_no;     // sequence number (0 or 1)
    int size;       // size of payload
    int is_last;    // last packet flag
    int type;       // 0 = DATA, 1 = ACK
    char data[BUFLEN];
} PACKET;

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    struct sockaddr_in si_other;
    int s, slen = sizeof(si_other);

    PACKET send_pkt, rcv_pkt;

    // Open input file
    FILE *fp = fopen("input.txt", "r");

    // Create UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        die("socket");

    // Initialize server address
    memset(&si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    si_other.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Set timeout for recvfrom()
    struct timeval tv;
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;

    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    int seq = 0; // initial sequence number

    while(1)
    {
        char buffer[BUFLEN];

        // Read one line from file
        if (fgets(buffer, BUFLEN, fp) == NULL)
            break;

        // Prepare packet
        send_pkt.seq_no = seq;
        send_pkt.type = 0; // DATA
        send_pkt.size = strlen(buffer);
        send_pkt.is_last = feof(fp) ? 1 : 0;

        strcpy(send_pkt.data, buffer);

SEND_AGAIN:

        // Send packet
        if (sendto(s, &send_pkt, sizeof(send_pkt), 0,
            (struct sockaddr *) &si_other, slen) == -1)
            die("sendto()");

        printf("SENT DATA: Seq.No %d of size %d Bytes\n",
               send_pkt.seq_no, send_pkt.size);

        // Wait for ACK
        int recv_len = recvfrom(s, &rcv_pkt, sizeof(rcv_pkt), 0,
                        (struct sockaddr *) &si_other, &slen);

        // If timeout occurs → retransmit
        if (recv_len == -1)
        {
            printf("TIMEOUT → RESENT DATA: Seq.No %d of size %d Bytes\n",
                   send_pkt.seq_no, send_pkt.size);
            goto SEND_AGAIN;
        }

        // Check valid ACK
        if (rcv_pkt.type == 1 && rcv_pkt.seq_no == seq)
        {
            printf("RCVD ACK: for PKT with Seq.No %d\n", seq);

            // Move to next sequence number
            seq = 1 - seq;
        }
        else
        {
            // Wrong ACK → retransmit
            goto SEND_AGAIN;
        }
    }

    fclose(fp);
    close(s);
    return 0;
}