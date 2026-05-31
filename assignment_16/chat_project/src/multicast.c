#define _DEFAULT_SOURCE
#include "./../include/multicast.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "./../include/limitation.h"

int MulticastCreateReceiverSocket(const char *_multicastIp, int _port)
{
    int sock;
    int reuse = 1;
    struct sockaddr_in localAddr;
    struct ip_mreq multicastRequest;

    if (_multicastIp == NULL || _port <= 0)
    {
        return -1;
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("socket failed");
        return -1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        perror("setsockopt SO_REUSEADDR failed");
        close(sock);
        return -1;
    }

    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port = htons(_port);

    if (bind(sock, (struct sockaddr *)&localAddr, sizeof(localAddr)) < 0)
    {
        perror("bind failed");
        close(sock);
        return -1;
    }

    multicastRequest.imr_multiaddr.s_addr = inet_addr(_multicastIp);
    multicastRequest.imr_interface.s_addr = INADDR_ANY;

    if (setsockopt(sock,
                   IPPROTO_IP,
                   IP_ADD_MEMBERSHIP,
                   &multicastRequest,
                   sizeof(multicastRequest)) < 0)
    {
        perror("setsockopt IP_ADD_MEMBERSHIP failed");
        close(sock);
        return -1;
    }

    return sock;
}

int MulticastCreateSenderSocket(void)
{
    int sock;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("socket failed");
        return -1;
    }

    return sock;
}

int MulticastSendMessage(int _sock,
                         const char *_multicastIp,
                         int _port,
                         const char *_message)
{
    struct sockaddr_in groupAddr;

    if (_sock < 0 || _multicastIp == NULL || _message == NULL || _port <= 0)
    {
        return -1;
    }

    memset(&groupAddr, 0, sizeof(groupAddr));
    groupAddr.sin_family = AF_INET;
    groupAddr.sin_addr.s_addr = inet_addr(_multicastIp);
    groupAddr.sin_port = htons(_port);

    return sendto(_sock,
                  _message,
                  strlen(_message),
                  0,
                  (struct sockaddr *)&groupAddr,
                  sizeof(groupAddr));
}

int MulticastReceiveMessage(int _sock, char *_buffer, size_t _bufferSize)
{
    ssize_t bytesReceived;

    if (_sock < 0 || _buffer == NULL || _bufferSize == 0)
    {
        return -1;
    }

    bytesReceived = recvfrom(_sock, _buffer, _bufferSize - 1, 0, NULL, NULL);

    if (bytesReceived < 0)
    {
        return -1;
    }

    _buffer[bytesReceived] = '\0';

    return (int)bytesReceived;
}

void RunMulticastReceiver(const char *_multicastIp, int _port)
{
    int sock;
    char buffer[BUFFER_SIZE];

    printf("Receiver PID=%d started\n", getpid());

    sock = MulticastCreateReceiverSocket(_multicastIp, _port);
    if (sock < 0)
    {
        return;
    }

    printf("Receiver started on %s:%d\n", _multicastIp, _port);

    while (1)
    {
        if (MulticastReceiveMessage(sock, buffer, sizeof(buffer)) > 0)
        {
            printf("[GROUP] %s\n", buffer);
            fflush(stdout);
        }
    }
}

void RunMulticastSender(const char *_multicastIp,
                        int _port,
                        const char *_username)
{
    int sock;
    char message[BUFFER_SIZE];
    char fullMessage[BUFFER_SIZE + MAX_USERNAME_SIZE + 4];

    sock = MulticastCreateSenderSocket();
    if (sock < 0)
    {
        return;
    }

    printf("Sender started for %s:%d as %s\n",_multicastIp,_port,_username);

    while (1)
    {
        if (fgets(message, sizeof(message), stdin) == NULL)
        {
            break;
        }

        snprintf(fullMessage, sizeof(fullMessage), "%s: %s", _username, message);

        MulticastSendMessage(sock, _multicastIp, _port, fullMessage);
    }
}