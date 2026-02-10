gcc server_udp.c -o server_udp

gcc client_udp.c -o client_udp



gcc server_broken_ftp.c -o server_broken_ftp

gcc client_broken_ftp.c -o client_broken_ftp


# Lab Sheet 4 – Program Interaction & Core Network Concepts

This document explains:
- Which programs work together
- How they communicate
- Core Computer Network concepts demonstrated by each program
- Simple layman explanations for easy understanding

---

## Program Sets (Which Files Work Together)

### Set 1: UDP Echo Application
Files that work together:
- `server_udp.c`
- `client_udp.c`

➡️ Server is started first, then the client.

---

### Set 2: Broken FTP Application (TCP)
Files that work together:
- `server_broken_ftp.c`
- `client_broken_ftp.c`

➡️ Server is started first, then the client.

---

## Core Network Concepts Demonstrated

---

## 1. Client–Server Architecture

**Concept:**
- One machine acts as a **server** (provides service).
- Another acts as a **client** (requests service).

**In This Lab:**
- `server_udp.c` and `server_broken_ftp.c` act as servers.
- `client_udp.c` and `client_broken_ftp.c` act as clients.

**Layman Explanation:**
> Like a restaurant: the waiter (server) responds only when a customer (client) places an order.

---

## 2. Socket Programming

**Concept:**
- A **socket** is an endpoint for communication between two programs over a network.
- Identified by **IP address + port number**.

**In This Lab:**
- Port `8888` → UDP Echo
- Port `5001` → Broken FTP

**Layman Explanation:**
> A socket is like a phone number that tells where to send or receive messages.

---

## 3. UDP (User Datagram Protocol)

**Concept:**
- Connectionless protocol.
- No guarantee of delivery, order, or duplication.
- Faster and lightweight.

**In This Lab:**
- Used by `client_udp.c` and `server_udp.c`.
- Uses `sendto()` and `recvfrom()`.

**Layman Explanation:**
> Like sending postcards — fast, but you don’t know if or when they will arrive.

---

## 4. TCP (Transmission Control Protocol)

**Concept:**
- Connection-oriented protocol.
- Guarantees reliable, ordered, and error-free delivery.
- Uses acknowledgments and retransmissions.

**In This Lab:**
- Used by Broken FTP programs.
- Uses `connect()`, `accept()`, `read()`, and `write()`.

**Layman Explanation:**
> Like a phone call — connection is set up, data is exchanged reliably, then the call ends.

---

## 5. Ports and Multiplexing

**Concept:**
- Ports allow multiple applications to run on the same machine.
- Each service listens on a unique port.

**In This Lab:**
- UDP Echo → Port `8888`
- Broken FTP → Port `5001`

**Layman Explanation:**
> Same house, different doors — each door leads to a different service.

---

## 6. Blocking I/O

**Concept:**
- A blocking call waits until data is available.
- Program execution pauses until the operation completes.

**In This Lab:**
- `recvfrom()` and `read()` block until data arrives.

**Layman Explanation:**
> Like waiting at a bus stop until the bus actually comes.

---

## 7. Reliable vs Unreliable Communication

**Concept:**
- Reliable: data is guaranteed to arrive correctly (TCP).
- Unreliable: data may be lost (UDP).

**In This Lab:**
- UDP Echo → unreliable but fast.
- Broken FTP → reliable file transfer.

**Layman Explanation:**
> UDP is like shouting across a room; TCP is like sending a registered courier.

---

## 8. File Transfer Using Streams

**Concept:**
- Files are transferred as a stream of bytes.
- Large files are sent in smaller chunks.

**In This Lab:**
- File data sent in 256-byte chunks.
- Prevents memory overload.

**Layman Explanation:**
> Instead of sending an entire book at once, pages are sent one by one.

---

## 9. Broken Download & Offset Concept

**Concept:**
- Offset allows resuming file transfer from a specific byte.
- Avoids re-downloading already received data.

**In This Lab:**
- Client sends offset value.
- Server starts sending file from that position.

**Layman Explanation:**
> If a download stops halfway, you continue from where it stopped instead of starting again.

---

## 10. Data Serialization (Basic)

**Concept:**
- Data like commands and offsets are converted into strings before transmission.

**In This Lab:**
- `sprintf()` used to send command and offset.

**Layman Explanation:**
> Numbers are converted into text so both sides understand the message.

---

## Overall Summary

- UDP programs demonstrate **connectionless, fast communication**.
- TCP programs demonstrate **reliable, resumable file transfer**.
- Together, these programs clearly show the **core differences between UDP and TCP** and how real network applications are built.

