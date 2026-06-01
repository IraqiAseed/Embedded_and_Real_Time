#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "client_manager.h"
#include "client_chat.h"
#include "client_networking.h"
#include "client.h"


extern char g_pendingGroupName[MAX_GROUP_NAME_SIZE + 1]; //TODO remove
extern char g_loggedInUsername[MAX_USERNAME_SIZE + 1];  //TODO remove


static int GetGroupName(char *_groupName);
int RecvTcpPacket(int sock, TcpPacket *_packet);



static int GetUserNameAndPassword(char *_username, char *_password)
{
    if (_username == NULL || _password == NULL)
    {
        return -1;
    }

    printf("Enter username: ");
    fgets(_username, MAX_USERNAME_SIZE + 1, stdin);
    _username[strcspn(_username, "\n")] = '\0';
    printf("Enter password: ");
    fgets(_password, MAX_PASSWORD_SIZE + 1, stdin);
    _password[strcspn(_password, "\n")] = '\0';

    return 0;
}
int ProcessClientRequest(int sock, TcpPacket *packet)
{
    size_t packetSize;
    TcpPacket response;
    ResponseStatus status;

    if (sock < 0 || packet == NULL)
    {
        return -1;
    }

    packetSize = 2 + packet->length;

    if (send(sock, packet, packetSize, 0) < 0)
    {
        perror("send failed");
        return -1;
    }

    if (RecvTcpPacket(sock, &response) <= 0)
    {
        printf("Failed to receive server response\n");
        return -1;
    }

    if (response.tag == TAG_REGISTER_RESPONSE)
    {
        if (ParseRegisterResponse(&response, &status) < 0)
        {
            return -1;
        }

        if (status == RESPONSE_STATUS_OK)
        {
            printf("Registration successful, you can now login!\n");
            return 0; // register success
        }
        else
        {
            printf("Registration failed, username may already exist.\n");
            return -1; // register failed
        }
    }

    if (response.tag == TAG_LOGIN_RESPONSE)
    {
        if (ParseLoginResponse(&response, &status) < 0)
        {
            return -1;
        }

        if (status == RESPONSE_STATUS_OK)
        {
            printf("Login successful, welcome to the chat!\n");
            return 1; // login success
        }
        else
        {
            printf("Login failed, please check your username and password.\n");
            return -1; // login failed
        }
    }
    if (response.tag == TAG_LOGOUT_RESPONSE)
    {
        if (ParseLogoutResponse(&response, &status) < 0)
        {
            printf("Invalid logout response\n");
            return -1;
        }

        if (status == RESPONSE_STATUS_OK)
        {
            StopAllChatSessions();
            printf("Logout successful.\n");
            return 0; // logout success
        }
        else
        {
            printf("Failed to logout.\n");
            return -1; // logout failed
        }
    }
    if (response.tag == TAG_CREATE_GROUP_RESPONSE)
    {
        char ip[MAX_MULTICAST_IP_SIZE];
        char port[8];

        if (ParseCreateGroupResponse(&response, &status, ip, port) < 0)
        {
            printf("Invalid create group response\n");
            return -1;
        }

        if (status == RESPONSE_STATUS_OK)
        {
            printf("Group created successfully! Multicast IP: %s, Port: %s\n", ip, port);
            StartChatSession(g_pendingGroupName, ip, atoi(port));
            return 1; // create group success
        }
        else
        {
            printf("Failed to create group. It may already exist.\n");
            return -1; // create group failed
        }
    }
    if (response.tag == TAG_JOIN_GROUP_RESPONSE)
    {
        char ip[MAX_MULTICAST_IP_SIZE];
        char port[8];

        if (ParseJoinGroupResponse(&response, &status, ip, port) < 0)
        {
            printf("Invalid join group response\n");
            return -1;
        }

        if (status == RESPONSE_STATUS_OK)
        {
            printf("Joined group successfully! Multicast IP: %s, Port: %s\n", ip, port);
            StartChatSession(g_pendingGroupName, ip, atoi(port));
            return 1; // join group success
        }
        else
        {
            printf("Failed to join group. It may not exist.\n");
            return -1; // join group failed
        }
    }
    if (response.tag == TAG_LEAVE_GROUP_RESPONSE)
    {
        if (ParseLeaveGroupResponse(&response, &status) < 0)
        {
            printf("Invalid leave group response\n");
            return -1;
        }

        if (status == RESPONSE_STATUS_OK)
        {
            StopChatSession(g_pendingGroupName);
            printf("Left group successfully\n");
            return 0;
        }
        else
        {
            printf("Failed to leave group.\n");
            return -1;
        }
    }

    if (response.tag == TAG_EXIT_RESPONSE)
    {
        if (ParseExitResponse(&response, &status) < 0)
        {
            printf("Invalid exit response\n");
            return -1;
        }

        if (status == RESPONSE_STATUS_OK)
        {
            StopAllChatSessions();
            printf("Exit successful, goodbye!\n");
            return 0;
        }
        else
        {
            printf("Failed to exit properly.\n");
            return -1;
        }
    }

    return -1;
}


