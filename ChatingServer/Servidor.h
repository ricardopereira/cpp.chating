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
	int SendToClient(MSG_T* buffer , HANDLE hPipe);
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
	void LoadRegistry();
	void SaveRegistry();
	rMsg Login(sTchar_t username, sTchar_t password, int* pos);
	rMsg RegisterUser(sTchar_t username, sTchar_t password, int type);
	rMsg RemoveUser(sTchar_t username);
	rMsg LancarChat(sTchar_t username, ClienteDados* partner);
	rMsg SendPrivateMessage(ClienteDados &partner);
	rMsg SendPublicMessage(sTchar_t message, sTchar_t owner, ClienteDados* cliente);
	rMsg CloseChat();
	rMsg RetrieveInformation();
	ClienteDados* getClientData(int& pos);

	int getUserCount();
	int getUserOnlineCount();

	Servidor();
	~Servidor();

};