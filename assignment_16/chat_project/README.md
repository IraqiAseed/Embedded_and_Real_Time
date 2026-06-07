# Chat Project

## Overview

Multi-client chat application implemented in C using TCP and UDP Multicast.

The server manages:

* User registration
* User login/logout
* Group creation
* Group joining/leaving

Each chat group is assigned a unique multicast address and multicast port.

Clients communicate with the server using TCP and communicate with group members using UDP Multicast.

---

## Features

### User Management

* Register new users
* Login existing users
* Logout users

### Group Management

* Create groups
* Join groups
* Leave groups

### Chat

* UDP Multicast based group chat
* Multiple chat groups
* Separate sender and receiver processes
* Automatic terminal creation for chat sessions

---

## Architecture

### Client

Responsible for:

* User interaction
* TCP communication with server
* Managing active chat sessions
* Launching multicast sender/receiver processes

### Server

Responsible for:

* User authentication
* Group management
* Client request processing

### Managers

* UserManager
* GroupManager

### Common

Shared modules:

* Protocol
* Multicast
* IPC

---

## Folder Structure

chat_project/

├── client/
│   ├── include/
│   └── src/
│
├── server/
│   ├── include/
│   └── src/
│
├── common/
│   ├── include/
│   └── src/
│
├── managers/
│   ├── include/
│   └── src/
│
├── external/
├── build/
├── bin/
└── makefile

---

## Protocol

TCP packets use a TLV-style format:

* Tag
* Length
* Value

Used for:

* Registration
* Login
* Logout
* Create Group
* Join Group
* Leave Group

---

## Build

```bash
make
```

---

## Run Server

```bash
make run-server
```

---

## Run Client

```bash
make run-client
```

---

## Technologies

* C
* TCP Sockets
* UDP Multicast
* POSIX APIs
* Message Queues
* HashMap
* Linux

---

## Future Improvements TODO

* Persistent user database
* Private messaging
* Chat history
* Threaded server
* Event-driven server using poll/epoll
* GUI client
