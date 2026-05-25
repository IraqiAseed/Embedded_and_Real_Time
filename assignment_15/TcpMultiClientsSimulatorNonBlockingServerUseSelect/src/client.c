

#include "./../include/client.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

struct clientList
{
    int *m_clients;
    size_t *m_freeSlots;
    size_t m_freeSize;

    size_t m_size;
    size_t m_capacity;
};

clientList *CreateClientList(size_t _capacity)
{
    clientList *clients = (clientList *)malloc(sizeof(clientList));

    if (clients == NULL)
    {
        return NULL;
    }

    clients->m_clients = (int *)calloc(_capacity, sizeof(int));

    if (clients->m_clients == NULL)
    {
        free(clients);
        return NULL;
    }

    clients->m_freeSlots = (size_t *)calloc(_capacity, sizeof(size_t));
    if (clients->m_freeSlots == NULL)
    {
        free(clients->m_clients);
        free(clients);
        return NULL;
    }

    clients->m_size = 0;
    clients->m_capacity = _capacity;

    for (size_t i = 0; i < _capacity; i++)
    {
        clients->m_clients[i] = -1;
        clients->m_freeSlots[i] = i;
    }
    clients->m_freeSize = _capacity;

    return clients;
}

void DestroyClientList(clientList **_clientList)
{
    if (_clientList == NULL || *_clientList == NULL)
    {
        return;
    }

    free((*_clientList)->m_clients);
    free((*_clientList)->m_freeSlots);
    free(*_clientList);
    *_clientList = NULL;
}

int InsertClient(clientList *_clientList, int _socket)
{
    if (_clientList == NULL)
    {
        return -1;
    }
    if (_clientList->m_size >= _clientList->m_capacity)
    {
        return -2;
    }

    size_t index = _clientList->m_freeSlots[--_clientList->m_freeSize];

    _clientList->m_clients[index] = _socket;
    _clientList->m_size++;

    return (int)index;
}

int RemoveClient(clientList *_clientList, size_t index)
{
    if (_clientList == NULL)
    {
        return -1;
    }

    if (index >= _clientList->m_capacity)
    {
        return -2;
    }

    if (_clientList->m_clients[index] == -1)
    {
        return -3;
    }

    close(_clientList->m_clients[index]);

    _clientList->m_clients[index] = -1;

    _clientList->m_freeSlots[_clientList->m_freeSize] = index;

    _clientList->m_freeSize++;

    _clientList->m_size--;

    return 0;
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

int FillClientList(clientList *list)
{

    if (list == NULL)
    {
        return -1;
    }

    while (list->m_size < list->m_capacity)
    {
        int fd = ConnectToServer(SERVER_IP, SERVER_PORT);

        if (fd < 0)
        {
            break;
        }

        InsertClient(list, fd);
    }

    return 0;
}
void ProcessClient(clientList *list, size_t *currentIndex)
{
    size_t i = *currentIndex;

    int random = rand() % 100 + 1;

    if (list->m_clients[i] == -1)
    {
        if (random <= 30)
        {
            int fd = ConnectToServer(SERVER_IP, SERVER_PORT);
            if (fd >= 0)
            {
                InsertClient(list, fd);
            }
        }
    }
    else
    {
        if (random <= 5)
        {
            RemoveClient(list, i);
        }
        else if (random <= 35)
        {
            int res = SendReceive(list->m_clients[i]);
            if (res < 0)
            {
                RemoveClient(list, i);
            }
        }
    }

    *currentIndex = (*currentIndex + 1) % list->m_capacity;
}

int SendReceive(int sock)
{
    char sendBuffer[BUFFER_SIZE];
    char recvBuffer[BUFFER_SIZE];

    snprintf(sendBuffer,
         sizeof(sendBuffer),
         "hello from client %d\n",
         sock);

    if (send(sock, sendBuffer, strlen(sendBuffer), 0) < 0)
    {
        perror("send failed");
        return -1;
    }

    ssize_t bytesReceived = recv(sock,
                                 recvBuffer,
                                 sizeof(recvBuffer) - 1,
                                 0);

    if (bytesReceived < 0)
    {
        perror("recv failed");
        return -1;
    }

    if (bytesReceived == 0)
    {
        printf("Server disconnected.\n");
        return -2;
    }

    recvBuffer[bytesReceived] = '\0';

    printf("%s", recvBuffer);

    return 0;
}

int main()
{
    size_t currentIndex = 0;
    clientList *clients = CreateClientList(100);
    int res = FillClientList(clients);
    if(res == -1)
    {
        return 0;
    }

    while (1)
    {
        ProcessClient(clients, &currentIndex);
        usleep(100000);
    }

    DestroyClientList(&clients);
    return 0;
}

/*
int main(void)
{
    int sock = ConnectToServer(SERVER_IP, SERVER_PORT);
    if (sock < 0)
    {
        return 1;
    }

    printf("Connected to server.\n");

    char sendBuffer[BUFFER_SIZE];
    char recvBuffer[BUFFER_SIZE];

    while (1)
    {
        printf("Enter message: ");

        if (fgets(sendBuffer, sizeof(sendBuffer), stdin) == NULL)
        {
            break;
        }

        if (strncmp(sendBuffer, "exit", 4) == 0)
        {
            break;
        }

        if (send(sock, sendBuffer, strlen(sendBuffer), 0) < 0)
        {
            perror("send failed");
            break;
        }

        ssize_t bytesReceived = recv(sock,
                                     recvBuffer,
                                     sizeof(recvBuffer) - 1,
                                     0);

        if (bytesReceived < 0)
        {
            perror("recv failed");
            break;
        }

        if (bytesReceived == 0)
        {
            printf("Server disconnected.\n");
            break;
        }

        recvBuffer[bytesReceived] = '\0';

        printf("%s", recvBuffer);
    }

    close(sock);

    return 0;
}

*/
