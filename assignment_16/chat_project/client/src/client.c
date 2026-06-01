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
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include "./../include/multicast.h"
#include "./../include/chat_ipc.h"
#include <stdlib.h>

#include "client.h"
#include "client_chat.h"
#include "client_manager.h"
#include "protocol.h"

 char g_loggedInUsername[MAX_USERNAME_SIZE + 1];

 char g_pendingGroupName[MAX_GROUP_NAME_SIZE + 1];


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


static int PrintMainMenuAndGetChoice(void)
{
    int choice = 0;

    printf("\n===== Chat Project =====\n");
    printf("1. Registration\n");
    printf("2. Login\n");
    printf("3. Exit\n");
    printf("Choose option: ");

    if (scanf("%d", &choice) != 1)
    {
        while (getchar() != '\n')
        {
        }
        return -1;
    }

    while (getchar() != '\n')
    {
    }

    return choice;
}
static int PrintGroupMenuAndGetChoice(void)
{
    int choice = 0;

    printf("\n===== Groups Menu =====\n");
    printf("1. Create Group\n");
    printf("2. Join Group\n");
    printf("3. Leave Group\n");
    printf("4. Logout\n");
    printf("Choose option: ");

    if (scanf("%d", &choice) != 1)
    {
        while (getchar() != '\n')
        {
        }
        return -1;
    }

    while (getchar() != '\n')
    {
    }

    return choice;
}

static void GroupMenuLoop(int _socket)
{
    int choice;

    while (1)
    {
        choice = PrintGroupMenuAndGetChoice();

        if (choice == 1)
        {
            printf("Create Group selected\n");
            TcpPacket packet;
            if (StartCreateGroupRequest(&packet) == 0)
            {
                if (ProcessClientRequest(_socket, &packet) < 0)
                {
                    printf("Failed to create group request\n");
                }
            }
        }
        else if (choice == 2)
        {
            printf("Join Group selected\n");
            TcpPacket packet;
            if (StartJoinGroupRequest(&packet) == 0)
            {
                if (ProcessClientRequest(_socket, &packet) < 0)
                {
                    printf("Failed to join group request\n");
                }
            }
        }
        else if (choice == 3)
        {
            printf("Leave Group selected\n");
            TcpPacket packet;
            if (StartLeaveGroupRequest(&packet) == 0)
            {
                if (ProcessClientRequest(_socket, &packet) < 0)
                {
                    printf("Failed to leave group request\n");
                }
            }
        }
        else if (choice == 4)
        {
            TcpPacket packet;

            printf("Logout selected\n");

            if (BuildLogoutRequest(&packet) < 0)
            {
                printf("Failed to build logout request\n");
                continue;
            }

            if (ProcessClientRequest(_socket, &packet) < 0)
            {
                printf("Failed to logout\n");
                continue;
            }

            break;
        }
        else
        {
            printf("Invalid option\n");
        }
    }
}



static int MainLoopSelection(int _sock)
{
    int choice;

    while (1)
    {
        choice = PrintMainMenuAndGetChoice();

        if (choice == 1)
        {
            TcpPacket packet;

            if (StartRegisterRequest(&packet) == 0)
            {
                if (ProcessClientRequest(_sock, &packet) < 0)
                {
                    printf("Failed to handle client request\n");
                }
            }
        }
        else if (choice == 2)
        {
            TcpPacket packet;

            if (StartLoginRequest(&packet) == 0)
            {
                int result = ProcessClientRequest(_sock, &packet);

                if (result == 1)
                {
                    GroupMenuLoop(_sock);
                }
                else if (result < 0)
                {
                    printf("Failed to handle client request\n");
                }
            }
        }
        else if (choice == 3)
        {
            TcpPacket packet;

            printf("Exit selected\n");

            if (BuildExitRequest(&packet) < 0)
            {
                printf("Failed to build exit request\n");
                break;
            }

            if (ProcessClientRequest(_sock, &packet) < 0)
            {
                printf("Failed to send exit request\n");
            }

            break;
        }
        else
        {
            printf("Invalid option\n");
        }
    }

    return choice;
}

int main(int argc, char *argv[])
{

    if (argc == 5 && strcmp(argv[1], "receiver") == 0)
    {
        int port = atoi(argv[3]);
        int queueId = atoi(argv[4]);

        SendPid(queueId, 1, getpid());
        RunMulticastReceiver(argv[2], port);
        return 0;
    }

    if (argc == 6 && strcmp(argv[1], "sender") == 0)
    {
        int port = atoi(argv[3]);
        int queueId = atoi(argv[4]);

        SendPid(queueId, 2, getpid());
        RunMulticastSender(argv[2], port, argv[5]);
        return 0;
    }

    int sock = ConnectToServer(SERVER_IP, SERVER_PORT);

    if (sock < 0)
    {
        return 1;
    }

    printf("Connected to server.\n");

    MainLoopSelection(sock);

    close(sock);
    return 0;
}
