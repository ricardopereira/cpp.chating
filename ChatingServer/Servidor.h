#pragma once
#include <vector>
#include "Mensagens.h"
#include "ClienteDados.h"
#include "Semaforo.h"
#include "Mutex_t.h"

class Servidor
{
private:
	Semaforo					sem_ServerData;
	Mutex_t						mut_ServerData;
	std::vector<Mensagens*>		msgs;
	std::vector<ClienteDados*>	clientes;
public:
	enum rMsg {
		USER_NOT_REGISTERED,
		USER_NOT_FOUND,
		INCORRECT_PASSWORD,
		SUCCESS,
		SUCCESS_ADMIN,
		NO_PRIVILEDGES,
		USER_BUSY,
		PIPE_ERROR,
		REGEDIT_OK,
		REGEDIT_FIRST_TIME, 
		REGEDIT_NOT_OK
	};
	void NovaMensagem(DATA data, int user1, int user2, sTchar_t msg);
	void LoadRegistry();
	rMsg Login(sTchar_t username, sTchar_t password, ClienteDados* cliente);
	rMsg RegisterUser(sTchar_t username, sTchar_t password, int type);
	rMsg LancarChat(sTchar_t username, ClienteDados* partner);
	rMsg SendPrivateMessage(ClienteDados &partner);
	rMsg SendPublicMessage();
	rMsg CloseChat();
	rMsg RetrieveInformation();
	rMsg RemoveUser(sTchar_t username);

	Servidor();
	~Servidor();

	
};

