#include <stdio.h>
#include <string.h>
#include <stdlib.h> // exit
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "server.h"
#include "server_networking.h"
#include "server_manager.h"
#include "protocol.h"

#define SERVER_PORT 8080






int UpdateMaxFd(fd_set *masterSet, int currentMaxFd)
{
    int fd;

    for (fd = currentMaxFd; fd >= 0; fd--)
    {
        if (FD_ISSET(fd, masterSet))
        {
            return fd;
        }
    }

    return -1;
}

int main(void)
{

    fd_set masterSet;
    fd_set tempSet;
    UserManager *userManager;
    GroupManager *groupManager;

    int listenerSock = CreateSocket(); // listner socket
    if (listenerSock < 0)
    {
        return 1;
    }

    if (BindSocket(listenerSock, SERVER_PORT) < 0)
    {
        close(listenerSock);
        return 1;
    }

    if (StartListen(listenerSock, BACKLOG) < 0)
    {
        close(listenerSock);
        return 1;
    }

    userManager = UserManagerCreate();

    if (userManager == NULL)
    {
        printf("Failed to create UserManager\n");
        close(listenerSock);
        return 1;
    }

    groupManager = GroupManagerCreate();

    if (groupManager == NULL)
    {
        UserManagerDestroy(&userManager);
        close(listenerSock);
        return 1;
    }

    FD_ZERO(&masterSet);
    FD_ZERO(&tempSet);

    FD_SET(listenerSock, &masterSet);

    int maxFd = listenerSock;

    while (1)
    {
        tempSet = masterSet;

        int activity = select(maxFd + 1, &tempSet, NULL, NULL, NULL);

        if (activity < 0)
        {
            perror("select failed");
            break;
        }

        for (int fd = 0; fd <= maxFd; fd++)
        {
            if (FD_ISSET(fd, &tempSet))
            {
                // There is at least one pending connection waiting in the kernel accept queue.
                if (fd == listenerSock)
                {
                    int clientSock = AcceptNewClient(listenerSock); /* new connection */

                    if (clientSock >= 0)
                    {
                        FD_SET(clientSock, &masterSet);

                        if (clientSock > maxFd)
                        {
                            maxFd = clientSock;
                        }
                    }
                }
                else /* existing client sent data */
                {
                    int result = ProcessClient(fd, &masterSet, userManager, groupManager);

                    if (result <= 0 && fd == maxFd)
                    {
                        maxFd = UpdateMaxFd(&masterSet, maxFd);
                    }
                }
            }
        }
    }

    CloseAllSockets(&masterSet, maxFd);
    UserManagerDestroy(&userManager);
    GroupManagerDestroy(&groupManager);
}