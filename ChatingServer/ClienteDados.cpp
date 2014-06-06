#include "ClienteDados.h"
#include "../ChatingDll/Dll.h"
#include <string>
#include "Shell.h"


ClienteDados::ClienteDados(const sTchar_t username, const sTchar_t password, int tipo, int id)
{
	this->username = username;
	this->password = password;
	this->tipo = tipo; // ToDo: talvez seja melhor criar enumerado
	this->online = false;
	this->id = id;
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

sTchar_t ClienteDados::GetUsername()const{
	return this->username;
}

sTchar_t ClienteDados::GetPassword()const{
	return this->password;
}

int ClienteDados::GetTipo()const{
	return this->tipo;
}

void ClienteDados::CreatePrivatePipe(){
	oTcharStream_t pipeName;
	pipeName << TEXT("\\\\.\\pipe\\") << this->username << TEXT('\0');

	this->privatePipe = INVALID_HANDLE_VALUE;
	this->privatePipe = CreateNamedPipe(pipeName.str().c_str(),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE |
		PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		sizeof(MSG_T)* 50,
		sizeof(MSG_T)* 50,
		0,
		NULL);

	if (this->privatePipe == INVALID_HANDLE_VALUE)
		_tprintf(TEXT("\nErro na criacao do pipe: %d\n"), GetLastError());
}

int ClienteDados::GetId()const{
	return this->id;
}

HANDLE ClienteDados::GetPipe()const{
	return this->privatePipe;
}

bool ClienteDados::GetIsOnline()const{
	return this->online;
}