static int GetGroupName(char *_groupName)
{
    if (_groupName == NULL)
    {
        return -1;
    }

    printf("Enter group name: ");
    fgets(_groupName, MAX_GROUP_NAME_SIZE + 1, stdin);
    _groupName[strcspn(_groupName, "\n")] = '\0';

    return 0;
}

int StartCreateGroupRequest(TcpPacket *_packet)
{
    char groupName[MAX_GROUP_NAME_SIZE + 1];

    if (_packet == NULL)
    {
        return -1;
    }

    if (GetGroupName(groupName) < 0)
    {
        return -1;
    }

    int ret = BuildCreateGroupRequest(groupName, _packet);

    if (ret == 0)
    {
        strcpy(g_pendingGroupName, groupName);
    }

    return ret;
}
int StartJoinGroupRequest(TcpPacket *_packet)
{
    char groupName[MAX_GROUP_NAME_SIZE + 1];

    if (_packet == NULL)
    {
        return -1;
    }

    if (GetGroupName(groupName) < 0)
    {
        return -1;
    }

    int ret = BuildJoinGroupRequest(groupName, _packet);

    if (ret == 0)
    {
        strcpy(g_pendingGroupName, groupName);
    }

    return ret;
}

int StartLeaveGroupRequest(TcpPacket *_packet)
{
    char groupName[MAX_GROUP_NAME_SIZE + 1];

    if (_packet == NULL)
    {
        return -1;
    }

    if (GetGroupName(groupName) < 0)
    {
        return -1;
    }

    if (BuildLeaveGroupRequest(groupName, _packet) < 0)
    {
        return -1;
    }

    strcpy(g_pendingGroupName, groupName);

    return 0;
}
int StartRegisterRequest(TcpPacket *_packet)
{
    char user[MAX_USERNAME_SIZE + 1];
    char pass[MAX_PASSWORD_SIZE + 1];

    if (_packet == NULL)
    {
        return -1;
    }

    printf("Registration selected\n");

    if (GetUserNameAndPassword(user, pass) < 0)
    {
        printf("Failed to get username and password\n");
        return -1;
    }

    if (BuildRegisterRequest(user, pass, _packet) < 0)
    {
        printf("Failed to build register request\n");
        return -1;
    }

    return 0;
}
int StartLoginRequest(TcpPacket *_packet)
{
    char user[MAX_USERNAME_SIZE + 1];
    char pass[MAX_PASSWORD_SIZE + 1];

    if (_packet == NULL)
    {
        return -1;
    }

    printf("Login selected\n");

    if (GetUserNameAndPassword(user, pass) < 0)
    {
        printf("Failed to get username and password\n");
        return -1;
    }

    if (BuildLoginRequest(user, pass, _packet) < 0)
    {
        printf("Failed to build login request\n");
        return -1;
    }

    strcpy(g_loggedInUsername, user);
    return 0;
}