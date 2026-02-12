/* UDP Guessing Game Client */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFLEN 512
#define PORT 8889

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    struct sockaddr_in si_other;
    int s, slen = sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];

    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        die("socket");

    memset(&si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    si_other.sin_addr.s_addr = inet_addr("127.0.0.9");

    printf("Guess a number between 1 and 6: ");
    fgets(message, BUFLEN, stdin);

    // Send guess
    if (sendto(s, message, strlen(message), 0,
               (struct sockaddr *)&si_other, slen) == -1)
        die("sendto");

    memset(buf, '\0', BUFLEN);

    // Receive result
    if (recvfrom(s, buf, BUFLEN, 0,
                 (struct sockaddr *)&si_other, &slen) == -1)
        die("recvfrom");

    printf("Server says: %s\n", buf);

    close(s);
    return 0;
}
