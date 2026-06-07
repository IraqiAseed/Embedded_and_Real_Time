#include "group_manager.h"
#include "HashMap.h"
#include "circularQueue.h"
#include "doubleLinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Group
{
    char m_groupName[MAX_GROUP_NAME_SIZE + 1];
    char *m_multicastIp; //[MAX_MULTICAST_IP_SIZE];
    int m_multicastPort; // port is default port
    List *m_members;
    size_t m_memberCount;

} Group;

struct GroupManager
{
    HashMap *m_groupname_to_group; /* key: group name, value: Group* */
    Queue *m_freeMulticastIps;     /* available multicast IPs */
    int m_nextMulticastPort;
};

static int FillMulticastIpQueue(Queue *_queue)
{
    int i;

    if (_queue == NULL)
    {
        return -1;
    }

    for (i = 1; i <= MAX_GROUPS; ++i)
    {
        char *ip = malloc(MAX_MULTICAST_IP_SIZE);

        if (ip == NULL)
        {
            return -1;
        }

        snprintf(ip, MAX_MULTICAST_IP_SIZE, "239.0.0.%d", i);

        if (QueueInsert(_queue, ip) != QUEUE_SUCCESS)
        {
            free(ip);
            return -1;
        }
    }

    return 0;
}

static size_t GroupHashFunction(void *_key)
{
    char *str = (char *)_key;
    size_t hash = 5381;

    while (*str != '\0')
    {
        hash = hash * 33 + (unsigned char)(*str);
        ++str;
    }

    return hash;
}

static int GroupEqualityFunction(void *_first, void *_second)
{
    return strcmp((char *)_first, (char *)_second) == 0;
}
GroupManager *GroupManagerCreate(void)
{
    GroupManager *manager;

    manager = malloc(sizeof(GroupManager));

    if (manager == NULL)
    {
        return NULL;
    }

    manager->m_nextMulticastPort = DEFAULT_MULTICAST_PORT;

    manager->m_groupname_to_group = HashMap_Create(MAX_GROUPS, GroupHashFunction,
                                                   GroupEqualityFunction);

    if (manager->m_groupname_to_group == NULL)
    {
        free(manager);
        return NULL;
    }

    manager->m_freeMulticastIps = QueueCreate(MAX_GROUPS);

    if (manager->m_freeMulticastIps == NULL)
    {
        HashMap_Destroy(&manager->m_groupname_to_group, NULL, NULL);
        free(manager);
        return NULL;
    }

    if (FillMulticastIpQueue(manager->m_freeMulticastIps) < 0)
    {
        QueueDestroy(&manager->m_freeMulticastIps, free);

        HashMap_Destroy(&manager->m_groupname_to_group, NULL, NULL);

        free(manager);
        return NULL;
    }

    return manager;
}

GroupManagerResult GroupManagerCreateGroup(GroupManager *_manager, const char *_groupName)
{
    Group *group = NULL;
    char *ip = NULL;

    if (_manager == NULL || _groupName == NULL)
    {
        return GROUP_MANAGER_NULL_ERROR;
    }

    if (HashMap_Find(_manager->m_groupname_to_group,
                     (void *)_groupName,
                     (void **)&group) == MAP_SUCCESS)
    {
        return GROUP_MANAGER_ALREADY_EXISTS_ERROR;
    }

    if (QueueRemove(_manager->m_freeMulticastIps, (void **)&ip) != QUEUE_SUCCESS)
    {
        return GROUP_MANAGER_NO_FREE_MULTICAST_IP_ERROR;
    }

    group = malloc(sizeof(Group));

    if (group == NULL)
    {
        QueueInsert(_manager->m_freeMulticastIps, ip); // return IP back to queue
        return GROUP_MANAGER_ALLOCATION_ERROR;
    }

    group->m_members = ListCreate();
    if (group->m_members == NULL)
    {
        free(group);
        QueueInsert(_manager->m_freeMulticastIps, ip); // return IP back to queue
        return GROUP_MANAGER_ALLOCATION_ERROR;
    }

    strcpy(group->m_groupName, _groupName);
    group->m_multicastIp = ip;
    group->m_multicastPort = _manager->m_nextMulticastPort++;
    group->m_memberCount = 0;

    if (HashMap_Insert(_manager->m_groupname_to_group,
                       group->m_groupName,
                       group) != MAP_SUCCESS)
    {
        ListDestroy(&group->m_members, NULL);
        QueueInsert(_manager->m_freeMulticastIps, group->m_multicastIp); // return IP back to queue
        group->m_multicastIp = NULL;
        free(group);
        return GROUP_MANAGER_INSERT_ERROR;
    }

    return GROUP_MANAGER_SUCCESS;
}

