#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "client.h"
#include "client_networking.h"
#include "client_manager.h"
#include "chat_ipc.h"
#include "multicast.h"



 char g_loggedInUsername[MAX_USERNAME_SIZE + 1];

 char g_pendingGroupName[MAX_GROUP_NAME_SIZE + 1];


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
