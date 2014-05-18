#pragma once
#include <vector>
#include "Mensagens.h"
#include "ClienteDados.h"

class Servidor
{
	std::vector<Mensagens*> msgs;
	std::vector<ClienteDados*> clientes;
public:
	void NovaMensagem(DATA data, int user1, int user2, sTchar_t msg);
	Servidor();
	~Servidor();
};

