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
sTchar_t Mensagens::GetMensagem()const{
	return this->msg;
}
const DATA& Mensagens::GetDataMensagem(){
	return this->data;
}

int Mensagens::GetReceiver()const{
	return this->user1;
}
int Mensagens::GetSender()const{
	return this->user2;
}