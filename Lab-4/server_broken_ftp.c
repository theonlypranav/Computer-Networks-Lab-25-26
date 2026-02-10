/* =========================================================
   BROKEN FTP SERVER (TCP)
   - Sends file to client
   - Supports offset-based partial transfer
   ========================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int listenfd, connfd;
    struct sockaddr_in serv_addr;
    char commandBuff[2];
    char offsetBuff[10];
    int command, offset = 0;

    /* Create TCP socket */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    /* Configure server address */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5001);

    /* Bind socket to port */
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    /* Listen for incoming connections */
    listen(listenfd, 5);

    while (1) {
        /* Accept client connection */
        connfd = accept(listenfd, NULL, NULL);

        /* Read command from client */
        read(connfd, commandBuff, 2);
        sscanf(commandBuff, "%d", &command);

        /* Read offset if partial download */
        if (command != 0) {
            read(connfd, offsetBuff, 10);
            sscanf(offsetBuff, "%d", &offset);
        }

        /* Open source file */
        FILE *fp = fopen("source_file.txt", "rb");

        /* Move to requested offset */
        fseek(fp, offset, SEEK_SET);

        /* Send file data in chunks */
        while (1) {
            char buffer[256];
            int nread = fread(buffer, 1, 256, fp);

            if (nread > 0)
                write(connfd, buffer, nread);

            if (nread < 256)
                break; // End of file
        }

        fclose(fp);
        close(connfd);
    }

    return 0;
}
