#ifndef SERVER_NETWORKING_H_
#define SERVER_NETWORKING_H_

#include <sys/select.h>
#include <stddef.h>
#include "protocol.h"

int CreateSocket(void);
int BindSocket(int sock, int port);
int StartListen(int sock, int backlog);

int SetNonBlocking(int sock);

int AcceptNewClient(int listenerSock);

int RecvTcpPacket(int sock, TcpPacket *_packet);
int SendTcpPacket(int sock, const TcpPacket *_packet);

void CloseAllSockets(fd_set *masterSet, int maxFd);

#endif // SERVER_NETWORKING_H_