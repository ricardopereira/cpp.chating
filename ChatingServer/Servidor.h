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
		NO_PRIVILEDGES
	};
	void NovaMensagem(DATA data, int user1, int user2, sTchar_t msg);
	rMsg Login(sTchar_t username, sTchar_t password, ClienteDados* cliente);
	rMsg RegisterUser(sTchar_t username, sTchar_t password);
	Servidor();
	~Servidor();

	
};

