#pragma once

#include "ChatBox.h"
#include "comunicacao.h"

class ChatBoxItem
{
private:
	sTchar_t username;
	sTchar_t message;
	sTchar_t info;
	int isOwner;
public:
	ChatBoxItem(sTchar_t username, sTchar_t message, sTchar_t info, int isOwner);
	~ChatBoxItem();

	virtual void doPaint(HDC hdc, HWND hWnd, ChatBox& chat, int index);

	sTchar_t getUsername();
	sTchar_t getMessage();
	sTchar_t getInfo();
	int getIsOwner();
};