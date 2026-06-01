#include "./../include/protocol.h"
#include <string.h>

static int BuildUserPasswordRequest(const char *_username, const char *_password,
                                    TcpPacket *_packet)
{
    size_t usernameLen;
    size_t passwordLen;
    size_t valueLen;

    if (_username == NULL || _password == NULL || _packet == NULL)
    {
        return -1;
    }

    usernameLen = strlen(_username);
    passwordLen = strlen(_password);

    if (usernameLen == 0 || usernameLen > MAX_USERNAME_SIZE ||
        passwordLen == 0 || passwordLen > MAX_PASSWORD_SIZE)
    {
        return -1;
    }

    valueLen = 1 + usernameLen + 1 + passwordLen;

    if (valueLen > MAX_VALUE_SIZE)
    {
        return -1;
    }

    _packet->length = (uint8_t)(valueLen);

    _packet->value[0] = (uint8_t)usernameLen;
    memcpy(_packet->value + 1, _username, usernameLen);

    _packet->value[1 + usernameLen] = (uint8_t)passwordLen;
    memcpy(_packet->value + 1 + usernameLen + 1, _password, passwordLen);

    return 0;
}

static int ParseUserPasswordRequest(const TcpPacket *_packet, char *_username,
                                    char *_password)
{
    if (_packet == NULL || _username == NULL || _password == NULL)
    {
        return -1;
    }

    if (_packet->length < 2)
    {
        return -1;
    }

    size_t usernameLen = _packet->value[0];

    if (usernameLen == 0 || usernameLen > MAX_USERNAME_SIZE)
    {
        return -1;
    }

    if ((size_t)_packet->length < 1 + usernameLen + 1)
    {
        return -1;
    }

    size_t passwordLen = _packet->value[1 + usernameLen];

    if (passwordLen == 0 || passwordLen > MAX_PASSWORD_SIZE)
    {
        return -1;
    }

    if ((size_t)_packet->length != 1 + usernameLen + 1 + passwordLen)
    {
        return -1;
    }

    memcpy(_username, _packet->value + 1, usernameLen);
    _username[usernameLen] = '\0';

    memcpy(_password, _packet->value + 1 + usernameLen + 1, passwordLen);
    _password[passwordLen] = '\0';

    return 0;
}

int BuildRegisterRequest(const char *_username, const char *_password,
                         TcpPacket *_packet)
{
    if (_packet == NULL)
    {
        return -1;
    }
    _packet->tag = (uint8_t)TAG_REGISTER_REQUEST;
    return BuildUserPasswordRequest(_username, _password, _packet);
}

int BuildLoginRequest(const char *_username, const char *_password,
                      TcpPacket *_packet)
{
    if (_packet == NULL)
    {
        return -1;
    }
    _packet->tag = (uint8_t)TAG_LOGIN_REQUEST;
    return BuildUserPasswordRequest(_username, _password, _packet);
}

int ParseRegisterRequest(const TcpPacket *_packet, char *_username, char *_password)
{
    if (_packet == NULL || _username == NULL || _password == NULL)
    {
        return -1;
    }

    if (_packet->tag != TAG_REGISTER_REQUEST)
    {
        return -1;
    }

    return ParseUserPasswordRequest(_packet, _username, _password);
}

int ParseLoginRequest(const TcpPacket *_packet, char *_username, char *_password)
{
    if (_packet == NULL || _username == NULL || _password == NULL)
    {
        return -1;
    }

    if (_packet->tag != TAG_LOGIN_REQUEST)
    {
        return -1;
    }

    return ParseUserPasswordRequest(_packet, _username, _password);
}

int BuildExitRequest(TcpPacket *packet)
{
    if (packet == NULL)
    {
        return -1;
    }

    packet->tag = (uint8_t)TAG_EXIT_REQUEST;
    packet->length = 0;

    return 0;
}

int ParseExitRequest(const TcpPacket *packet)
{
    if (packet == NULL)
    {
        return -1;
    }

    if (packet->tag != TAG_EXIT_REQUEST || packet->length != 0)
    {
        return -1;
    }

    return 0;
}

static int BuildStatusResponse(uint8_t _tag, ResponseStatus _status, TcpPacket *_packet)
{
    if (_packet == NULL)
    {
        return -1;
    }

    if (_status != RESPONSE_STATUS_OK && _status != RESPONSE_STATUS_FAIL)
    {
        return -1;
    }

    _packet->tag = _tag;
    _packet->length = 1;
    _packet->value[0] = (uint8_t)_status;

    return 0;
}

static int ParseStatusResponse(const TcpPacket *_packet, uint8_t _expectedTag,
                               ResponseStatus *_status)
{
    if (_packet == NULL || _status == NULL)
    {
        return -1;
    }

    if (_packet->tag != _expectedTag)
    {
        return -1;
    }

    if (_packet->length != 1)
    {
        return -1;
    }

    if (_packet->value[0] != RESPONSE_STATUS_OK &&
        _packet->value[0] != RESPONSE_STATUS_FAIL)
    {
        return -1;
    }

    *_status = (ResponseStatus)_packet->value[0];

    return 0;
}

