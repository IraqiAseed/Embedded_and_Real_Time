#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>

#define SERVER_PORT 8080
#define BACKLOG 10
#define MAX_CLIENTS 100
#define MAX_FDS (MAX_CLIENTS + 1)
#define BUFFER_SIZE 1024

int SetNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0)
    {
        perror("fcntl F_GETFL failed");
        return -1;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        perror("fcntl F_SETFL failed");
        return -1;
    }
    return 0;
}
int CreateServerSocket(int port)
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
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(serverSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("bind failed");
        close(serverSock);
        return -1;
    }
    if (listen(serverSock, BACKLOG) < 0)
    {
        perror("listen failed");
        close(serverSock);
        return -1;
    }
    if (SetNonBlocking(serverSock) < 0)
    {
        close(serverSock);
        return -1;
    }

    return serverSock;
}

void RemoveClient(struct pollfd fds[], nfds_t *nfds, int index)
{
    close(fds[index].fd);

    fds[index] = fds[*nfds - 1];
    (*nfds)--;

    printf("Client removed from slot %d\n", index);
}

int AddClient(struct pollfd fds[], nfds_t *nfds, int clientSock)
{
    if (*nfds >= MAX_FDS)
    {
        return -1;
    }

    fds[*nfds].fd = clientSock;
    fds[*nfds].events = POLLIN;
    fds[*nfds].revents = 0;

    (*nfds)++;

    return 0;
}
void AcceptNewClients(int serverSock, struct pollfd fds[], nfds_t *nfds)
{
    while (1)
    {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);

        int clientSock = accept(serverSock,
                                (struct sockaddr *)&clientAddr,
                                &clientLen);
        if (clientSock < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;
            }

            perror("accept failed");
            break;
        }
        printf("Client connected: %s:%d\n",
               inet_ntoa(clientAddr.sin_addr),
               ntohs(clientAddr.sin_port));

        if (SetNonBlocking(clientSock) < 0)
        {
            close(clientSock);
            continue;
        }

        if (AddClient(fds, nfds, clientSock) < 0)
        {
            printf("Too many clients. Closing new connection.\n");
            close(clientSock);
            continue;
        }
    }
}
int HandleClientMessage(struct pollfd fds[], nfds_t *nfds, int index)
{
    char buffer[BUFFER_SIZE];

    ssize_t bytesReceived = recv(fds[index].fd,
                                 buffer,
                                 sizeof(buffer) - 1,
                                 0);

    if (bytesReceived < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return 0;
        }

        perror("recv failed");
        RemoveClient(fds, nfds, index);
        return 1;
    }
    if (bytesReceived == 0)
    {
        printf("Client disconnected.\n");
        RemoveClient(fds, nfds, index);
        return 1;
    }
    buffer[bytesReceived] = '\0';
    printf("Client says: %s", buffer);
    char reply[BUFFER_SIZE + 64];
    snprintf(reply,
             sizeof(reply),
             "Server reply: %s",
             buffer);

    ssize_t bytesSent = send(fds[index].fd,
                             reply,
                             strlen(reply),
                             0);

    if (bytesSent < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return 0;
        }

        perror("send failed");
        RemoveClient(fds, nfds, index);
        return 1;
    }

    return 0;
}

int main(void)
{
    int serverSock = CreateServerSocket(SERVER_PORT);
    if (serverSock < 0)
    {
        return 1;
    }
    struct pollfd fds[MAX_FDS];
    nfds_t nfds = 1;
    fds[0].fd = serverSock;
    fds[0].events = POLLIN;
    fds[0].revents = 0;

    printf("poll + non-blocking server listening on port %d...\n", SERVER_PORT);
    while (1)
    {
        int ready = poll(fds, nfds, -1);

        if (ready < 0)
        {
            perror("poll failed");
            break;
        }
        if (fds[0].revents & POLLIN)
        {
            AcceptNewClients(serverSock, fds, &nfds);
        }

        for (int i = 1; i < (int)nfds; i++)
        {
            if (fds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
            {
                printf("Client socket error/hangup.\n");
                RemoveClient(fds, &nfds, i);
                i--;
                continue;
            }

            if (fds[i].revents & POLLIN)
            {
                int removed = HandleClientMessage(fds, &nfds, i);
                if (removed)
                {
                    i--;
                }
            }
        }
    }

    for (int i = 0; i < (int)nfds; i++)
    {
        close(fds[i].fd);
    }
    return 0;
}
