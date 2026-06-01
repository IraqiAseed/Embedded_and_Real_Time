
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h>  // htons inet_ntoa
#include <sys/socket.h> // socket bind listen accept
#include <fcntl.h>
#include <errno.h>
#include "server_networking.h"

int CreateSocket()
{
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0)
    {
        perror("socket failed");
        return -1;
    }

    int opt = 1;
    if (setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt failed");
        close(serverSock);
        return -1;
    }

    if (SetNonBlocking(serverSock) < 0)
    {
        perror("socket non blocking failed");
        close(serverSock);
        return -1;
    }

    return serverSock;
}

int BindSocket(int sock, int port)
{
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        return -1;
    }

    return 0;
}
int StartListen(int sock, int backlog)
{
    if (listen(sock, backlog) < 0)
    {
        perror("listen");
        return -1;
    }

    return 0;
}

int AcceptNewClient(int listenerSock)
{
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    int clientSock = accept(listenerSock,
                            (struct sockaddr *)&clientAddr,
                            &clientLen);

    if (clientSock < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return -1;
        }

        perror("accept failed");
        return -1;
    }

    /*
    TODO:
    RecvAll + non-blocking sockets is not fully correct.
    Need per-client receive state machine later.
    */

    if (SetNonBlocking(clientSock) < 0)
    {
        close(clientSock);
        return -1;
    }

    printf("Client connected fd=%d ip=%s port=%d\n",
           clientSock,
           inet_ntoa(clientAddr.sin_addr),
           ntohs(clientAddr.sin_port));

    return clientSock;
}
static int RecvAll(int sock, void *buffer, size_t size)
{
    size_t total = 0;
    char *ptr = buffer;

    while (total < size)
    {
        ssize_t bytesRead = recv(sock, ptr + total, size - total, 0);

        if (bytesRead < 0)
        {
            return -1;
        }

        if (bytesRead == 0)
        {
            return 0; /* peer disconnected */
        }

        total += bytesRead;
    }

    return 1;
}
int RecvTcpPacket(int sock, TcpPacket *_packet)
{
    int result;

    if (_packet == NULL)
    {
        return -1;
    }

    result = RecvAll(sock, &_packet->tag, sizeof(_packet->tag));
    if (result <= 0)
    {
        return result;
    }

    result = RecvAll(sock, &_packet->length, sizeof(_packet->length));
    if (result <= 0)
    {
        return result;
    }

    result = RecvAll(sock, _packet->value, _packet->length);
    if (result <= 0)
    {
        return result;
    }

    return sizeof(_packet->tag) + sizeof(_packet->length) + _packet->length;
}

int SendTcpPacket(int _clientSocket, const TcpPacket *_packet)
{

    if (_clientSocket < 0 || _packet == NULL)
    {
        return -1;
    }

    return send(_clientSocket, _packet, GetTcpPacketSize(_packet), 0);
}
void CloseAllSockets(fd_set *masterSet, int maxFd)
{
    for (int fd = 0; fd <= maxFd; fd++)
    {
        if (FD_ISSET(fd, masterSet))
        {
            close(fd);
        }
    }
}
int SetNonBlocking(int sock)
{
    int flags = fcntl(sock, F_GETFL, 0);

    if (flags < 0)
    {
        perror("fcntl F_GETFL");
        return -1;
    }

    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        perror("fcntl F_SETFL");
        return -1;
    }

    return 0;
}
