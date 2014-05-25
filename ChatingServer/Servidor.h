#pragma once
#include <vector>
#include "Mensagens.h"
#include "ClienteDados.h"

class Servidor
{
	std::vector<Mensagens*> msgs;
	std::vector<ClienteDados*> clientes;
public:
	enum rMsg {
		USER_NOT_REGISTERED,
		USER_NOT_FOUND,
		INCORRECT_PASSWORD,
		SUCCESS,
		NO_PRIVILEDGES,
		USER_BUSY,
		PIPE_ERROR
	};
	void NovaMensagem(DATA data, int user1, int user2, sTchar_t msg);
	rMsg Login(sTchar_t username, sTchar_t password, ClienteDados* cliente);
	rMsg RegisterUser(sTchar_t username, sTchar_t password);
	rMsg LancarChat(sTchar_t username, ClienteDados* partner);
	rMsg SendPrivateMessage(ClienteDados &partner);
	rMsg SendPublicMessage();
	rMsg CloseChat();
	rMsg RetrieveInformation();
	rMsg RemoveUser(sTchar_t username);

	Servidor();
	~Servidor();

	
};

