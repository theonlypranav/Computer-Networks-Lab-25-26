# Exercise 2 --- Concurrent Key-Value Store using TCP

## Objective

Create a concurrent key-value database server using TCP sockets.

Supports commands: - put key value - get key - del key - Bye

Data stored persistently in database.txt

------------------------------------------------------------------------

## Server Working Logic

1.  Create socket

``` c
socket(AF_INET, SOCK_STREAM, 0);
```

2.  Bind and listen

``` c
bind(...);
listen(...);
```

3.  Accept clients

``` c
client_sock = accept(...);
```

4.  Create thread for each client

``` c
pthread_create(&thread_id, NULL, handle_client, data);
```

5.  Parse client command

``` c
sscanf(buffer, "%s %d %s", command, &key, value);
```

6.  Perform operation

Put:

``` c
fprintf(fp, "%d %s\n", key, value);
```

Get:

``` c
if (key_exists(key))
    send(value);
```

Delete:

``` c
remove(key);
```

7.  Send response

``` c
write(sock, response, strlen(response));
```

------------------------------------------------------------------------

## Client Working Logic

1.  Connect to server

``` c
connect(sock, ...);
```

2.  Send command

``` c
write(sock, buffer, strlen(buffer));
```

3.  Receive response

``` c
read(sock, buffer, sizeof(buffer));
```

------------------------------------------------------------------------

## Database Storage

Stored in file:

    database.txt

Example:

    1 hello
    2 world

------------------------------------------------------------------------

## Concurrency Control

Mutex prevents race conditions:

``` c
pthread_mutex_lock(&db_mutex);
pthread_mutex_unlock(&db_mutex);
```

------------------------------------------------------------------------

## Key Concepts

-   TCP socket communication
-   File-based database
-   Multi-client concurrency using threads
-   Persistent storage
-   Thread synchronization using mutex

------------------------------------------------------------------------

## Result

Multiple clients can simultaneously put/get/delete keys and share the
same persistent database.
