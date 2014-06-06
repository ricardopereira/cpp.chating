#include <iostream>

#include "../ChatingServer/Common.h"

#ifndef __CHATUSER_H__
#define __CHATUSER_H__

class ChatUser
{
	sTchar_t username;
	int type;
	bool online;
	bool admin;
	bool hasPrivateChat;

	ChatUser() {};
public:
	ChatUser(const TCHAR *username);
	virtual ~ChatUser();

	void setOnline();
	void setOffline();
	sTchar_t getUsername() const;
	int getTipo() const;
	bool getIsOnline() const;
	bool getIsAdmin() const;
	void setAdmin();
};

#endif