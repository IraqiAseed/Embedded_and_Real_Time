#ifndef SERVER_MANAGER_H_
#define SERVER_MANAGER_H_

#include "protocol.h"
#include "user_manager.h"
#include "group_manager.h"

#include <sys/select.h>

int ProcessClient(int _clientSock,
                  fd_set *_masterSet,
                  UserManager *_userManager,
                  GroupManager *_groupManager);

#endif /* SERVER_MANAGER_H_ */