GroupManagerResult GroupManagerFindGroup(GroupManager *_manager,
                                         const char *_groupName,
                                         char *_multicastIp,
                                         int *_multicastPort)
{
    Group *group = NULL;

    if (_manager == NULL || _groupName == NULL ||
        _multicastIp == NULL || _multicastPort == NULL)
    {
        return GROUP_MANAGER_NULL_ERROR;
    }

    if (HashMap_Find(_manager->m_groupname_to_group,
                     (void *)_groupName,
                     (void **)&group) != MAP_SUCCESS)
    {
        return GROUP_MANAGER_NOT_FOUND_ERROR;
    }

    strcpy(_multicastIp, group->m_multicastIp);
    *_multicastPort = group->m_multicastPort;

    return GROUP_MANAGER_SUCCESS;
}
static void GroupValueDestroy(void *_group)
{
    Group *group = (Group *)_group;

    if (group == NULL)
    {
        return;
    }

    ListDestroy(&group->m_members, free);

    free(group->m_multicastIp);
    free(group);
}

GroupManagerResult GroupManagerDestroy(GroupManager **_manager)
{
    if (_manager == NULL || *_manager == NULL)
    {
        return GROUP_MANAGER_NULL_ERROR;
    }

    HashMap_Destroy(&(*_manager)->m_groupname_to_group,
                    NULL,
                    GroupValueDestroy);

    QueueDestroy(&(*_manager)->m_freeMulticastIps, free);

    free(*_manager);
    *_manager = NULL;

    return GROUP_MANAGER_SUCCESS;
}
static ListItr FindMemberInGroup(Group *_group, const char *_username)
{
    ListItr itr;
    ListItr end;
    char *memberName;

    if (_group == NULL || _username == NULL)
    {
        return NULL;
    }

    itr = ListItrBegin(_group->m_members);
    end = ListItrEnd(_group->m_members);

    while (itr != end)
    {
        memberName = ListItrGet(itr);

        if (memberName != NULL && strcmp(memberName, _username) == 0)
        {
            return itr;
        }

        itr = ListItrNext(itr);
    }

    return end;
}
GroupManagerResult GroupManagerAddUserToGroup(GroupManager *_manager,
                                              const char *_groupName,
                                              const char *_username)
{
    Group *group = NULL;
    char *memberName = NULL;

    if (_manager == NULL || _groupName == NULL || _username == NULL)
    {
        return GROUP_MANAGER_NULL_ERROR;
    }

    if (HashMap_Find(_manager->m_groupname_to_group,
                     (void *)_groupName,
                     (void **)&group) != MAP_SUCCESS)
    {
        return GROUP_MANAGER_NOT_FOUND_ERROR;
    }

    if (FindMemberInGroup(group, _username) != ListItrEnd(group->m_members))
    {
        return GROUP_MANAGER_ALREADY_EXISTS_ERROR;
    }

    memberName = malloc(strlen(_username) + 1);

    if (memberName == NULL)
    {
        return GROUP_MANAGER_ALLOCATION_ERROR;
    }

    strcpy(memberName, _username);

    if (ListPushTail(group->m_members, memberName) == NULL)
    {
        free(memberName);
        return GROUP_MANAGER_INSERT_ERROR;
    }

    group->m_memberCount++;

    return GROUP_MANAGER_SUCCESS;
}

static GroupManagerResult RemoveGroup(GroupManager *_manager, Group *_group)
{
    void *pKey = NULL;
    void *pValue = NULL;

    if (_manager == NULL || _group == NULL)
    {
        return GROUP_MANAGER_NULL_ERROR;
    }

    if (HashMap_Remove(_manager->m_groupname_to_group,
                       _group->m_groupName,
                       &pKey,
                       &pValue) != MAP_SUCCESS)
    {
        return GROUP_MANAGER_NOT_FOUND_ERROR;
    }

    printf("Group deleted: %s\n", _group->m_groupName);

    if (QueueInsert(_manager->m_freeMulticastIps,
                    _group->m_multicastIp) != QUEUE_SUCCESS)
    {
        return GROUP_MANAGER_INSERT_ERROR;
    }
    _group->m_multicastIp = NULL;

    ListDestroy(&_group->m_members, free);

    free(_group);

    return GROUP_MANAGER_SUCCESS;
}

