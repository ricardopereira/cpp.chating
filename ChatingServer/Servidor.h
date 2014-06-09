#pragma once

#include <vector>
#include "Mensagens.h"
#include "ClienteDados.h"
#include "Semaforo.h"
#include "Mutex_t.h"
#include "ChatComunication.h"

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

	Servidor();
	~Servidor();

	void LoadRegistry();
	void SaveRegistry();
	void Shutdown();
	rMsg Login(sTchar_t username, sTchar_t password, int* pos);
	rMsg Logout(sTchar_t username);
	rMsg RegisterUser(sTchar_t username, sTchar_t password, int type);
	rMsg RemoveUser(sTchar_t username);
	bool ExistUser(sTchar_t username);
	rMsg ShutdownUser(sTchar_t username);
	rMsg LancarChat(sTchar_t username, ClienteDados* partner);
	rMsg SendPrivateMessage(ClienteDados &partner);
	rMsg SendPublicMessage(sTchar_t message, sTchar_t owner, ClienteDados* cliente);
	rMsg SendUsers(ClienteDados* currentClient);
	rMsg SendUsersOnline(ClienteDados* currentClient);
	rMsg UserGoOnline(ClienteDados* cliente);
	rMsg UserGoOffline(ClienteDados* cliente);
	rMsg CloseChat();
	rMsg RetrieveInformation(ClienteDados* currentClient);
	ClienteDados* getClientData(int& pos);

	int getUserCount();
	int getUserOnlineCount();
};