# CS F303 – Computer Networks  
## Lab Sheet 2: FTP and DNS using Wireshark

---

## Objectives
- Learn how **FTP (File Transfer Protocol)** works
- Understand how **DNS (Domain Name System)** resolves domain names
- Use **Wireshark** to observe real network packets
- Practice networking tools like `ftp` and `nslookup`

---

## Lab Structure
This lab has **three main parts**:
1. Studying **FTP** using Wireshark  
2. Studying **DNS** using Wireshark and `nslookup`  
3. Learning the importance of **traceroute** (for network troubleshooting)

---

## Part 1: File Transfer Protocol (FTP)

### What is FTP?
- FTP is used to **transfer files** between a client and a server.
- You must **log in** using a username and password.
- FTP uses **two connections**:
  - **Control connection** → for commands
  - **Data connection** → for sending files or directory lists

---

### FTP Commands
Some common FTP commands are:
- `USER`, `PASS` → login
- `LIST` → show files
- `RETR` → download file
- `STOR` → upload file
- `PORT` → tells server which port to use
- `QUIT` → end FTP session

---

### FTP Working
- Control connection stays active throughout the session.
- Data connection is created **only when needed**.
- Client selects a temporary port and informs server using `PORT`.
- Server always uses **port 20** for data transfer.

---

## FTP Lab Exercises

### Exercise 1: FTP Session
- Start FTP from command prompt
- Connect to `ftp.bits-pilani.ac.in`
- Upload a file
- Exit FTP

---

### Exercise 2: FTP in Wireshark
- Capture FTP packets in Wireshark
- Apply TCP and IP filters
- Observe:
  - TCP **three-way handshake**
  - Creation and closing of TCP connections
  - Reliable data transfer using TCP

---

### Exercise 3: FTP Analysis
- Observe `PORT` and `OPTS` commands
- Understand how server connects back to client
- Verify that directory listing is sent using **data connection**

---

## Part 2: Domain Name System (DNS)

### What is DNS?
- DNS converts **website names** into **IP addresses**
- Example: `google.com → 216.58.199.174`
- DNS is **distributed and hierarchical**

---

### DNS Record Types
- `A / AAAA` → IP address
- `NS` → name servers
- `MX` → mail servers
- `CNAME` → alias
- `PTR` → reverse lookup
- `SOA` → authority info

---

## DNS Experiments

### DNS Packet Capture
- Clear browser history and DNS cache
- Use Wireshark filter: `dns`
- Open a website and observe DNS packets
- Learn that DNS cache exists locally

Commands used:
- `ipconfig /displaydns`
- `ipconfig /flushdns`

---

### DNS Questions Answered
- DNS uses **UDP** (mostly)
- Destination port is **53**
- Query and response have the **same transaction ID**
- DNS resolution happens **before HTTP**

---

## DNS using nslookup

### Exercise 4
Commands:
- `nslookup bits-pilani.ac.in`
- `nslookup -type=NS bits-pilani.ac.in`

Observe:
- DNS server IP
- Source and destination ports
- Query and response messages

---

### Exercise 5: Understanding nslookup Output
- Learn difference between:
  - Authoritative and non-authoritative responses
  - A, NS, MX, PTR records
- Understand reverse DNS lookup (IP → domain)

---

## Key Takeaways
- FTP uses **two TCP connections**
- Wireshark helps analyze real network traffic
- DNS resolves names **before web pages load**
- DNS caching improves performance
- `nslookup` is useful for DNS debugging

---
