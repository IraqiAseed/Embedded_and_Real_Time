#ifndef CLIENT_NETWORKING_H_
#define CLIENT_NETWORKING_H_

#include "protocol.h"

int ConnectToServer(const char *ip, int port);
int RecvAll(int sock, void *buffer, size_t length);
int RecvTcpPacket(int sock, TcpPacket *_packet);

#endif