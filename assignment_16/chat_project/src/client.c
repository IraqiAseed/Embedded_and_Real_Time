#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "./../include/protocol.h"
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include "./../include/multicast.h"
#include "./../include/chat_ipc.h"
#include <stdlib.h>

#define MAX_ACTIVE_GROUPS 100
static char g_loggedInUsername[MAX_USERNAME_SIZE + 1];
typedef struct ChatSession
{
    char m_groupName[MAX_GROUP_NAME_SIZE + 1];
    char m_ip[MAX_MULTICAST_IP_SIZE];
    int m_port;

    pid_t m_senderPid;
    pid_t m_receiverPid;

} ChatSession;

static ChatSession g_sessions[MAX_ACTIVE_GROUPS];
static size_t g_sessionCount = 0;
static char g_pendingGroupName[MAX_GROUP_NAME_SIZE + 1];

static ChatSession *FindChatSession(const char *_groupName);
static ChatSession *AddChatSession(const char *_groupName, const char *_ip, int _port);
static void RemoveChatSession(const char *_groupName);
static int StartChatSession(const char *_groupName, const char *_ip, int _port);
static int StopChatSession(const char *_groupName);
static void StopAllChatSessions(void);

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

int ProcessClientRequest(int sock, TcpPacket *packet)
{
    size_t packetSize;
    TcpPacket response;
    ResponseStatus status;

    if (sock < 0 || packet == NULL)
    {
        return -1;
    }

    packetSize = 2 + packet->length;

    if (send(sock, packet, packetSize, 0) < 0)
    {
        perror("send failed");
        return -1;
    }

    if (RecvTcpPacket(sock, &response) <= 0)
    {
        printf("Failed to receive server response\n");
        return -1;
    }

    if (response.tag == TAG_REGISTER_RESPONSE)
    {
        if (ParseRegisterResponse(&response, &status) < 0)
        {
            return -1;
        }

        if (status == RESPONSE_STATUS_OK)
        {
            printf("Registration successful, you can now login!\n");
            return 0; // register success
        }
        else
        {
            printf("Registration failed, username may already exist.\n");
            return -1; // register failed
        }
    }

    if (response.tag == TAG_LOGIN_RESPONSE)
    {
        if (ParseLoginResponse(&response, &status) < 0)
        {
            return -1;
        }

        if (status == RESPONSE_STATUS_OK)
        {
            printf("Login successful, welcome to the chat!\n");
            return 1; // login success
        }
        else
        {
            printf("Login failed, please check your username and password.\n");
            return -1; // login failed
        }
    }
    if (response.tag == TAG_LOGOUT_RESPONSE)
    {
        if (ParseLogoutResponse(&response, &status) < 0)
        {
            printf("Invalid logout response\n");
            return -1;
        }

        if (status == RESPONSE_STATUS_OK)
        {
            StopAllChatSessions();
            printf("Logout successful.\n");
            return 0; // logout success
        }
        else
        {
            printf("Failed to logout.\n");
            return -1; // logout failed
        }
    }
    if (response.tag == TAG_CREATE_GROUP_RESPONSE)
    {
        char ip[MAX_MULTICAST_IP_SIZE];
        char port[8];

        if (ParseCreateGroupResponse(&response, &status, ip, port) < 0)
        {
            printf("Invalid create group response\n");
            return -1;
        }

        if (status == RESPONSE_STATUS_OK)
        {
            printf("Group created successfully! Multicast IP: %s, Port: %s\n", ip, port);
            StartChatSession(g_pendingGroupName, ip, atoi(port));
            return 1; // create group success
        }
        else
        {
            printf("Failed to create group. It may already exist.\n");
            return -1; // create group failed
        }
    }
    if (response.tag == TAG_JOIN_GROUP_RESPONSE)
    {
        char ip[MAX_MULTICAST_IP_SIZE];
        char port[8];

        if (ParseJoinGroupResponse(&response, &status, ip, port) < 0)
        {
            printf("Invalid join group response\n");
            return -1;
        }

        if (status == RESPONSE_STATUS_OK)
        {
            printf("Joined group successfully! Multicast IP: %s, Port: %s\n", ip, port);
            StartChatSession(g_pendingGroupName, ip, atoi(port));
            return 1; // join group success
        }
        else
        {
            printf("Failed to join group. It may not exist.\n");
            return -1; // join group failed
        }
    }
    if (response.tag == TAG_LEAVE_GROUP_RESPONSE)
    {
        if (ParseLeaveGroupResponse(&response, &status) < 0)
        {
            printf("Invalid leave group response\n");
            return -1;
        }

        if (status == RESPONSE_STATUS_OK)
        {
            StopChatSession(g_pendingGroupName);
            printf("Left group successfully\n");
            return 0;
        }
        else
        {
            printf("Failed to leave group.\n");
            return -1;
        }
    }

    if (response.tag == TAG_EXIT_RESPONSE)
    {
        if (ParseExitResponse(&response, &status) < 0)
        {
            printf("Invalid exit response\n");
            return -1;
        }

        if (status == RESPONSE_STATUS_OK)
        {
            StopAllChatSessions();
            printf("Exit successful, goodbye!\n");
            return 0;
        }
        else
        {
            printf("Failed to exit properly.\n");
            return -1;
        }
    }

    return -1;
}
static void StopAllChatSessions(void)
{
    while (g_sessionCount > 0)
    {
        StopChatSession(g_sessions[0].m_groupName);
    }
}
static int StartChatSession(const char *_groupName,
                            const char *_ip,
                            int _port)
{
    ChatSession *session;
    int queueId;
    char command[256];

    if (_groupName == NULL || _ip == NULL || _port <= 0)
    {
        return -1;
    }

    if (FindChatSession(_groupName) != NULL)
    {
        printf("Chat session already opened: %s\n", _groupName);
        return 0;
    }

    session = AddChatSession(_groupName, _ip, _port);
    if (session == NULL)
    {
        return -1;
    }

    queueId = CreatePidQueue();
    if (queueId < 0)
    {
        RemoveChatSession(_groupName);
        return -1;
    }

    snprintf(command, sizeof(command),
             "gnome-terminal --title='Receiver-%s' -- ./bin/client.out receiver %s %d %d",
             _groupName,
             _ip,
             _port,
             queueId
             );

    system(command);

    snprintf(command, sizeof(command),
             "gnome-terminal --title='Sender-%s' -- ./bin/client.out sender %s %d %d %s",
             _groupName,
             _ip,
             _port,
             queueId,
             g_loggedInUsername);

    system(command);

    if (ReceivePid(queueId, 1, &session->m_receiverPid) < 0)
    {
        RemoveChatSession(_groupName);
        return -1;
    }

    if (ReceivePid(queueId, 2, &session->m_senderPid) < 0)
    {
        RemoveChatSession(_groupName);
        return -1;
    }

    printf("Chat session started: %s (%s:%d), senderPid=%d receiverPid=%d\n",
           session->m_groupName,
           session->m_ip,
           session->m_port,
           session->m_senderPid,
           session->m_receiverPid);

    return 0;
}
static ChatSession *FindChatSession(const char *_groupName)
{
    size_t i;

    if (_groupName == NULL)
    {
        return NULL;
    }

    for (i = 0; i < g_sessionCount; ++i)
    {
        if (strcmp(g_sessions[i].m_groupName, _groupName) == 0)
        {
            return &g_sessions[i];
        }
    }

    return NULL;
}
static ChatSession *AddChatSession(const char *_groupName, const char *_ip, int _port)
{
    ChatSession *session;

    if (_groupName == NULL || _ip == NULL || g_sessionCount >= MAX_ACTIVE_GROUPS)
    {
        return NULL;
    }

    session = &g_sessions[g_sessionCount];

    strcpy(session->m_groupName, _groupName);
    strcpy(session->m_ip, _ip);

    session->m_port = _port;
    session->m_senderPid = -1;
    session->m_receiverPid = -1;

    ++g_sessionCount;

    return session;
}
static void RemoveChatSession(const char *_groupName)
{
    size_t i;

    if (_groupName == NULL)
    {
        return;
    }

    for (i = 0; i < g_sessionCount; ++i)
    {
        if (strcmp(g_sessions[i].m_groupName, _groupName) == 0)
        {
            g_sessions[i] = g_sessions[g_sessionCount - 1];
            --g_sessionCount;
            return;
        }
    }
}
static int StopChatSession(const char *_groupName)
{
    ChatSession *session;
    pid_t senderPid;
    pid_t receiverPid;
    char groupNameCopy[MAX_GROUP_NAME_SIZE + 1];

    if (_groupName == NULL)
    {
        return -1;
    }

    session = FindChatSession(_groupName);
    if (session == NULL)
    {
        return -1;
    }

    strcpy(groupNameCopy, session->m_groupName);

    senderPid = session->m_senderPid;
    receiverPid = session->m_receiverPid;

    if (senderPid > 0)
    {
        kill(senderPid, SIGTERM);
        waitpid(senderPid, NULL, 0);
    }

    if (receiverPid > 0)
    {
        kill(receiverPid, SIGTERM);
        waitpid(receiverPid, NULL, 0);
    }

    RemoveChatSession(groupNameCopy);

    printf("Chat session stopped: %s\n", groupNameCopy);

    return 0;
}

