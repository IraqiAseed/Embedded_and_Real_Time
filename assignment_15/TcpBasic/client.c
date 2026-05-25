#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main(void)
{
    int sock = -1;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    const char *message = "Hello from client";

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket failed");
        return 1;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0)
    {
        perror("inet_pton failed");
        close(sock);
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("connect failed");
        close(sock);
        return 1;
    }

    printf("Connected to server.\n");

    ssize_t bytesSent = send(sock, message, strlen(message), 0);
    if (bytesSent < 0)
    {
        perror("send failed");
        close(sock);
        return 1;
    }

    ssize_t bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived < 0)
    {
        perror("recv failed");
        close(sock);
        return 1;
    }
    if (bytesReceived == 0)
    {
        printf("Server disconnected.\n");
        close(sock);
        return 0;
    }

    buffer[bytesReceived] = '\0';
    printf("Server replied: %s\n", buffer);
    close(sock);
    return 0;
}
