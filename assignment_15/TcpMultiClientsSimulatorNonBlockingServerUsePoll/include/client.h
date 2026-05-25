#ifndef CLIENT_H_
#define CLIENT_H_



#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include<stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

typedef struct clientList clientList;

int ConnectToServer(const char *ip, int port);
int SendReceive(int sock);
clientList* CreateClientList(size_t _capacity);
int InsertClient(clientList *_clientList, int _socket);
int RemoveClient(clientList *_clientList, size_t index);
int FillClientList(clientList *list);
void ProcessClient(clientList *list, size_t *currentIndex);
void DestroyClientList(clientList** _clientList);





#endif // CLIENT_H_