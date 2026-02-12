/* UDP Guessing Game Server */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define BUFLEN 512
#define PORT 8889

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    struct sockaddr_in si_me, si_other;
    int s, slen = sizeof(si_other), recv_len;
    char buf[BUFLEN];
    char reply[BUFLEN];

    // Create UDP socket
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        die("socket");

    memset(&si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind
    if (bind(s, (struct sockaddr *)&si_me, sizeof(si_me)) == -1)
        die("bind");

    // Seed random number generator
    srand(time(NULL));

    printf("Guessing Game Server started...\n");

    while (1)
    {
        printf("Waiting for guess...\n");

        memset(buf, '\0', BUFLEN);

        if ((recv_len = recvfrom(s, buf, BUFLEN, 0,
                                 (struct sockaddr *)&si_other, &slen)) == -1)
            die("recvfrom");

        int client_guess = atoi(buf);
        int secret_number = (rand() % 6) + 1;

        printf("Client guessed: %d | Secret number: %d\n",
               client_guess, secret_number);

        if (client_guess == secret_number)
            strcpy(reply, "🎉 Correct guess! You WIN!");
        else
            sprintf(reply, "❌ Wrong guess! You LOSE. Correct number was %d",
                    secret_number);

        if (sendto(s, reply, strlen(reply), 0,
                   (struct sockaddr *)&si_other, slen) == -1)
            die("sendto");
    }

    close(s);
    return 0;
}
