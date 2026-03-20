# Computer Networks Lab 7

## Transport Layer Protocols using Wireshark (TCP & UDP)

---

## 🎯 Objective

* Understand TCP and UDP behavior using Wireshark
* Analyze sequence numbers, ACKs, RTT, throughput
* Observe congestion control (slow start & avoidance)
* Study UDP header structure

---

# 🚀 Overall Flow

1. Exercise 1 → Filters + basic TCP
2. Exercise 2 → Capture file upload
3. Exercise 3 → TCP analysis (seq, ACK, RTT)
4. Exercise 4 → Congestion control graph
5. Exercise 5 → UDP analysis

---

# 🔸 Exercise 1: Filters + Basic TCP

## Steps

* Start Wireshark capture
* Open browser → search on Google → click links/images
* Stop capture

## Filters to Try

```
tcp.port == 80
tcp.port == 80 || tcp.port == 443
ip.addr == <your IP>
ip.len == 1500
```

## Key Analysis

* TCP Handshake → SYN, SYN-ACK, ACK
* Reset connections → `tcp.flags.reset == 1`
* Header > 20 bytes → TCP options present
* Duplicate ACKs → `tcp.analysis.duplicate_ack`
* Out-of-order → `tcp.analysis.out_of_order`

---

# 🔸 Exercise 2: Bulk TCP Transfer

## Steps

* Download file (alice.txt)
* Open upload page
* Start Wireshark capture
* Upload file
* Stop capture

## Logic

* Generates large TCP traffic (~150KB)
* Helps observe real TCP behavior

---

# 🔸 Exercise 3: TCP Deep Analysis

## Basic Info

* Client IP → your machine
* Server IP → gaia.cs.umass.edu
* Port → usually 80

## Important Filters

```
tcp
tcp.flags.syn == 1 && tcp.flags.ack == 0
tcp.flags.syn == 1 && tcp.flags.ack == 1
tcp contains "POST"
tcp.analysis.retransmission
```

## Key Concepts

* Sequence Number → identifies data order
* ACK Number → confirms received data
* RTT = ACK time – send time
* Estimated RTT = smoothed average
* Segment length → `tcp.len`
* Receiver buffer → `tcp.window_size`

## Observations

* Retransmissions if loss occurs
* Cumulative ACK behavior
* Throughput = Total Data / Time

---

# 🔸 Exercise 4: Congestion Control

## Steps

* Select TCP packet
* Go to:

```
Statistics → TCP Stream Graph → Time-Sequence Graph (Stevens)
```

## Interpretation

* Slow Start → exponential growth
* Congestion Avoidance → linear growth

---

# 🔸 Exercise 5: UDP Analysis

## Steps

* Generate UDP traffic (YouTube / DNS)
* Apply filter:

```
udp
```

## UDP Header Fields

* Source Port (2 bytes)
* Destination Port (2 bytes)
* Length (2 bytes)
* Checksum (2 bytes)

## Key Points

* Length = header + data
* Max payload = 65527 bytes
* Max port = 65535
* Protocol number = 17 (0x11)

## Request-Response Behavior

* Ports swap between sender and receiver

---

# 🧠 Key Learnings

| Concept            | Insight                         |
| ------------------ | ------------------------------- |
| TCP                | Reliable, ordered communication |
| UDP                | Fast, unreliable                |
| Seq/ACK            | Ensure correct delivery         |
| RTT                | Measures delay                  |
| Throughput         | Measures performance            |
| Congestion Control | Adapts sending rate             |

---

# 🎯 Viva Summary

* TCP provides reliable data transfer using sequence numbers, ACKs, and congestion control
* UDP is connectionless and faster but does not guarantee delivery

---

# ✅ Conclusion

Using Wireshark, we analyzed real TCP and UDP traffic and understood how reliability, flow control, and congestion control work in transport layer protocols.
