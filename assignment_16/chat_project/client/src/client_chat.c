#include "client_chat.h"
#include "protocol.h"
#include "multicast.h"
#include "chat_ipc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define MAX_ACTIVE_GROUPS 100

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

extern char g_loggedInUsername[MAX_USERNAME_SIZE + 1]; //TODO remove


static ChatSession *FindChatSession(const char *_groupName);
static ChatSession *AddChatSession(const char *_groupName,const char *_ip,int _port);
static void RemoveChatSession(const char *_groupName);


int StartChatSession(const char *_groupName,
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
 int StopChatSession(const char *_groupName)
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

 void StopAllChatSessions(void)
{
    while (g_sessionCount > 0)
    {
        StopChatSession(g_sessions[0].m_groupName);
    }
}
