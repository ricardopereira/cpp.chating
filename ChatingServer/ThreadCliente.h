#pragma once

#include "Thread.h"

class ThreadCliente: public Thread
{
private:
	HANDLE hPipe;
public:
	ThreadCliente(HANDLE hPipe);
	~ThreadCliente();
	virtual DWORD WINAPI funcaoThread();
};

