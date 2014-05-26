#include "Mensagens.h"


Mensagens::Mensagens(DATA data, int user1, int user2, sTchar_t msg)
{
	this->data = data;
	this->user1 = user1;
	this->user2 = user2;
	this->msg = msg;
}


Mensagens::~Mensagens()
{
}
sTchar_t Mensagens::GetMensagem(){
	return this->msg;
}
const DATA& Mensagens::GetDataMensagem(){
	return this->data;
}

