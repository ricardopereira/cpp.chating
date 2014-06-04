#include "ClienteDados.h"
#include "../ChatingDll/Dll.h"
#include <string>
#include "Shell.h"


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
	pipeName << TEXT("\\\.\\pipe\\") << this->username;
	
	this->privatePipe = INVALID_HANDLE_VALUE;
	this->privatePipe = CreateNamedPipe(pipeName.str().c_str(),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE |
		PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		sizeof(MSG_T),
		sizeof(MSG_T),
		0,
		NULL);

	if (this->privatePipe == INVALID_HANDLE_VALUE)
		_tprintf(TEXT("\nErro na criacao do pipe: Erro no (%d)\n"), GetLastError());
}
