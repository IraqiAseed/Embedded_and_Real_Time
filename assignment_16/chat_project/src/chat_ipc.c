#include "./../include/chat_ipc.h"
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int CreatePidQueue(void)
{
    return msgget(IPC_PRIVATE, IPC_CREAT | 0666);
}

int SendPid(int _queueId, long _type, pid_t _pid)
{
    PidMessage msg;

    msg.m_type = _type;
    msg.m_pid = _pid;

    return msgsnd(_queueId, &msg, sizeof(pid_t), 0);
}

int ReceivePid(int _queueId, long _type, pid_t *_pid)
{
    PidMessage msg;

    if (_pid == NULL)
    {
        return -1;
    }

    if (msgrcv(_queueId, &msg, sizeof(pid_t), _type, 0) < 0)
    {
        return -1;
    }

    *_pid = msg.m_pid;

    return 0;
}