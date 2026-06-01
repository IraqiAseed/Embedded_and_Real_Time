#ifndef CLIENT_CHAT_H_
#define CLIENT_CHAT_H_

int StartChatSession(const char *_groupName, const char *_ip, int _port);
int StopChatSession(const char *_groupName);
void StopAllChatSessions(void);

#endif