#ifndef CHAT_IPC_H_
#define CHAT_IPC_H_

#include <sys/types.h>

typedef struct PidMessage
{
    long m_type;
    pid_t m_pid;

} PidMessage;

int CreatePidQueue(void);

int SendPid(int _queueId, long _type, pid_t _pid);

int ReceivePid(int _queueId, long _type, pid_t *_pid);

#endif