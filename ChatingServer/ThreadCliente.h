#pragma once

#include "Thread.h"
#include "Servidor.h"
#include "../Logic/ChatComunication.h"

class ThreadCliente: public Thread
{
private:
	HANDLE hPipe;
	Servidor* server;
	ClienteDados* currentClient;
	ClienteDados* currentPartner;

	sTchar_t getInfo();
public:
	ThreadCliente(HANDLE hPipe, Servidor* server);
	~ThreadCliente();
	virtual DWORD WINAPI funcaoThread();
};

