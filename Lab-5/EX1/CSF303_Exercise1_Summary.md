# Exercise 1 --- Concurrent TCP Echo Server (Fork & Thread)

## Objective

Create a concurrent TCP Echo Server that handles multiple clients
using: - fork() (process-based) - pthread() (thread-based)

Client sends message → Server echoes message back.

------------------------------------------------------------------------

## Working Logic

### Server Workflow

1.  Create socket

``` c
server_sock = socket(AF_INET, SOCK_STREAM, 0);
```

2.  Bind socket to port

``` c
bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
```

3.  Listen for clients

``` c
listen(server_sock, MAX_CONN);
```

4.  Accept client

``` c
client_sock = accept(server_sock, ...);
```

5.  Create concurrent handler

Fork mode:

``` c
if (fork() == 0) {
    handle_client(client_sock);
}
```

Thread mode:

``` c
pthread_create(&thread_id, NULL, handle_client_thread, data);
```

6.  Read and write (Echo)

``` c
read(client_sock, buffer, sizeof(buffer));
write(client_sock, buffer, bytes_read);
```

------------------------------------------------------------------------

## Client Workflow

1.  Create socket

``` c
sock = socket(AF_INET, SOCK_STREAM, 0);
```

2.  Connect to server

``` c
connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
```

3.  Send message

``` c
write(sock, buffer, strlen(buffer));
```

4.  Receive reply

``` c
read(sock, buffer, sizeof(buffer));
```

------------------------------------------------------------------------

## Key Concepts

-   TCP communication
-   Client-server model
-   fork() creates new process
-   pthread_create() creates new thread
-   Concurrent handling of multiple clients

------------------------------------------------------------------------

## Result

Multiple clients can communicate with server simultaneously.
