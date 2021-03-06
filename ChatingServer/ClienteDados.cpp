#include "ClienteDados.h"
#include "../ChatingDll/Dll.h"
#include <string>
#include "Shell.h"
#include "Security.h"

ClienteDados::ClienteDados(const sTchar_t username, const sTchar_t password, int tipo, int id)
{
	this->username = username;
	this->password = password;
	this->tipo = tipo; // 1-Normal, 2-Admin
	this->online = false;
	this->id = id;
	this->isBusy = false;
	this->sa = security::Seguranca();
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

sTchar_t ClienteDados::GetUsername() const {
	return this->username;
}

sTchar_t ClienteDados::GetPassword() const {
	return this->password;
}

int ClienteDados::GetTipo() const {
	return this->tipo;
}

bool ClienteDados::getIsAdmin() const {
	return this->tipo == 2;
}

void ClienteDados::CreatePrivatePipe() {
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
		this->getSecurity());

	if (this->privatePipe == INVALID_HANDLE_VALUE)
		_tprintf(TEXT("\nErro na criacao do pipe: %d\n"), GetLastError());
}

SECURITY_ATTRIBUTES* ClienteDados::getSecurity() {
	return &this->sa;
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

bool ClienteDados::GetIsBusy() const{
	return this->isBusy;
}

void ClienteDados::SetIsBusy(const bool isbusy){
	this->isBusy = isbusy;
}