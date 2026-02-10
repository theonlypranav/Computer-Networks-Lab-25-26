/* =========================================================
   BROKEN FTP CLIENT (TCP)
   - Downloads file from server
   - Supports partial download using offset
   ========================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    int bytesReceived;
    char recvBuff[256];      // Buffer to receive file data
    char commandBuff[2];     // Stores command (0/1/2)
    char offsetBuff[10];     // Stores offset value as string
    int command;
    int offset = 0;

    struct sockaddr_in serv_addr;

    /* Create TCP socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /* Configure server address */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5001);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* Establish TCP connection */
    connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    /* Open destination file in append mode
       ab+ allows resume of download */
    FILE *fp = fopen("destination_file.txt", "ab+");

    /* Move file pointer to end to calculate offset */
    fseek(fp, 0, SEEK_END);
    offset = ftell(fp);

    /* User selects download mode */
    printf("Enter command:\n");
    printf("0 -> Full file\n1 -> Manual offset\n2 -> Auto offset\n");
    scanf("%d", &command);

    /* Send command to server */
    sprintf(commandBuff, "%d", command);
    write(sockfd, commandBuff, 2);

    /* Handle offset sending */
    if (command == 1) {
        printf("Enter offset: ");
        scanf("%d", &offset);
    }

    if (command == 1 || command == 2) {
        sprintf(offsetBuff, "%d", offset);
        write(sockfd, offsetBuff, 10);
    }

    /* Receive file data in chunks */
    while ((bytesReceived = read(sockfd, recvBuff, 256)) > 0) {
        fwrite(recvBuff, 1, bytesReceived, fp);
        printf("Received %d bytes\n", bytesReceived);
    }

    fclose(fp);
    close(sockfd);
    return 0;
}
