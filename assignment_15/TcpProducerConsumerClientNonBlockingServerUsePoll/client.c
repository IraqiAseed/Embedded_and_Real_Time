#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "circularQueue.h"
#include <semaphore.h>
#include <pthread.h>

pthread_mutex_t mutex;

sem_t empty;
sem_t full;

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024
#define QUEUE_CAPACITY 100

void DestroySocketItem(void *sock)
{
    int fd = *(int *)sock;
    close(fd);
    free(sock);
}

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


int ProducerConnectAndInsert(Queue *queue)
{
    int fd = ConnectToServer(SERVER_IP, SERVER_PORT);
    if (fd < 0)
    {
        return 1;
    }

    int *fdPtr = malloc(sizeof(int));
    if (fdPtr == NULL)
    {
        close(fd);
        return 1;
    }

    *fdPtr = fd;

    sem_wait(&empty);

    pthread_mutex_lock(&mutex);

    QueueResult res = QueueInsert(queue, fdPtr);

    pthread_mutex_unlock(&mutex);

    if (res != QUEUE_SUCCESS)
    {
        sem_post(&empty);
        close(fd);
        free(fdPtr);
        return 1;
    }

    sem_post(&full);

    printf("Connected and inserted socket %d\n", fd);

    return 0;
}
void *ProducerThread(void *arg)
{
    Queue *queue = (Queue *)arg;

    while (1)
    {
        ProducerConnectAndInsert(queue);
        sleep(1);
    }

    return NULL;
}


int ConsumerConsumeSocket(Queue *queue)
{
    void *item = NULL;
    int fd;

    sem_wait(&full);

    pthread_mutex_lock(&mutex);

    QueueResult res = QueueRemove(queue, &item);

    pthread_mutex_unlock(&mutex);

    sem_post(&empty);

    if (res != QUEUE_SUCCESS || item == NULL)
    {
        return 1;
    }

    fd = *(int *)item;
    free(item);

    char sendBuffer[BUFFER_SIZE];
    char recvBuffer[BUFFER_SIZE];

    strcpy(sendBuffer, "hello\n");

    if (send(fd, sendBuffer, strlen(sendBuffer), 0) < 0)
    {
        perror("send failed");
        close(fd);
        return 1;
    }

    ssize_t bytesReceived = recv(fd,
                                 recvBuffer,
                                 sizeof(recvBuffer) - 1,
                                 0);

    if (bytesReceived < 0)
    {
        perror("recv failed");
        close(fd);
        return 1;
    }

    if (bytesReceived == 0)
    {
        printf("Server disconnected.\n");
        close(fd);
        return 0;
    }

    recvBuffer[bytesReceived] = '\0';

    printf("%s", recvBuffer);

    close(fd);

    return 0;
}
void *ConsumerThread(void *arg)
{
    Queue *queue = (Queue *)arg;

    while (1)
    {
        ConsumerConsumeSocket(queue);
        sleep(1);
    }

    return NULL;
}
int main(void)
{

    Queue *queue = QueueCreate(QUEUE_CAPACITY);

    if (queue == NULL)
    {
        return 1;
    }

    pthread_mutex_init(&mutex, NULL);

    sem_init(&empty, 0, QUEUE_CAPACITY);
    sem_init(&full, 0, 0);

    pthread_t producer;
    pthread_t consumer;

    if (pthread_create(&producer, NULL, ProducerThread, queue) != 0)
    {
        perror("pthread_create producer failed");
        return 1;
    }

    if (pthread_create(&consumer, NULL, ConsumerThread, queue) != 0)
    {
        perror("pthread_create consumer failed");
        return 1;
    }

    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    pthread_mutex_destroy(&mutex);

    sem_destroy(&empty);
    sem_destroy(&full);

    QueueDestroy(&queue, DestroySocketItem);

    return 0;
}
