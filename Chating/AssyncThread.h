#pragma once
#include "../Core/Thread.h"
#include "Common.h"
#include "ChatBox.h"
#include "ListBox.h"

class AssyncThread :
	public Thread
{
private:
	sTchar_t pipeName;
	ListBox &listUsers;
	ChatBox &messageArea;
public:
	AssyncThread(sTchar_t username, ChatBox& messageArea, ListBox& listUsers);
	~AssyncThread();
	virtual DWORD WINAPI funcaoThread();
};

