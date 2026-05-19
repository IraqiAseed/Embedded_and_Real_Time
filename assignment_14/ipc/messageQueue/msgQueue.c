#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define TEXT_SIZE 100

typedef struct Message
{
    long m_type;
    char m_text[TEXT_SIZE];
} Message;

int main(void)
{
    int msgid;
    Message msg;

    msgid = msgget(IPC_PRIVATE, IPC_CREAT | 0666);

    if (msgid == -1)
    {
        perror("msgget");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        msgctl(msgid, IPC_RMID, NULL);
        return 1;
    }

    if (pid == 0)
    {
        /* Child */

        if (msgrcv(msgid, &msg, sizeof(msg.m_text), 1, 0) == -1)
        {
            perror("child msgrcv");
            exit(1);
        }

        printf("Child received: %s\n", msg.m_text);

        msg.m_type = 2;
        strcpy(msg.m_text, "pong");

        if (msgsnd(msgid, &msg, sizeof(msg.m_text), 0) == -1)
        {
            perror("child msgsnd");
            exit(1);
        }

        exit(0);
    }
    else
    {
        /* Parent */

        msg.m_type = 1;
        strcpy(msg.m_text, "ping");

        if (msgsnd(msgid, &msg, sizeof(msg.m_text), 0) == -1)
        {
            perror("parent msgsnd");
            msgctl(msgid, IPC_RMID, NULL);
            return 1;
        }

        if (msgrcv(msgid, &msg, sizeof(msg.m_text), 2, 0) == -1)
        {
            perror("parent msgrcv");
            msgctl(msgid, IPC_RMID, NULL);
            return 1;
        }

        printf("Parent received: %s\n", msg.m_text);

        wait(NULL);

        msgctl(msgid, IPC_RMID, NULL);
    }

    return 0;
}