#include "ClienteDados.h"


ClienteDados::ClienteDados(sTchar_t username, sTchar_t password, int tipo)
{
	this->username = username;
	this->password = password;
	this->tipo = tipo;
	this->online = false;
}


ClienteDados::~ClienteDados()
{
}


void ClienteDados::SetOnline(){
	this->online = true;
}

void ClienteDados::SetOffline(){
	this->online = false;
}


sTchar_t ClienteDados::GetUsername(){
	return this->username;
}
sTchar_t ClienteDados::GetPassword(){
	return this->password;
}