static int GetGroupName(char *_groupName)
{
    if (_groupName == NULL)
    {
        return -1;
    }

    printf("Enter group name: ");
    fgets(_groupName, MAX_GROUP_NAME_SIZE + 1, stdin);
    _groupName[strcspn(_groupName, "\n")] = '\0';

    return 0;
}

int StartCreateGroupRequest(TcpPacket *_packet)
{
    char groupName[MAX_GROUP_NAME_SIZE + 1];

    if (_packet == NULL)
    {
        return -1;
    }

    if (GetGroupName(groupName) < 0)
    {
        return -1;
    }

    int ret = BuildCreateGroupRequest(groupName, _packet);

    if (ret == 0)
    {
        strcpy(g_pendingGroupName, groupName);
    }

    return ret;
}
int StartJoinGroupRequest(TcpPacket *_packet)
{
    char groupName[MAX_GROUP_NAME_SIZE + 1];

    if (_packet == NULL)
    {
        return -1;
    }

    if (GetGroupName(groupName) < 0)
    {
        return -1;
    }

    int ret = BuildJoinGroupRequest(groupName, _packet);

    if (ret == 0)
    {
        strcpy(g_pendingGroupName, groupName);
    }

    return ret;
}

int StartLeaveGroupRequest(TcpPacket *_packet)
{
    char groupName[MAX_GROUP_NAME_SIZE + 1];

    if (_packet == NULL)
    {
        return -1;
    }

    if (GetGroupName(groupName) < 0)
    {
        return -1;
    }

    if (BuildLeaveGroupRequest(groupName, _packet) < 0)
    {
        return -1;
    }

    strcpy(g_pendingGroupName, groupName);

    return 0;
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
static int GetUserNameAndPassword(char *_username, char *_password)
{
    if (_username == NULL || _password == NULL)
    {
        return -1;
    }

    printf("Enter username: ");
    fgets(_username, MAX_USERNAME_SIZE + 1, stdin);
    _username[strcspn(_username, "\n")] = '\0';
    printf("Enter password: ");
    fgets(_password, MAX_PASSWORD_SIZE + 1, stdin);
    _password[strcspn(_password, "\n")] = '\0';

    return 0;
}

int StartRegisterRequest(TcpPacket *_packet)
{
    char user[MAX_USERNAME_SIZE + 1];
    char pass[MAX_PASSWORD_SIZE + 1];

    if (_packet == NULL)
    {
        return -1;
    }

    printf("Registration selected\n");

    if (GetUserNameAndPassword(user, pass) < 0)
    {
        printf("Failed to get username and password\n");
        return -1;
    }

    if (BuildRegisterRequest(user, pass, _packet) < 0)
    {
        printf("Failed to build register request\n");
        return -1;
    }

    return 0;
}
int StartLoginRequest(TcpPacket *_packet)
{
    char user[MAX_USERNAME_SIZE + 1];
    char pass[MAX_PASSWORD_SIZE + 1];

    if (_packet == NULL)
    {
        return -1;
    }

    printf("Login selected\n");

    if (GetUserNameAndPassword(user, pass) < 0)
    {
        printf("Failed to get username and password\n");
        return -1;
    }

    if (BuildLoginRequest(user, pass, _packet) < 0)
    {
        printf("Failed to build login request\n");
        return -1;
    }

    strcpy(g_loggedInUsername, user);
    return 0;
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
