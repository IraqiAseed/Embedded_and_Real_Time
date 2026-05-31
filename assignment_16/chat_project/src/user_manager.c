#include <string.h>
#include <stdlib.h>
#include "./../include/user_manager.h"
#include "./../include/protocol.h"

typedef struct User
{
    char m_username[MAX_USERNAME_SIZE + 1];
    char m_password[MAX_PASSWORD_SIZE + 1];

    int m_isActive;
    int m_socket;

} User;

struct UserManager
{
    HashMap *m_username_to_user; // All registered accounts
    HashMap *m_socket_to_user;   // Currently logged in users
};

static size_t UserHashFunction(void *key)
{
    const char *str = key;
    size_t hash = 5381;

    while (*str != '\0')
    {
        hash = hash * 33 + (unsigned char)*str;
        ++str;
    }

    return hash;
}
static int UserEqualityFunction(void *first, void *second)
{
    return strcmp((char *)first, (char *)second) == 0;
}
static size_t SocketHashFunction(void *key)
{
    int socket = *(int *)key;
    return (size_t)socket;
}

static int SocketEqualityFunction(void *first, void *second)
{
    return *(int *)first == *(int *)second;
}
void ValueDestroy(void *_user)
{
    if (_user != NULL)
    {
        free((User *)_user);
    }
}
UserManager *UserManagerCreate(void)
{
    UserManager *manager;

    manager = malloc(sizeof(UserManager));

    if (manager == NULL)
    {
        return NULL;
    }

    manager->m_username_to_user = HashMap_Create(MAX_USERS, UserHashFunction,
                                                 UserEqualityFunction);

    if (manager->m_username_to_user == NULL)
    {
        free(manager);
        return NULL;
    }
    manager->m_socket_to_user = HashMap_Create(MAX_USERS, SocketHashFunction,
                                               SocketEqualityFunction);

    if (manager->m_socket_to_user == NULL)
    {
        HashMap_Destroy(&manager->m_username_to_user, NULL, ValueDestroy);
        free(manager);
        return NULL;
    }

    return manager;
}

UserManagerResult UserManagerDestroy(UserManager **manager)
{
    if (manager == NULL || *manager == NULL)
    {
        return USER_MANAGER_NULL_ERROR;
    }

    HashMap_Destroy(&(*manager)->m_username_to_user,
                    NULL,
                    ValueDestroy);

    free(*manager);
    *manager = NULL;

    return USER_MANAGER_SUCCESS;
}

UserManagerResult UserManagerRegister(UserManager *_manager,
                                      const char *_username,
                                      const char *_password)
{
    User *user = NULL;

    if (_manager == NULL || _username == NULL || _password == NULL)
    {
        return USER_MANAGER_NULL_ERROR;
    }

    if (HashMap_Find(_manager->m_username_to_user, (void *)_username,
                     (void **)&user) == MAP_SUCCESS)
    {
        return USER_MANAGER_ALREADY_EXISTS_ERROR;
    }

    user = malloc(sizeof(User));
    if (user == NULL)
    {
        return USER_MANAGER_INSERT_ERROR;
    }

    strcpy(user->m_username, _username);
    strcpy(user->m_password, _password);
    user->m_isActive = 0;
    user->m_socket = -1;

    if (HashMap_Insert(_manager->m_username_to_user,
                       user->m_username,
                       user) != MAP_SUCCESS)
    {
        free(user);
        return USER_MANAGER_INSERT_ERROR;
    }

    return USER_MANAGER_SUCCESS;
}

UserManagerResult UserManagerLogin(UserManager *_manager, const char *_username,
                                   const char *_password, const int _clientSocket)
{
    User *user;

    if (_manager == NULL || _username == NULL || _password == NULL)
    {
        return USER_MANAGER_NULL_ERROR;
    }

    if (HashMap_Find(_manager->m_username_to_user, (void *)_username,
                     (void **)&user) != MAP_SUCCESS)
    {
        return USER_MANAGER_NOT_FOUND_ERROR;
    }

    if (strcmp(user->m_password, _password) != 0)
    {
        return USER_MANAGER_INVALID_PASSWORD_ERROR;
    }

    if (user->m_isActive)
    {
        return USER_MANAGER_ALREADY_LOGGED_IN_ERROR;
    }

    user->m_isActive = 1;
    user->m_socket = _clientSocket;

    if (HashMap_Insert(_manager->m_socket_to_user, &user->m_socket, user) != MAP_SUCCESS)
    {
        user->m_isActive = 0;
        user->m_socket = -1;
        return USER_MANAGER_INSERT_ERROR;
    }

    return USER_MANAGER_SUCCESS;
}

UserManagerResult UserManagerLogout(UserManager *_manager, const int _clientSock)
{
    User *user = NULL;

    if (_manager == NULL || _clientSock < 0)
    {
        return USER_MANAGER_NULL_ERROR;
    }

    if (HashMap_Find(_manager->m_socket_to_user, &_clientSock,
                     (void **)&user) != MAP_SUCCESS)
    {
        return USER_MANAGER_NOT_FOUND_ERROR;
    }

    void *pKey = NULL;
    void *pValue = NULL;

    if (HashMap_Remove(_manager->m_socket_to_user, &_clientSock, &pKey,
                       &pValue) != MAP_SUCCESS)
    {
        return USER_MANAGER_NOT_FOUND_ERROR;
    }

    user = (User *)pValue;

    user->m_isActive = 0;
    user->m_socket = -1;

    return USER_MANAGER_SUCCESS;
}

UserManagerResult UserManagerGetUsernameBySocket(UserManager *_manager,
                                                 int _clientSock,
                                                 char *_username)
{
    User *user = NULL;

    if (_manager == NULL || _clientSock < 0 || _username == NULL)
    {
        return USER_MANAGER_NULL_ERROR;
    }

    if (HashMap_Find(_manager->m_socket_to_user,
                     &_clientSock,
                     (void **)&user) != MAP_SUCCESS)
    {
        return USER_MANAGER_NOT_FOUND_ERROR;
    }

    strcpy(_username, user->m_username);

    return USER_MANAGER_SUCCESS;
}
