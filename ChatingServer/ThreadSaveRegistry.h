#pragma once

#include "Thread.h"
#include "Servidor.h"
#include "../Logic/ChatComunication.h"

class ThreadSaveRegistry: public Thread
{
private:
	Servidor* server;
public:
	ThreadSaveRegistry(Servidor& server);
	~ThreadSaveRegistry();
	virtual DWORD WINAPI funcaoThread();
};

