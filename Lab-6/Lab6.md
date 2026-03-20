# Computer Networks Lab 6

## Stop-and-Wait Protocol (rdt 2.5) using UDP

---

## 🎯 Objective

* Understand how reliability is built over unreliable networks
* Implement Stop-and-Wait protocol using UDP
* Handle packet loss using retransmissions and timeouts

---

## 🔹 Core Idea (Big Picture)

UDP is unreliable:

* No guarantee of delivery
* No guarantee of order
* No acknowledgements

👉 So we build our own **reliable protocol (rdt 2.5)** on top of UDP.

---

# 🔸 PART 1: Stop-and-Wait without Packet Loss (EX-1)

## 💡 Logic

1. Sender sends ONE packet
2. Sender waits for ACK
3. Receiver sends ACK after receiving packet
4. Sender sends next packet only after ACK

👉 This ensures **controlled communication**

---

## 🔹 Sequence Numbers

We use:

* `0` and `1` alternately

Why?

* Helps detect duplicates later (important for EX-2)

Flow:

```
Sender → Packet 0 → Receiver
Receiver → ACK 0 → Sender
Sender → Packet 1 → Receiver
Receiver → ACK 1 → Sender
```

---

## 🔹 Sender Logic (Client)

* Take input from user
* Attach sequence number (0 or 1)
* Send packet using UDP
* Wait (blocking) for ACK
* If correct ACK received → move to next sequence

---

## 🔹 Receiver Logic (Server)

* Wait for packet
* Check sequence number
* Print data
* Send ACK with same sequence number
* Toggle expected sequence (0 ↔ 1)

---

## 🧠 Key Insight

Even without loss, we are implementing:

* Stop-and-wait discipline
* Acknowledgement system
* Sequence tracking

👉 This is **foundation of reliability**

---

# 🔸 PART 2: Stop-and-Wait with Packet Loss (EX-2)

Now we simulate real network behavior.

---

## 🔹 1. Data Packet Loss (Receiver Side)

### 💡 Logic

* Receiver randomly drops packets
* If dropped → no ACK sent

### Implementation Idea

```
if(rand()%3==0)
    drop packet
```

👉 Simulates unreliable network

---

## 🔹 2. ACK Packet Loss (Sender Side)

### 💡 Logic

* Sender ignores some ACKs randomly

```
if(rand()%3==0)
    ignore ACK
```

👉 Simulates ACK loss in network

---

## 🔹 3. Timeout Mechanism

### 💡 Problem

* If packet/ACK is lost → sender waits forever

### ✅ Solution

Use `select()` to wait for limited time

```
Wait for ACK for 3 seconds
If not received → timeout
```

---

## 🔹 4. Retransmission

### 💡 Logic

* On timeout → resend SAME packet

Flow:

```
Send packet → wait → timeout → resend
```

---

## 🔹 5. Duplicate Handling (Receiver)

### 💡 Problem

* Sender may resend same packet

### ✅ Solution

* If duplicate packet arrives:

  * Do NOT process data again
  * Resend previous ACK

---

## 🔹 Complete Flow (EX-2)

1. Sender sends packet (seq = 0)
2. Receiver may drop it
3. If dropped → no ACK → timeout
4. Sender retransmits
5. Receiver receives → sends ACK
6. ACK may be lost
7. Sender times out again → retransmits
8. Receiver detects duplicate → resends ACK
9. Sender finally gets ACK → moves to next packet

---

## 🧠 Key Concepts Learned

* Reliable data transfer over unreliable channel
* Stop-and-Wait protocol
* Sequence numbers
* Acknowledgements
* Timeout and retransmission
* Duplicate detection

---

## 🔥 Final Intuition

* UDP = Unreliable base layer
* rdt 2.5 = Reliable protocol built on top

👉 We simulate real network issues and fix them using logic

---

## 🎯 Viva Summary (2 lines)

* EX-1: Implements stop-and-wait with sequence numbers and ACKs (no loss)
* EX-2: Adds packet loss, timeout, and retransmission to ensure reliability

---

## ✅ End Result

You successfully implemented a **basic reliable transport protocol over UDP