int BuildRegisterResponse(ResponseStatus status, TcpPacket *_packet)
{
    return BuildStatusResponse((uint8_t)TAG_REGISTER_RESPONSE, status, _packet);
}

int ParseRegisterResponse(const TcpPacket *_packet, ResponseStatus *_status)
{
    return ParseStatusResponse(_packet, (uint8_t)TAG_REGISTER_RESPONSE, _status);
}

int BuildLoginResponse(ResponseStatus status, TcpPacket *_packet)
{
    return BuildStatusResponse((uint8_t)TAG_LOGIN_RESPONSE, status, _packet);
}

int ParseLoginResponse(const TcpPacket *_packet, ResponseStatus *_status)
{
    return ParseStatusResponse(_packet, (uint8_t)TAG_LOGIN_RESPONSE, _status);
}

size_t GetTcpPacketSize(const TcpPacket *_packet)
{
    if (_packet == NULL)
    {
        return 0;
    }

    return 2 + _packet->length;
}

int BuildLogoutRequest(TcpPacket *_packet)
{
    if (_packet == NULL)
    {
        return -1;
    }

    _packet->tag = (uint8_t)TAG_LOGOUT_REQUEST;
    _packet->length = 0;

    return 0;
}

int ParseLogoutRequest(const TcpPacket *_packet)
{
    if (_packet == NULL)
    {
        return -1;
    }

    if (_packet->tag != TAG_LOGOUT_REQUEST)
    {
        return -1;
    }

    if (_packet->length != 0)
    {
        return -1;
    }

    return 0;
}

int BuildLogoutResponse(ResponseStatus status, TcpPacket *_packet)
{
    return BuildStatusResponse((uint8_t)TAG_LOGOUT_RESPONSE, status, _packet);
}

int ParseLogoutResponse(const TcpPacket *_packet, ResponseStatus *_status)
{
    return ParseStatusResponse(_packet, (uint8_t)TAG_LOGOUT_RESPONSE, _status);
}

int BuildExitResponse(ResponseStatus status, TcpPacket *_packet)
{
    return BuildStatusResponse((uint8_t)TAG_EXIT_RESPONSE, status, _packet);
}

int ParseExitResponse(const TcpPacket *_packet, ResponseStatus *_status)
{
    return ParseStatusResponse(_packet, (uint8_t)TAG_EXIT_RESPONSE, _status);
}

static int BuildGroupNameRequest(uint8_t _tag, const char *_groupName, TcpPacket *_packet)
{
    if (_packet == NULL || _groupName == NULL)
    {
        return -1;
    }

    size_t groupNameLen = strlen(_groupName);

    if (groupNameLen == 0 || groupNameLen > MAX_GROUP_NAME_SIZE)
    {
        return -1;
    }

    _packet->tag = _tag;
    _packet->length = (uint8_t)groupNameLen;
    memcpy(_packet->value, _groupName, groupNameLen);

    return 0;
}
static int ParseGroupNameRequest(const TcpPacket *_packet, uint8_t _expectedTag,
                                 char *_groupName)
{
    if (_packet == NULL || _groupName == NULL)
    {
        return -1;
    }

    if (_packet->tag != _expectedTag)
    {
        return -1;
    }

    if (_packet->length == 0 || _packet->length > MAX_GROUP_NAME_SIZE)
    {
        return -1;
    }

    memcpy(_groupName, _packet->value, _packet->length);

    _groupName[_packet->length] = '\0';

    return 0;
}
int BuildCreateGroupRequest(const char *_groupName, TcpPacket *_packet)
{
    return BuildGroupNameRequest((uint8_t)TAG_CREATE_GROUP_REQUEST, _groupName, _packet);
}
int ParseCreateGroupRequest(const TcpPacket *_packet, char *_groupName)
{
    return ParseGroupNameRequest(_packet, (uint8_t)TAG_CREATE_GROUP_REQUEST, _groupName);
}

int BuildJoinGroupRequest(const char *_groupName, TcpPacket *_packet)
{
    return BuildGroupNameRequest((uint8_t)TAG_JOIN_GROUP_REQUEST, _groupName, _packet);
}
int ParseJoinGroupRequest(const TcpPacket *_packet, char *_groupName)
{
    return ParseGroupNameRequest(_packet, (uint8_t)TAG_JOIN_GROUP_REQUEST, _groupName);
}

int BuildLeaveGroupRequest(const char *_groupName, TcpPacket *_packet)
{
    return BuildGroupNameRequest((uint8_t)TAG_LEAVE_GROUP_REQUEST, _groupName, _packet);
}
int ParseLeaveGroupRequest(const TcpPacket *_packet, char *_groupName)
{
    return ParseGroupNameRequest(_packet, (uint8_t)TAG_LEAVE_GROUP_REQUEST, _groupName);
}

