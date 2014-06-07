#include "ChatUser.h"

ChatUser::ChatUser(const TCHAR *username) : username(username)
{

}

ChatUser::~ChatUser()
{

}

void ChatUser::setOnline()
{
	this->online = true;
}

void ChatUser::setOffline()
{
	this->online = false;
}

sTchar_t ChatUser::getUsername() const
{
	return this->username;
}

int ChatUser::getTipo() const
{
	return this->type;
}

bool ChatUser::getIsOnline() const
{
	return this->online;
}

bool ChatUser::getIsAdmin() const
{
	return this->admin;
}

void ChatUser::setAdmin()
{
	this->admin = true;
}