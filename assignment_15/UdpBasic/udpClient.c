// udp_client.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 1234
#define BUFFER_SIZE 4096

int InitClient(void);

void SendMessage(int _sock,
                 const char *_message,
                 struct sockaddr_in *_serverAddr);

void ReceiveMessage(int _sock,
                    char *_buffer);

int InitClient(void)
{
    int sock;

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0)
    {
        perror("socket failed");

        exit(1);
    }

    return sock;
}

void SendMessage(int _sock,
                 const char *_message,
                 struct sockaddr_in *_serverAddr)
{
    int sent_bytes;

    sent_bytes = sendto(_sock,
                        _message,
                        strlen(_message),
                        0,
                        (struct sockaddr *)_serverAddr,
                        sizeof(*_serverAddr));

    if (sent_bytes < 0)
    {
        perror("sendto failed");

        exit(1);
    }
}

void ReceiveMessage(int _sock,
                    char *_buffer)
{
    int read_bytes;

    struct sockaddr_in server_addr;

    socklen_t server_len = sizeof(server_addr);

    read_bytes = recvfrom(_sock,
                          _buffer,
                          BUFFER_SIZE - 1,
                          0,
                          (struct sockaddr *)&server_addr,
                          &server_len);

    if (read_bytes < 0)
    {
        perror("recvfrom failed");

        exit(1);
    }

    _buffer[read_bytes] = '\0';

    printf("Server replied: %s\n", _buffer);
}

int main(void)
{
    int sock;

    char buffer[BUFFER_SIZE];

    struct sockaddr_in server_addr;

    sock = InitClient();

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    inet_pton(AF_INET,
              SERVER_IP,
              &server_addr.sin_addr);

    while (1)
    {
        printf("Enter message: ");

        fgets(buffer, BUFFER_SIZE, stdin);

        buffer[strcspn(buffer, "\n")] = '\0';

        SendMessage(sock,
                    buffer,
                    &server_addr);

        ReceiveMessage(sock,
                       buffer);
    }

    close(sock);

    return 0;
}