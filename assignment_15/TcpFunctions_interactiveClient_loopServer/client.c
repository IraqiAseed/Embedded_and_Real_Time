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

int ConnectToServer(const char* ip, int port)
{
   int sock = -1;

   struct sockaddr_in serverAddr;

   sock = socket(AF_INET, SOCK_STREAM, 0);
   if (sock < 0)
   {
       perror("socket failed");
       return -1;
   }

   memset(&serverAddr, 0, sizeof(serverAddr));

   serverAddr.sin_family = AF_INET;
   serverAddr.sin_port = htons(port);

   if (inet_pton(AF_INET, ip,&serverAddr.sin_addr) <= 0)
   {
       perror("inet_pton failed");
       close(sock);
       return -1;
   }

   if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
   {
       perror("connect failed");
       close(sock);
       return -1;
   }

   return sock;
}

int main(void)
{
   int sock = -1;

   char sendBuffer[BUFFER_SIZE];
   char recvBuffer[BUFFER_SIZE];

   sock = ConnectToServer(SERVER_IP, SERVER_PORT);
   if (sock < 0)
   {
       return 1;
   }

   printf("Connected to server.\n");

   while (1)
   {
       printf("Enter message: ");

       if (fgets(sendBuffer,sizeof(sendBuffer),stdin) == NULL)
       {
           break;
       }

       if (strncmp(sendBuffer, "exit", 4) == 0)
       {
           break;
       }

   
    ssize_t bytesSent =
           send(sock,
                sendBuffer,
                strlen(sendBuffer),
                0);

       if (bytesSent < 0)
       {
           perror("send failed");
           break;
       }

       ssize_t bytesReceived =
           recv(sock,
                recvBuffer,
                sizeof(recvBuffer) - 1,
                0);

       if (bytesReceived < 0)
       {
           perror("recv failed");
           break;
       }

       if (bytesReceived == 0)
       {
           printf("Server disconnected.\n");
           break;
       }

       recvBuffer[bytesReceived] = '\0';

       printf("Server reply: %s\n", recvBuffer);
   }

   close(sock);

   return 0;
}
