#ifndef GROUP_MANAGER_H_
#define GROUP_MANAGER_H_

#include <stddef.h>
#include "limitation.h"

typedef struct GroupManager GroupManager;

typedef enum GroupManagerResult
{
    GROUP_MANAGER_SUCCESS = 0,
    GROUP_MANAGER_NULL_ERROR,
    GROUP_MANAGER_ALREADY_EXISTS_ERROR,
    GROUP_MANAGER_NOT_FOUND_ERROR,
    GROUP_MANAGER_ALLOCATION_ERROR,
    GROUP_MANAGER_INSERT_ERROR,
    GROUP_MANAGER_NO_FREE_MULTICAST_IP_ERROR

} GroupManagerResult;

GroupManager *GroupManagerCreate(void);

GroupManagerResult GroupManagerDestroy(GroupManager **_manager);

GroupManagerResult GroupManagerCreateGroup(GroupManager *_manager, const char *_groupName);

GroupManagerResult GroupManagerFindGroup(GroupManager *_manager,
                                         const char *_groupName,
                                         char *_multicastIp,
                                         int *_multicastPort);

GroupManagerResult GroupManagerAddUserToGroup(GroupManager *_manager,
                                              const char *_groupName,
                                              const char *_username);

GroupManagerResult GroupManagerRemoveUserFromGroup(GroupManager *_manager,
                                                   const char *_groupName,
                                                   const char *_username);

GroupManagerResult GroupManagerRemoveUserFromAllGroups(GroupManager *_manager,
                                                       const char *_username);

#endif