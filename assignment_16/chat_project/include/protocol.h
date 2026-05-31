#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <stddef.h>
#include <stdint.h>
#include "./limitation.h"

typedef enum Tag
{
    TAG_EXIT_REQUEST = 1,
    TAG_REGISTER_REQUEST,
    TAG_LOGIN_REQUEST,
    TAG_LOGOUT_REQUEST,

    TAG_CREATE_GROUP_REQUEST,
    TAG_JOIN_GROUP_REQUEST,
    TAG_LEAVE_GROUP_REQUEST,

    TAG_EXIT_RESPONSE,
    TAG_REGISTER_RESPONSE,
    TAG_LOGIN_RESPONSE,
    TAG_LOGOUT_RESPONSE,

    TAG_CREATE_GROUP_RESPONSE,
    TAG_JOIN_GROUP_RESPONSE,
    TAG_LEAVE_GROUP_RESPONSE

} Tag;

typedef enum ResponseStatus
{
    RESPONSE_STATUS_FAIL = 0,
    RESPONSE_STATUS_OK = 1

} ResponseStatus;

typedef struct TcpPacket
{
    uint8_t tag;
    uint8_t length;
    uint8_t value[MAX_VALUE_SIZE];
} TcpPacket;

//[tag][length][value]
// Register Request
int BuildRegisterRequest(const char *username, const char *password, TcpPacket *packet);
int ParseRegisterRequest(const TcpPacket *_packet, char *_username, char *_password);

int BuildRegisterResponse(ResponseStatus status, TcpPacket *_packet);
int ParseRegisterResponse(const TcpPacket *_packet, ResponseStatus *_status);

int BuildLoginRequest(const char *username, const char *password, TcpPacket *packet);
int ParseLoginRequest(const TcpPacket *_packet, char *_username, char *_password);

int BuildLoginResponse(ResponseStatus status, TcpPacket *_packet);
int ParseLoginResponse(const TcpPacket *_packet, ResponseStatus *_status);

int BuildExitRequest(TcpPacket *packet);
int ParseExitRequest(const TcpPacket *packet);

int BuildExitResponse(ResponseStatus status, TcpPacket *_packet);
int ParseExitResponse(const TcpPacket *_packet, ResponseStatus *_status);

int BuildLogoutRequest(TcpPacket *_packet);
int ParseLogoutRequest(const TcpPacket *_packet);

int BuildLogoutResponse(ResponseStatus status, TcpPacket *_packet);
int ParseLogoutResponse(const TcpPacket *_packet, ResponseStatus *_status);

int BuildCreateGroupRequest(const char *_groupName, TcpPacket *_packet);
int ParseCreateGroupRequest(const TcpPacket *_packet, char *_groupName);

int BuildJoinGroupRequest(const char *_groupName, TcpPacket *_packet);
int ParseJoinGroupRequest(const TcpPacket *_packet, char *_groupName);

int BuildLeaveGroupRequest(const char *_groupName, TcpPacket *_packet);
int ParseLeaveGroupRequest(const TcpPacket *_packet, char *_groupName);

int BuildCreateGroupResponse(ResponseStatus _status,
                             const char *_ip,
                             const char *_port,
                             TcpPacket *_packet);

int ParseCreateGroupResponse(const TcpPacket *_packet,
                             ResponseStatus *_status,
                             char *_ip,
                             char *_port);

int BuildJoinGroupResponse(ResponseStatus _status,
                           const char *_ip,
                           const char *_port,
                           TcpPacket *_packet);

int ParseJoinGroupResponse(const TcpPacket *_packet,
                           ResponseStatus *_status,
                           char *_ip,
                           char *_port);

int BuildLeaveGroupResponse(ResponseStatus _status, TcpPacket *_packet);
int ParseLeaveGroupResponse(const TcpPacket *_packet, ResponseStatus *_status);

size_t GetTcpPacketSize(const TcpPacket *_packet);

#endif // PROTOCOL_H_