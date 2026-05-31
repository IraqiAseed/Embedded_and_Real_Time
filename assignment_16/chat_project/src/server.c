#include <stdio.h>
#include <string.h>
#include <stdlib.h> // exit
#include <sys/types.h>
#include <sys/socket.h> // socket bind listen accept
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h>  // htons inet_ntoa

#include "./../include/server.h"
#include "./../include/protocol.h"

#define SERVER_PORT 8080
#define BACKLOG 10
#define MAX_CLIENTS 100
#define MAX_FDS (MAX_CLIENTS + 1)
#define BUFFER_SIZE 1024

int CreateSocket()
{
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0)
    {
        perror("socket failed");
        return -1;
    }

    int opt = 1;
    if (setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt failed");
        close(serverSock);
        return -1;
    }

    if (SetNonBlocking(serverSock) < 0)
    {
        perror("socket non blocking failed");
        close(serverSock);
        return -1;
    }

    return serverSock;
}

int BindSocket(int sock, int port)
{
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        return -1;
    }

    return 0;
}
int StartListen(int sock, int backlog)
{
    if (listen(sock, backlog) < 0)
    {
        perror("listen");
        return -1;
    }

    return 0;
}

int AcceptNewClient(int listenerSock)
{
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    int clientSock = accept(listenerSock,
                            (struct sockaddr *)&clientAddr,
                            &clientLen);

    if (clientSock < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return -1;
        }

        perror("accept failed");
        return -1;
    }

    /*
    TODO:
    RecvAll + non-blocking sockets is not fully correct.
    Need per-client receive state machine later.
    */

    if (SetNonBlocking(clientSock) < 0)
    {
        close(clientSock);
        return -1;
    }

    printf("Client connected fd=%d ip=%s port=%d\n",
           clientSock,
           inet_ntoa(clientAddr.sin_addr),
           ntohs(clientAddr.sin_port));

    return clientSock;
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
            return 0; /* peer disconnected */
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

    return sizeof(_packet->tag) + sizeof(_packet->length) + _packet->length;
}

