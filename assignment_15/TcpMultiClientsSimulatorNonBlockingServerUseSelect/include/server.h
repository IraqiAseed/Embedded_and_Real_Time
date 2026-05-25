#ifndef SERVER_H_
#define SERVER_H_

#include <sys/select.h> // select fd_set FD_SET ...

int CreateSocket();
int BindSocket(int sock, int port);
int StartListen(int sock, int backlog);
int AcceptNewClient(int listenerSock);
int ProcessClient(int clientSock, fd_set *masterSet);
int UpdateMaxFd(fd_set *masterSet, int currentMaxFd);
void CloseAllSockets(fd_set *masterSet, int maxFd);
int SetNonBlocking(int sock);



#endif // SERVER_H_