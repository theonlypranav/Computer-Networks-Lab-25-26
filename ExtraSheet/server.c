#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<time.h>

#define BUFLEN 512
#define PORT 8882
#define PDR 10   // Packet Drop Rate (10%)

typedef struct packet{
    int seq_no;     // sequence number (0 or 1)
    int size;       // size of actual data
    int is_last;    // 1 if last packet, else 0
    int type;       // 0 = DATA, 1 = ACK
    char data[BUFLEN]; // payload
} PACKET;

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    struct sockaddr_in si_me, si_other;
    int s, slen = sizeof(si_other), recv_len;

    PACKET rcv_pkt, ack_pkt;

    // Open output file (write mode)
    FILE *fp = fopen("output.txt", "w");

    // Create UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        die("socket");

    // Initialize server address structure
    memset(&si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind socket to port
    if(bind(s , (struct sockaddr*)&si_me, sizeof(si_me)) == -1)
        die("bind");

    srand(time(0)); // seed for random packet drop

    int expected_seq = 0; // expected sequence number

    while(1)
    {
        // Receive packet from client
        if ((recv_len = recvfrom(s, &rcv_pkt, sizeof(rcv_pkt), 0,
            (struct sockaddr *) &si_other, &slen)) == -1)
            die("recvfrom()");

        // Check if packet is DATA
        if (rcv_pkt.type == 0)
        {
            // Simulate packet loss
            int drop = rand() % 100;
            if (drop < PDR)
            {
                printf("DROP DATA: Seq.No %d of size %d Bytes\n",
                       rcv_pkt.seq_no, rcv_pkt.size);
                continue; // do not send ACK
            }

            // Packet successfully received
            printf("RCVD DATA: Seq.No %d of size %d Bytes\n",
                   rcv_pkt.seq_no, rcv_pkt.size);

            // Check if it's the expected packet
            if (rcv_pkt.seq_no == expected_seq)
            {
                // Write data directly to file
                fwrite(rcv_pkt.data, 1, rcv_pkt.size, fp);

                // Prepare ACK
                ack_pkt.seq_no = expected_seq;
                ack_pkt.type = 1;

                // Send ACK to client
                if (sendto(s, &ack_pkt, sizeof(ack_pkt), 0,
                    (struct sockaddr*) &si_other, slen) == -1)
                    die("sendto()");

                printf("SENT ACK: for PKT with Seq.No %d\n", expected_seq);

                // Toggle expected sequence number (0 ↔ 1)
                expected_seq = 1 - expected_seq;

                // If last packet, stop
                if (rcv_pkt.is_last)
                    break;
            }
            else
            {
                // Duplicate packet → resend last ACK
                ack_pkt.seq_no = 1 - expected_seq;
                ack_pkt.type = 1;

                sendto(s, &ack_pkt, sizeof(ack_pkt), 0,
                    (struct sockaddr*) &si_other, slen);
            }
        }
    }

    fclose(fp);
    close(s);
    return 0;
}