#ifndef CLIENT_MANAGER_H_
#define CLIENT_MANAGER_H_

#include "protocol.h"

int ProcessClientRequest(int sock, TcpPacket *packet);
int StartCreateGroupRequest(TcpPacket *_packet);
int StartJoinGroupRequest(TcpPacket *_packet);
int StartLeaveGroupRequest(TcpPacket *_packet);
int StartRegisterRequest(TcpPacket *_packet);
int StartLoginRequest(TcpPacket *_packet);


#endif // CLIENT_MANAGER_H_