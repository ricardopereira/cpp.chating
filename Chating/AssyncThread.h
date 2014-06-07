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
	Server& server;
	ChatBox& messageArea;
public:
	AssyncThread(sTchar_t username, Server& server, ChatBox& messageArea);
	~AssyncThread();
	virtual DWORD WINAPI funcaoThread();
};

