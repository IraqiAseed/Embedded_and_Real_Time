#ifndef MULTICAST_H_
#define MULTICAST_H_

#include <stddef.h>

int MulticastCreateReceiverSocket(const char *_multicastIp, int _port);
int MulticastCreateSenderSocket(void);

int MulticastSendMessage(int _sock,
                         const char *_multicastIp,
                         int _port,
                         const char *_message);

int MulticastReceiveMessage(int _sock,
                            char *_buffer,
                            size_t _bufferSize);

void RunMulticastReceiver(const char *_multicastIp, int _port);
void RunMulticastSender(const char *_multicastIp, int _port,const char *_username);

#endif