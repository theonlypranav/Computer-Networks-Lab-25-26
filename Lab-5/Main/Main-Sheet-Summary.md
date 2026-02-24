# Concurrent TCP Echo Server --- Logic and Working Summary

## Overview

This program implements a **Concurrent TCP Echo Server** that can handle
multiple clients simultaneously using either:

-   `fork()` → Process-based concurrency
-   `pthread()` → Thread-based concurrency

The server listens for client connections, receives messages, and sends
the same message back (echo).

------------------------------------------------------------------------

## Step-by-Step Working

### 1. Socket Creation

    socket(AF_INET, SOCK_STREAM, 0)

-   Creates a TCP socket
-   Endpoint for communication

------------------------------------------------------------------------

### 2. Bind Socket to Port

    bind(server_sock, ...)

-   Assigns server to a specific port (8898)
-   Makes server reachable

------------------------------------------------------------------------

### 3. Listen for Connections

    listen(server_sock, MAX_CONN)

-   Puts server in listening mode
-   Allows multiple clients to wait in queue

------------------------------------------------------------------------

### 4. Accept Client Connection

    client_sock = accept(server_sock, ...)

-   Accepts incoming client
-   Creates new socket for client communication
-   Retrieves client IP and port

------------------------------------------------------------------------

### 5. Create Concurrent Handler

#### Fork Version

    fork()

-   Creates new process
-   Child handles client
-   Parent accepts new clients

#### Thread Version

    pthread_create()

-   Creates new thread
-   Thread handles client
-   Main thread accepts new clients

------------------------------------------------------------------------

### 6. Handle Client Communication

    read(client_sock, buffer, size)
    write(client_sock, buffer, size)

-   Reads message from client
-   Sends same message back (echo)

------------------------------------------------------------------------

### 7. Close Connection

    close(client_sock)

-   Terminates client connection
-   Frees resources

------------------------------------------------------------------------

## Concurrent Execution Flow

    Server starts
        |
     socket()
        |
     bind()
        |
     listen()
        |
     accept()
        |
     fork() OR pthread_create()
        |
     handle_client()
        |
     read() and write()
        |
     close()

------------------------------------------------------------------------

## Key Concepts Used

-   TCP Socket Programming
-   Client--Server Communication
-   Concurrency using fork()
-   Concurrency using pthreads()
-   Process vs Thread management
-   Network address handling

------------------------------------------------------------------------

## Fork vs Thread Summary

  Feature          fork()     pthread()
  ---------------- ---------- -----------
  Creates          Process    Thread
  Memory           Separate   Shared
  Speed            Slower     Faster
  Resource usage   High       Low
  Scalability      Lower      Higher

------------------------------------------------------------------------

## Purpose of Program

-   Handle multiple clients simultaneously
-   Enable reliable TCP communication
-   Demonstrate concurrent server implementation

------------------------------------------------------------------------

## Real-world Applications

-   Web servers
-   Chat servers
-   Database servers
-   Network services

------------------------------------------------------------------------

End of Summary
