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

	void forceRefresh();
public:
	AssyncThread(sTchar_t username, Controller& controller, ChatBox& messageArea, ListBox& listUserOnline);
	~AssyncThread();
	virtual DWORD WINAPI funcaoThread();
};