GroupManagerResult GroupManagerRemoveUserFromGroup(GroupManager *_manager,
                                                   const char *_groupName,
                                                   const char *_username)
{
    Group *group = NULL;
    ListItr memberItr;

    if (_manager == NULL || _groupName == NULL || _username == NULL)
    {
        return GROUP_MANAGER_NULL_ERROR;
    }

    if (HashMap_Find(_manager->m_groupname_to_group,
                     (void *)_groupName,
                     (void **)&group) != MAP_SUCCESS)
    {
        return GROUP_MANAGER_NOT_FOUND_ERROR;
    }

    memberItr = FindMemberInGroup(group, _username);

    if (memberItr == ListItrEnd(group->m_members))
    {
        return GROUP_MANAGER_NOT_FOUND_ERROR;
    }

    void *memberName = ListItrRemove(memberItr);

    if (memberName == NULL)
    {
        return GROUP_MANAGER_NOT_FOUND_ERROR;
    }

    free(memberName);

    group->m_memberCount--;

    if (group->m_memberCount == 0)
    {
        return RemoveGroup(_manager, group);
    }

    return GROUP_MANAGER_SUCCESS;
}
typedef struct RemoveUserContext
{
    const char *m_username;

} RemoveUserContext;

static int RemoveUserFromGroupAction(const void *_key,
                                     void *_value,
                                     void *_context)
{
    Group *group = (Group *)_value;
    RemoveUserContext *context = (RemoveUserContext *)_context;

    ListItr memberItr;
    void *memberName;

    (void)_key;

    if (group == NULL || context == NULL)
    {
        return 1;
    }

    memberItr = FindMemberInGroup(group,
                                  context->m_username);

    if (memberItr == ListItrEnd(group->m_members))
    {
        return 1;
    }

    memberName = ListItrRemove(memberItr);

    if (memberName != NULL)
    {
        free(memberName);
        group->m_memberCount--;
    }

    return 1;
}
typedef struct RemoveEmptyGroupsContext
{
    GroupManager *m_manager;
    char m_groupNames[MAX_GROUPS][MAX_GROUP_NAME_SIZE + 1];
    size_t m_count;

} RemoveEmptyGroupsContext;

static int CollectEmptyGroupsAction(const void *_key,
                                    void *_value,
                                    void *_context)
{
    Group *group = (Group *)_value;
    RemoveEmptyGroupsContext *context = (RemoveEmptyGroupsContext *)_context;

    (void)_key;

    if (group == NULL || context == NULL)
    {
        return 1;
    }

    if (group->m_memberCount == 0 && context->m_count < MAX_GROUPS)
    {
        strcpy(context->m_groupNames[context->m_count],
               group->m_groupName);

        context->m_count++;
    }

    return 1;
}
static void RemoveEmptyGroups(GroupManager *_manager)
{
    RemoveEmptyGroupsContext context;
    size_t i;
    Group *group = NULL;

    if (_manager == NULL)
    {
        return;
    }

    context.m_manager = _manager;
    context.m_count = 0;

    HashMap_ForEach(_manager->m_groupname_to_group,
                    CollectEmptyGroupsAction,
                    &context);

    for (i = 0; i < context.m_count; ++i)
    {
        if (HashMap_Find(_manager->m_groupname_to_group,
                         context.m_groupNames[i],
                         (void **)&group) == MAP_SUCCESS)
        {
            RemoveGroup(_manager, group);
        }
    }
}

GroupManagerResult GroupManagerRemoveUserFromAllGroups(GroupManager *_manager,
                                                       const char *_username)
{
    RemoveUserContext context;

    if (_manager == NULL || _username == NULL)
    {
        return GROUP_MANAGER_NULL_ERROR;
    }

    context.m_username = _username;

    HashMap_ForEach(_manager->m_groupname_to_group,
                    RemoveUserFromGroupAction,
                    &context);

    RemoveEmptyGroups(_manager);

    return GROUP_MANAGER_SUCCESS;
}

#ifdef GROUP_MANAGER_TEST

int main(void)
{
    GroupManager *manager;
    char ip[MAX_MULTICAST_IP_SIZE];
    int port;

    manager = GroupManagerCreate();
    if (manager == NULL)
    {
        printf("failed create\n");
        return 1;
    }

    printf("create linux: %d\n", GroupManagerCreateGroup(manager, "linux"));
    printf("create cpp: %d\n", GroupManagerCreateGroup(manager, "cpp"));
    printf("create linux again: %d\n", GroupManagerCreateGroup(manager, "linux"));

    if (GroupManagerFindGroup(manager, "linux", ip, &port) == GROUP_MANAGER_SUCCESS)
    {
        printf("linux found ip=%s port=%d\n", ip, port);
    }
    else
    {
        printf("linux not found\n");
    }

    if (GroupManagerFindGroup(manager, "embedded", ip, &port) == GROUP_MANAGER_SUCCESS)
    {
        printf("embedded found ip=%s port=%d\n", ip, port);
    }
    else
    {
        printf("embedded not found\n");
    }

    GroupManagerDestroy(&manager);

    return 0;
}

#endif