#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SERVER_PORT 8080
#define BACKLOG 10
#define BUFFER_SIZE 1024

int main(void)
{
    int serverSock = -1;
    int clientSock = -1;
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];
    const char* globalTcpMsg = " Tcp Basic server handle only one message";

    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0)
    {
        perror("socket failed");
        return 1;
    }

    int opt = 1;
    if (setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt failed");
        close(serverSock);
        return 1;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSock, (struct sockaddr *)&serverAddr,
             sizeof(serverAddr)) < 0)
    {
        perror("bind failed");
        close(serverSock);
        return 1;
    }

    if (listen(serverSock, BACKLOG) < 0)
    {
        perror("listen failed");
        close(serverSock);
        return 1;
    }

    printf("Server listening on port %d...\n", SERVER_PORT);

    clientSock = accept(serverSock, (struct sockaddr *)&clientAddr,
                        &clientLen);
    if (clientSock < 0)
    {
        perror("accept failed");
        close(serverSock);
        return 1;
    }

    printf("Client connected: %s:%d\n",
           inet_ntoa(clientAddr.sin_addr),
           ntohs(clientAddr.sin_port));

    ssize_t bytesReceived = recv(clientSock, buffer, sizeof(buffer) - 1, 0);

    if (bytesReceived < 0)
    {
        perror("recv failed");
        close(clientSock);
        close(serverSock);
        return 1;
    }

    if (bytesReceived == 0)
    {
        printf("Client disconnected.\n");
        close(clientSock);
        close(serverSock);
        return 0;
    }

    buffer[bytesReceived] = '\0';
    printf("Received: %s\n", buffer);

    char sendBuffer[2048];

    snprintf(sendBuffer,sizeof(sendBuffer),"%s [ %s ]",globalTcpMsg,buffer);
    ssize_t bytesSent = send(clientSock, sendBuffer, strlen(sendBuffer), 0);
    if (bytesSent < 0)
    {
        perror("send failed");
        close(clientSock);
        close(serverSock);
        return 1;
    }

    printf("Echo sent back to client.\n");

    close(clientSock);
    close(serverSock);

    return 0;
}
