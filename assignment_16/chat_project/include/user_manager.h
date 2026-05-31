#ifndef USER_MANAGER_H_
#define USER_MANAGER_H_

#include <stddef.h>
#include "limitation.h"
#include "./../external/ds/include/HashMap.h"
#include "user_manager.h"

typedef struct UserManager UserManager;

typedef enum UserManagerResult
{
    USER_MANAGER_SUCCESS = 0,
    USER_MANAGER_NULL_ERROR,
    USER_MANAGER_ALREADY_EXISTS_ERROR,
    USER_MANAGER_INSERT_ERROR,
    USER_MANAGER_NOT_FOUND_ERROR,
    USER_MANAGER_INVALID_PASSWORD_ERROR,
    USER_MANAGER_ALREADY_LOGGED_IN_ERROR
} UserManagerResult;

UserManager *UserManagerCreate(void);

UserManagerResult UserManagerRegister(UserManager *_manager, const char *_username,
                                      const char *_password);

UserManagerResult UserManagerDestroy(UserManager **manager);

UserManagerResult UserManagerLogin(UserManager *_manager, const char *_username,
                                   const char *_password, const int _clientSocket);

UserManagerResult UserManagerLogout(UserManager *_manager, const int _clientSock);

UserManagerResult UserManagerGetUsernameBySocket(UserManager *_manager,
                                                 int _clientSock,
                                                 char *_username);



#endif // USER_MANAGER_H_