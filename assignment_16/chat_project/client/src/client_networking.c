#include "client_networking.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stddef.h>

int ConnectToServer(const char *ip, int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket failed");
        return -1;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &serverAddr.sin_addr) <= 0)
    {
        perror("inet_pton failed");
        close(sock);
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("connect failed");
        close(sock);
        return -1;
    }

    return sock;
}
int RecvAll(int sock, void *buffer, size_t size)
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
            return 0; /* server disconnected */
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

    return 2 + _packet->length;
}
