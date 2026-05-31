#ifndef LIMITATION_H_
#define LIMITATION_H_


#define PROTOCOL_HEADER_SIZE 2
#define MAX_VALUE_SIZE 255
#define MAX_USERNAME_SIZE 32
#define MAX_PASSWORD_SIZE 32
#define MAX_PACKET_SIZE 257

#define MAX_USERS 100


#define MAX_GROUP_NAME_SIZE 32
#define MAX_MULTICAST_IP_SIZE 16    //"xxx.xxx.xxx.xxx\0" 16 bytes

//max number of groups that can be created 0-255,
//if more need to change FillMulticastIpQueue 
#define MAX_GROUPS 100              

#define DEFAULT_MULTICAST_PORT 5000
#define BUFFER_SIZE 1024

#endif // LIMITATION_H_