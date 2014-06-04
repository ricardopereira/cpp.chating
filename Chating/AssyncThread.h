#pragma once
#include "../Core/Thread.h"
#include "Common.h"

class AssyncThread :
	public Thread
{
private:
	sTchar_t pipeName;
public:
	AssyncThread(sTchar_t username);
	~AssyncThread();
	virtual DWORD WINAPI funcaoThread();
};

