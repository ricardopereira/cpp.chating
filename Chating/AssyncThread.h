#pragma once
#include "../Core/Thread.h"
#include "../ChatingServer/Common.h"
#include "ChatBox.h"
#include "ListBox.h"

class AssyncThread :
	public Thread
{
private:
	sTchar_t pipeName;
	Controller& controller;
	ChatBox& messageArea;
	ListBox& listUserOnline;

	ChatBox* privateMessageArea;
	HWND privateWindowHandle;
	void forceRefresh();
public:
	AssyncThread(sTchar_t username, Controller& controller, ChatBox& messageArea, ListBox& listUserOnline);
	void setPrivateMessageArea(ChatBox &privateMessageArea);
	void setPrivateWindowHandle(HWND hWnd);
	~AssyncThread();
	virtual DWORD WINAPI funcaoThread();
};

