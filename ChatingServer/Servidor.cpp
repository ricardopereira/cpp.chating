#include "Servidor.h"


Servidor::Servidor()
{
}


Servidor::~Servidor()
{
//apagar memoria alocada dinamicamente
}

void Servidor::NovaMensagem(DATA data, int user1, int user2, sTchar_t msg){
	this->msgs.push_back(new Mensagens(data, user1, user2, msg));
}

