Lab Sheet 4
Task1: Same message as client gives is echoed back by the server

Echo logic in UDP Server (with comments)
// Receive data from client
// recvfrom() stores the incoming message in 'buf'
// It also stores client IP and port in 'si_other'
recv_len = recvfrom(s, buf, BUFLEN, 0,
                    (struct sockaddr *)&si_other, &slen);

// Print the received message (for verification)
printf("Data received: %s\n", buf);

// Echo functionality:
// Send the SAME data back to the SAME client
// 'buf' contains the received message
// 'recv_len' ensures only received bytes are sent
sendto(s, buf, recv_len, 0,
       (struct sockaddr *)&si_other, slen);

Echo logic in UDP Client (with comments)
// Send message to server
// Client sends user-entered message to server
sendto(s, message, strlen(message), 0,
       (struct sockaddr *)&si_other, slen);

// Receive echoed message from server
// recvfrom() waits until server sends back the same message
recvfrom(s, buf, BUFLEN, 0,
         (struct sockaddr *)&si_other, &slen);

// Display the echoed message
puts(buf);


Exercise1: 
The server logic was modified to generate a random number (between 1 and 6) using rand() instead of simply echoing back the received message.
The client now sends a guess (number entered by the user) instead of an arbitrary text message.
The server compares the client’s guess with the generated number.
Based on the comparison, the server sends a result message (WIN/LOSE) back to the client.
The client displays the outcome message received from the server.
UDP communication remains unchanged and still uses sendto() and recvfrom() without any connection establishment.

Task2:
This program implements a simple FTP-like file transfer using TCP and provides three download modes to demonstrate broken download handling.

Option 0 (Full File Download):
The client requests the complete file from the server by sending command 0. The server starts sending the file from byte offset 0, and the client writes all received data into the destination file. This option behaves like a normal FTP “get” operation.

Option 1 (Manual Offset Download):
The client manually specifies a byte offset from where the file should be downloaded. The server seeks to the given offset in the source file and sends only the remaining portion. This option demonstrates offset-based file transfer but does not automatically resume a previous download.

Option 2 (Automatic Offset / Resume Download):
The client automatically calculates the offset using the size of its partially downloaded local file. This offset is sent to the server, which resumes transmission from that point onward. This option implements the broken download (resume) feature, avoiding re-downloading data already present.

These options illustrate how file transfer behavior can be controlled using simple commands and offsets while relying on TCP for reliable and ordered delivery.

The broken part allows resuming an interrupted file transfer by continuing from a byte offset instead of restarting from the beginning.
