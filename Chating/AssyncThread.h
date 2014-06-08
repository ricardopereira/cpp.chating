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
public:
	AssyncThread(sTchar_t username, Controller& controller, ChatBox& messageArea);
	~AssyncThread();
	virtual DWORD WINAPI funcaoThread();
};

