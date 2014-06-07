#include "Shell.h"
#include "ThreadSaveRegistry.h"
#include "Servidor.h"
#include "Mensagens.h"

ThreadSaveRegistry::ThreadSaveRegistry(Servidor& server)
{
	this->ptrClasse = this;
	this->server = &server;
}

ThreadSaveRegistry::~ThreadSaveRegistry()
{

}

DWORD WINAPI ThreadSaveRegistry::funcaoThread() {
	DWORD time = 5000;
	
	while (1){
		Sleep(time);
		this->server->SaveRegistry();
	}
}