int SendTcpPacket(int _clientSocket, const TcpPacket *_packet)
{

    if (_clientSocket < 0 || _packet == NULL)
    {
        return -1;
    }

    return send(_clientSocket, _packet, GetTcpPacketSize(_packet), 0);
}
static int HandleRegisterRequest(int _clientSocket, const TcpPacket *_packet,
                                 UserManager *_userManager)
{
    int ret = 0;
    if (_packet == NULL || _userManager == NULL)
    {
        return -1;
    }

    if (_packet->tag != TAG_REGISTER_REQUEST)
    {
        return -1;
    }

    char username[MAX_USERNAME_SIZE + 1];
    char password[MAX_PASSWORD_SIZE + 1];

    printf("Packet tag=%u length=%u\n", _packet->tag, _packet->length);

    if (ParseRegisterRequest(_packet, username, password) != 0)
    {
        printf("Failed to parse register request\n");
        return -1;
    }

    printf("Parsed Register Request\n");
    printf("Username: %s\n", username);
    printf("Password: %s\n", password);

    UserManagerResult result = UserManagerRegister(_userManager, username, password);

    TcpPacket responsePacket;

    if (result == USER_MANAGER_SUCCESS)
    {
        BuildRegisterResponse(RESPONSE_STATUS_OK, &responsePacket);
        printf("User registered successfully\n");
    }
    else
    {
        BuildRegisterResponse(RESPONSE_STATUS_FAIL, &responsePacket);
        printf("Failed to register user, result=%d\n", result);
        ret = -1;
    }

    SendTcpPacket(_clientSocket, &responsePacket);
    return ret;
}
static int HandleLoginRequest(int _clientSocket,
                              const TcpPacket *_packet,
                              UserManager *_userManager)
{
    char username[MAX_USERNAME_SIZE + 1];
    char password[MAX_PASSWORD_SIZE + 1];
    UserManagerResult result;
    TcpPacket responsePacket;
    ResponseStatus status;

    if (_packet == NULL || _userManager == NULL)
    {
        return -1;
    }

    if (_packet->tag != TAG_LOGIN_REQUEST)
    {
        return -1;
    }

    if (ParseLoginRequest(_packet, username, password) != 0)
    {
        printf("Failed to parse login request\n");
        return -1;
    }

    result = UserManagerLogin(_userManager,
                              username,
                              password,
                              _clientSocket);

    if (result == USER_MANAGER_SUCCESS)
    {
        status = RESPONSE_STATUS_OK;
        printf("User logged in successfully\n");
    }
    else
    {
        status = RESPONSE_STATUS_FAIL;
        printf("Failed to login user, result=%d\n", result);
    }

    if (BuildLoginResponse(status, &responsePacket) != 0)
    {
        return -1;
    }

    if (SendTcpPacket(_clientSocket, &responsePacket) < 0)
    {
        return -1;
    }

    return status == RESPONSE_STATUS_OK ? 0 : -1;
}
static int HandleLogoutRequest(int _clientSock,
                               const TcpPacket *_packet,
                               UserManager *_userManager,
                               GroupManager *_groupManager)
{
    TcpPacket responsePacket;
    ResponseStatus status;
    char username[MAX_USERNAME_SIZE + 1];

    if (_packet == NULL || _userManager == NULL || _groupManager == NULL)
    {
        return -1;
    }

    if (ParseLogoutRequest(_packet) != 0)
    {
        printf("Failed to parse logout request\n");
        return -1;
    }

    if (UserManagerGetUsernameBySocket(_userManager,
                                       _clientSock,
                                       username) == USER_MANAGER_SUCCESS)
    {
        GroupManagerRemoveUserFromAllGroups(_groupManager,
                                            username);
    }

    if (UserManagerLogout(_userManager, _clientSock) == USER_MANAGER_SUCCESS)
    {
        status = RESPONSE_STATUS_OK;
        printf("User logged out successfully\n");
    }
    else
    {
        status = RESPONSE_STATUS_FAIL;
        printf("Failed to logout user\n");
    }

    if (BuildLogoutResponse(status, &responsePacket) != 0)
    {
        return -1;
    }

    if (SendTcpPacket(_clientSock, &responsePacket) < 0)
    {
        return -1;
    }

    return status == RESPONSE_STATUS_OK ? 0 : -1;
}
int HandleExitRequest(int _clientSocket, const TcpPacket *_packet, UserManager *_userManager)
{
    TcpPacket responsePacket;

    if (_packet == NULL || _userManager == NULL)
    {
        return -1;
    }

    if (ParseExitRequest(_packet) != 0)
    {
        return -1;
    }

    UserManagerLogout(_userManager, _clientSocket);

    if (BuildExitResponse(RESPONSE_STATUS_OK, &responsePacket) != 0)
    {
        return -1;
    }

    if (SendTcpPacket(_clientSocket, &responsePacket) < 0)
    {
        return -1;
    }

    return 0;
}
static int HandleCreateGroupRequest(int clientSock,
                                    const TcpPacket *_packet,
                                    UserManager *_userManager,
                                    GroupManager *_groupManager)
{
    char groupName[MAX_GROUP_NAME_SIZE + 1];
    char username[MAX_USERNAME_SIZE + 1];
    char ip[MAX_MULTICAST_IP_SIZE];
    int port;
    char portStr[8];
    TcpPacket response;
    ResponseStatus status;

    if (_packet == NULL || _userManager == NULL || _groupManager == NULL)
    {
        return -1;
    }

    if (ParseCreateGroupRequest(_packet, groupName) != 0)
    {
        return -1;
    }

    if (UserManagerGetUsernameBySocket(_userManager, clientSock, username) != USER_MANAGER_SUCCESS)
    {
        status = RESPONSE_STATUS_FAIL;
        ip[0] = '\0';
        portStr[0] = '\0';
        printf("Create group failed: client fd=%d is not logged in\n", clientSock);
    }
    else if (GroupManagerCreateGroup(_groupManager, groupName) != GROUP_MANAGER_SUCCESS)
    {
        status = RESPONSE_STATUS_FAIL;
        ip[0] = '\0';
        portStr[0] = '\0';
        printf("Failed to create group: %s\n", groupName);
    }
    else if (GroupManagerAddUserToGroup(_groupManager, groupName, username) != GROUP_MANAGER_SUCCESS)
    {
        status = RESPONSE_STATUS_FAIL;
        ip[0] = '\0';
        portStr[0] = '\0';
        printf("Failed to add creator %s to group %s\n", username, groupName);
    }
    else if (GroupManagerFindGroup(_groupManager, groupName, ip, &port) != GROUP_MANAGER_SUCCESS)
    {
        status = RESPONSE_STATUS_FAIL;
        ip[0] = '\0';
        portStr[0] = '\0';
        printf("Failed to get group info: %s\n", groupName);
    }
    else
    {
        status = RESPONSE_STATUS_OK;
        snprintf(portStr, sizeof(portStr), "%d", port);

        printf("Group created successfully: %s (%s:%d), creator=%s\n",
               groupName, ip, port, username);
    }

    if (BuildCreateGroupResponse(status, ip, portStr, &response) != 0)
    {
        return -1;
    }

    if (SendTcpPacket(clientSock, &response) < 0)
    {
        return -1;
    }

    return status == RESPONSE_STATUS_OK ? 0 : -1;
}
static int HandleJoinGroupRequest(int clientSock,
                                  const TcpPacket *_packet,
                                  UserManager *_userManager,
                                  GroupManager *_groupManager)
{
    char groupName[MAX_GROUP_NAME_SIZE + 1];
    char username[MAX_USERNAME_SIZE + 1];
    char ip[MAX_MULTICAST_IP_SIZE];
    int port;
    char portStr[8];
    TcpPacket response;
    ResponseStatus status = RESPONSE_STATUS_FAIL;

    ip[0] = '\0';
    portStr[0] = '\0';

    if (_packet == NULL || _userManager == NULL || _groupManager == NULL)
    {
        return -1;
    }

    if (ParseJoinGroupRequest(_packet, groupName) != 0)
    {
        printf("Failed to parse join group request\n");
        return -1;
    }

    if (UserManagerGetUsernameBySocket(_userManager,
                                       clientSock,
                                       username) != USER_MANAGER_SUCCESS)
    {
        printf("Join group failed: client fd=%d is not logged in\n", clientSock);
    }
    else if (GroupManagerAddUserToGroup(_groupManager,
                                        groupName,
                                        username) != GROUP_MANAGER_SUCCESS)
    {
        printf("Join group failed: user=%s group=%s\n", username, groupName);
    }
    else if (GroupManagerFindGroup(_groupManager, groupName, ip, &port) != GROUP_MANAGER_SUCCESS)
    {
        printf("Join group failed: cannot get group info: %s\n", groupName);
    }
    else
    {
        status = RESPONSE_STATUS_OK;
        snprintf(portStr, sizeof(portStr), "%d", port);

        printf("User %s joined group successfully: %s (%s:%d)\n",
               username, groupName, ip, port);
    }

    if (BuildJoinGroupResponse(status, ip, portStr, &response) != 0)
    {
        return -1;
    }

    if (SendTcpPacket(clientSock, &response) < 0)
    {
        return -1;
    }

    return status == RESPONSE_STATUS_OK ? 0 : -1;
}
static int HandleLeaveGroupRequest(int clientSock,
                                   const TcpPacket *_packet,
                                   UserManager *_userManager,
                                   GroupManager *_groupManager)
{
    char groupName[MAX_GROUP_NAME_SIZE + 1];
    char username[MAX_USERNAME_SIZE + 1];
    TcpPacket response;
    ResponseStatus status;

    if (_packet == NULL || _userManager == NULL || _groupManager == NULL)
    {
        return -1;
    }

    if (ParseLeaveGroupRequest(_packet, groupName) != 0)
    {
        printf("Failed to parse leave group request\n");
        return -1;
    }

    if (UserManagerGetUsernameBySocket(_userManager,
                                       clientSock,
                                       username) != USER_MANAGER_SUCCESS)
    {
        status = RESPONSE_STATUS_FAIL;
        printf("Leave group failed: client fd=%d is not logged in\n", clientSock);
    }
    else if (GroupManagerRemoveUserFromGroup(_groupManager,
                                             groupName,
                                             username) != GROUP_MANAGER_SUCCESS)
    {
        status = RESPONSE_STATUS_FAIL;
        printf("Leave group failed: user=%s group=%s\n", username, groupName);
    }
    else
    {
        status = RESPONSE_STATUS_OK;
        printf("User %s left group successfully: %s\n", username, groupName);
    }

    if (BuildLeaveGroupResponse(status, &response) != 0)
    {
        return -1;
    }

    if (SendTcpPacket(clientSock, &response) < 0)
    {
        return -1;
    }

    return status == RESPONSE_STATUS_OK ? 0 : -1;
}
static int HandleClientRequest(int _clientSocket,
                               TcpPacket *_packet,
                               UserManager *_userManager,
                               GroupManager *_groupManager)
{
    if (_packet == NULL)
    {
        return -1;
    }

    switch (_packet->tag)
    {
    case TAG_REGISTER_REQUEST:
        printf("Received Register Request\n");
        return HandleRegisterRequest(_clientSocket, _packet, _userManager);

    case TAG_LOGIN_REQUEST:
        printf("Received Login Request\n");
        return HandleLoginRequest(_clientSocket, _packet, _userManager);

    case TAG_LOGOUT_REQUEST:
        printf("Received Logout Request\n");
        return HandleLogoutRequest(_clientSocket, _packet, _userManager, _groupManager);

    case TAG_EXIT_REQUEST:
        printf("Received Exit Request\n");
        return HandleExitRequest(_clientSocket, _packet, _userManager);

    case TAG_CREATE_GROUP_REQUEST:
        printf("Received Create Group Request\n");
        return HandleCreateGroupRequest(_clientSocket, _packet, _userManager, _groupManager);

    case TAG_JOIN_GROUP_REQUEST:
        printf("Received Join Group Request\n");
        return HandleJoinGroupRequest(_clientSocket, _packet, _userManager, _groupManager);

    case TAG_LEAVE_GROUP_REQUEST:
        printf("Received Leave Group Request\n");
        return HandleLeaveGroupRequest(_clientSocket, _packet, _userManager, _groupManager);

    default:
        printf("Received unknown packet with tag=%u\n", _packet->tag);
        return -1;
    }

    return 0;
}

