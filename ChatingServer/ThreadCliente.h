#pragma once

#include "Thread.h"
#include "Servidor.h"

class ThreadCliente: public Thread
{
private:
	HANDLE hPipe;
	Servidor* server;
	ClienteDados* currentClient;
	ClienteDados* currentPartner;
public:
	ThreadCliente(HANDLE hPipe, Servidor* server);
	~ThreadCliente();
	virtual DWORD WINAPI funcaoThread();

	enum commands_t {
		REGISTER_NEW_USER,
		LOGIN,
		LANCAR_CHAT,
		ENVIAR_MSG_PRIVADA,
		ENVIAR_MSG_PUBLICA,
		FECHAR_CHAT,
		LER_INFO_INICIAL,
		CRIAR_USER,
		DESLIGAR,
		LER_MENSAGEM_PRIVADA,
		LER_MENSAGEM_PUBLICA,
		ELIMINAR_UTILIZADOR,
		LOGOUT
	};
};

