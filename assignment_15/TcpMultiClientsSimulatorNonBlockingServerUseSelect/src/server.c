#include <stdio.h>
#include <string.h>
#include <stdlib.h> // exit
#include <sys/types.h>
#include <sys/socket.h> // socket bind listen accept
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h>  // htons inet_ntoa

#include "./../include/server.h"

#define SERVER_PORT 8080
#define BACKLOG 10
#define MAX_CLIENTS 100
#define MAX_FDS (MAX_CLIENTS + 1)
#define BUFFER_SIZE 1024

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

int ProcessClient(int clientSock, fd_set *masterSet)
{
    char buffer[BUFFER_SIZE];

    ssize_t bytesRead = recv(clientSock,
                             buffer,
                             sizeof(buffer) - 1,
                             0);

    if (bytesRead < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return 1; /* not an error */
        }

        perror("recv failed");

        close(clientSock);
        FD_CLR(clientSock, masterSet);

        return -1;
    }

    if (bytesRead == 0)
    {
        printf("Client disconnected fd=%d\n", clientSock);

        close(clientSock);
        FD_CLR(clientSock, masterSet);

        return 0;
    }

    buffer[bytesRead] = '\0';

    printf("Received from fd=%d: %s\n", clientSock, buffer);

    ssize_t bytesSent = send(clientSock, buffer, bytesRead, 0);

    if (bytesSent < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return 1; /* send buffer full, ignore for now */
        }

        perror("send failed");

        close(clientSock);
        FD_CLR(clientSock, masterSet);

        return -1;
    }

    return 1;
}
int UpdateMaxFd(fd_set *masterSet, int currentMaxFd)
{
    int fd;

    for (fd = currentMaxFd; fd >= 0; fd--)
    {
        if (FD_ISSET(fd, masterSet))
        {
            return fd;
        }
    }

    return -1;
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

int main(void)
{

    fd_set masterSet;
    fd_set tempSet;

    int listenerSock = CreateSocket(); // listner socket

    BindSocket(listenerSock, SERVER_PORT);

    StartListen(listenerSock, BACKLOG);

    FD_ZERO(&masterSet);
    FD_ZERO(&tempSet);

    FD_SET(listenerSock, &masterSet);

    int maxFd = listenerSock;

    while (1)
    {
        tempSet = masterSet;

        int activity = select(maxFd + 1,
                              &tempSet,
                              NULL,
                              NULL,
                              NULL);

        if (activity < 0)
        {
            perror("select failed");
            break;
        }

        for (int fd = 0; fd <= maxFd; fd++)
        {
            if (FD_ISSET(fd, &tempSet))
            {
                if (fd == listenerSock) /* new connection */
                {
                    int clientSock = AcceptNewClient(listenerSock);

                    if (clientSock >= 0)
                    {
                        FD_SET(clientSock, &masterSet);

                        if (clientSock > maxFd)
                        {
                            maxFd = clientSock;
                        }
                    }
                }
                else /* existing client sent data */
                {
                    int result = ProcessClient(fd, &masterSet);

                    if (result <= 0 && fd == maxFd)
                    {
                        maxFd = UpdateMaxFd(&masterSet, maxFd);
                    }
                }
            }
        }
    }
    CloseAllSockets(&masterSet, maxFd);
}