int ProcessClient(int _clientSock, fd_set *_masterSet, UserManager *_userManager, GroupManager *groupManager)
{
    TcpPacket packet;

    ssize_t bytesRead = RecvTcpPacket(_clientSock, &packet);

    if (bytesRead < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return 1;
        }

        perror("recv failed");
        close(_clientSock);
        FD_CLR(_clientSock, _masterSet);
        return -1;
    }

    if (bytesRead == 0)
    {
        printf("Client disconnected fd=%d\n", _clientSock);

        UserManagerLogout(_userManager, _clientSock);

        close(_clientSock);
        FD_CLR(_clientSock, _masterSet);
        return 0;
    }

    printf("Received %zd bytes from fd=%d\n", bytesRead, _clientSock);

    int result = HandleClientRequest(_clientSock, &packet, _userManager, groupManager);

    if (result < 0)
    {
        printf("Failed to handle client request\n");
    }

    if (packet.tag == TAG_EXIT_REQUEST)
    {
        printf("Client exited fd=%d\n", _clientSock);

        UserManagerLogout(_userManager, _clientSock);

        close(_clientSock);
        FD_CLR(_clientSock, _masterSet);

        return 0;
    }

    return 1;
}
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
void CloseAllSockets(fd_set *masterSet, int maxFd)
{
    for (int fd = 0; fd <= maxFd; fd++)
    {
        if (FD_ISSET(fd, masterSet))
        {
            close(fd);
        }
    }
}
int SetNonBlocking(int sock)
{
    int flags = fcntl(sock, F_GETFL, 0);

    if (flags < 0)
    {
        perror("fcntl F_GETFL");
        return -1;
    }

    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        perror("fcntl F_SETFL");
        return -1;
    }

    return 0;
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