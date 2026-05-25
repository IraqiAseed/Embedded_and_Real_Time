// udp_server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 1234
#define BUFFER_SIZE 4096

int InitServer(void);
void ReceiveMessage(int _sock,
                    char *_buffer,
                    struct sockaddr_in *_clientAddr,
                    socklen_t *_clientLen);

void SendMessage(int _sock,
                 const char *_message,
                 struct sockaddr_in *_clientAddr,
                 socklen_t _clientLen);

int InitServer(void)
{
    int sock;

    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0)
    {
        perror("socket failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(sock,
             (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0)
    {
        perror("bind failed");

        close(sock);

        exit(1);
    }

    return sock;
}

void ReceiveMessage(int _sock,
                    char *_buffer,
                    struct sockaddr_in *_clientAddr,
                    socklen_t *_clientLen)
{
    int read_bytes;

    read_bytes = recvfrom(_sock,
                          _buffer,
                          BUFFER_SIZE - 1,
                          0,
                          (struct sockaddr *)_clientAddr,
                          _clientLen);

    if (read_bytes < 0)
    {
        perror("recvfrom failed");

        exit(1);
    }

    _buffer[read_bytes] = '\0';

    printf("Received from %s:%d -> %s\n",
           inet_ntoa(_clientAddr->sin_addr),
           ntohs(_clientAddr->sin_port),
           _buffer);
}

void SendMessage(int _sock,
                 const char *_message,
                 struct sockaddr_in *_clientAddr,
                 socklen_t _clientLen)
{
    int sent_bytes;

    sent_bytes = sendto(_sock,
                        _message,
                        strlen(_message),
                        0,
                        (struct sockaddr *)_clientAddr,
                        _clientLen);

    if (sent_bytes < 0)
    {
        perror("sendto failed");

        exit(1);
    }
}

int main(void)
{
    int sock;

    char buffer[BUFFER_SIZE];

    struct sockaddr_in client_addr;

    socklen_t client_len;

    sock = InitServer();

    printf("UDP server listening on port %d\n", SERVER_PORT);

    while (1)
    {
        client_len = sizeof(client_addr);

        ReceiveMessage(sock,
                       buffer,
                       &client_addr,
                       &client_len);

        SendMessage(sock,
                    "Aseed Server - OK: message received",
                    &client_addr,
                    client_len);
    }

    close(sock);

    return 0;
}