/*
Tag    = 1 byte
Length = 1 byte  // طول كل الـ value

Value:
    status  = 1 byte
    ipLen   = 1 byte
    ip      = ipLen bytes
    portLen = 1 byte
    port    = portLen bytes
*/
static int BuildGroupInfoResponse(ResponseStatus _status,
                                  const char *_ip,
                                  const char *_port,
                                  uint8_t _tag,
                                  TcpPacket *_packet)
{
    size_t ipLen;
    size_t portLen;
    size_t valueLen;

    if (_packet == NULL)
    {
        return -1;
    }

    _packet->tag = _tag;

    if (_status == RESPONSE_STATUS_FAIL)
    {
        _packet->length = 1;
        _packet->value[0] = (uint8_t)_status;
        return 0;
    }

    if (_status != RESPONSE_STATUS_OK || _ip == NULL || _port == NULL)
    {
        return -1;
    }

    ipLen = strlen(_ip);
    portLen = strlen(_port);

    if (ipLen == 0 || ipLen >= MAX_MULTICAST_IP_SIZE ||
        portLen == 0 || portLen > 5)
    {
        return -1;
    }

    valueLen = 1 + 1 + ipLen + 1 + portLen; //status(1) + ipLen(1) + ip + portLen(1) + port

    if (valueLen > MAX_VALUE_SIZE)
    {
        return -1;
    }

    _packet->length = (uint8_t)valueLen;

    _packet->value[0] = (uint8_t)_status;

    _packet->value[1] = (uint8_t)ipLen;
    memcpy(_packet->value + 2, _ip, ipLen);

    _packet->value[2 + ipLen] = (uint8_t)portLen;
    memcpy(_packet->value + 2 + ipLen + 1, _port, portLen);

    return 0;
}
static int ParseGroupInfoResponse(const TcpPacket *_packet,
                                  uint8_t _expectedTag,
                                  ResponseStatus *_status,
                                  char *_ip,
                                  char *_port)
{
    size_t ipLen;
    size_t portLen;

    if (_packet == NULL || _status == NULL)
    {
        return -1;
    }

    if (_packet->tag != _expectedTag)
    {
        return -1;
    }

    if (_packet->length < 1)
    {
        return -1;
    }

    *_status = (ResponseStatus)_packet->value[0];

    if (*_status == RESPONSE_STATUS_FAIL)
    {
        return 0;
    }

    if (*_status != RESPONSE_STATUS_OK || _ip == NULL || _port == NULL)
    {
        return -1;
    }

    if (_packet->length < 1 + 1 + 1)
    {
        return -1;
    }

    ipLen = (uint8_t)_packet->value[1];

    if (ipLen == 0 || ipLen >= MAX_MULTICAST_IP_SIZE)
    {
        return -1;
    }

    if ((size_t)_packet->length < 1 + 1 + ipLen + 1)
    {
        return -1;
    }

    portLen = (uint8_t)_packet->value[1 + 1 + ipLen];

    if (portLen == 0 || portLen > 5)
    {
        return -1;
    }

    if ((size_t)_packet->length != 1 + 1 + ipLen + 1 + portLen)
    {
        return -1;
    }

    memcpy(_ip, _packet->value + 2, ipLen);
    _ip[ipLen] = '\0';

    memcpy(_port, _packet->value + 2 + ipLen + 1, portLen);
    _port[portLen] = '\0';

    return 0;
}
int BuildCreateGroupResponse(ResponseStatus _status,
                             const char *_ip,
                             const char *_port,
                             TcpPacket *_packet)
{
    return BuildGroupInfoResponse(_status,
                                  _ip,
                                  _port,
                                  (uint8_t)TAG_CREATE_GROUP_RESPONSE,
                                  _packet);
}

int ParseCreateGroupResponse(const TcpPacket *_packet,
                             ResponseStatus *_status,
                             char *_ip,
                             char *_port)
{
    return ParseGroupInfoResponse(_packet,
                                  (uint8_t)TAG_CREATE_GROUP_RESPONSE,
                                  _status,
                                  _ip,
                                  _port);
}

int BuildJoinGroupResponse(ResponseStatus _status,
                           const char *_ip,
                           const char *_port,
                           TcpPacket *_packet)
{
    return BuildGroupInfoResponse(_status,
                                  _ip,
                                  _port,
                                  (uint8_t)TAG_JOIN_GROUP_RESPONSE,
                                  _packet);
}

int ParseJoinGroupResponse(const TcpPacket *_packet,
                           ResponseStatus *_status,
                           char *_ip,
                           char *_port)
{
    return ParseGroupInfoResponse(_packet,
                                  (uint8_t)TAG_JOIN_GROUP_RESPONSE,
                                  _status,
                                  _ip,
                                  _port);
}

int BuildLeaveGroupResponse(ResponseStatus _status, TcpPacket *_packet)
{
    return BuildStatusResponse((uint8_t)TAG_LEAVE_GROUP_RESPONSE, _status, _packet);
}

int ParseLeaveGroupResponse(const TcpPacket *_packet, ResponseStatus *_status)
{
    return ParseStatusResponse(_packet, (uint8_t)TAG_LEAVE_GROUP_RESPONSE, _status);
}
