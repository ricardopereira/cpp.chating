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

	enum commands_t {
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
		ELIMINAR_UTILIZADOR
	};
};

