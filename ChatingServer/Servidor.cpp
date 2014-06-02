#include "Servidor.h"
#include "Registry.h"


Servidor::Servidor()
{
}

Servidor::~Servidor()
{
//apagar memoria alocada dinamicamente
}

void Servidor::NovaMensagem(DATA data, int user1, int user2, sTchar_t msg){
	this->mut_ServerData.Wait();
	this->sem_ServerData.Wait();
	this->msgs.push_back(new Mensagens(data, user1, user2, msg));
	this->sem_ServerData.Release();
	this->mut_ServerData.Release();
	
}
void Servidor::LoadRegistry(){
	Registry::LoadData(this->clientes, this->msgs);
}

Servidor::rMsg Servidor::Login(sTchar_t username, sTchar_t password, ClienteDados* cliente){
	this->mut_ServerData.Wait();
	this->sem_ServerData.Wait();
	for (unsigned int i = 0; i < clientes.size(); i++){
		if (clientes.at(i)->GetUsername() == username){
			if (clientes.at(i)->GetPassword() == password){
				cliente = clientes.at(i);
				if (cliente->GetTipo() == 2){
					cliente->CreatePrivatePipe();
					return Servidor::SUCCESS_ADMIN;
				}
				else{
					cliente->CreatePrivatePipe();
					return Servidor::SUCCESS;
				}
			}
			else{
				cliente = nullptr;
				return Servidor::INCORRECT_PASSWORD;
			}
		}
	}
	this->sem_ServerData.Release();
	this->mut_ServerData.Release();
	cliente = nullptr;
	return Servidor::USER_NOT_FOUND;
}

Servidor::rMsg Servidor::RegisterUser(sTchar_t username, sTchar_t password, int type){
	this->mut_ServerData.Wait();
	this->sem_ServerData.Wait();
	clientes.push_back(new ClienteDados(username, password, type));

	this->sem_ServerData.Release();
	this->mut_ServerData.Release();
	//Limitar a adição de novos utilizadores?
	return Servidor::SUCCESS;
}

Servidor::rMsg Servidor::LancarChat(sTchar_t username, ClienteDados* partner){
	this->mut_ServerData.Wait();
	this->sem_ServerData.Wait();
	for (unsigned int i = 0; i < clientes.size(); i++){
		if (clientes.at(i)->GetUsername() == username){
			partner = clientes.at(i);
			return Servidor::SUCCESS;
		}
		
	}
	this->sem_ServerData.Release();
	this->mut_ServerData.Release();
	partner = nullptr;
	return Servidor::USER_NOT_FOUND;
}


Servidor::rMsg Servidor::SendPrivateMessage(ClienteDados &partner){
	this->mut_ServerData.Wait();
	this->sem_ServerData.Wait();

	this->sem_ServerData.Release();
	this->mut_ServerData.Release();

	return Servidor::SUCCESS;
	return Servidor::PIPE_ERROR;
}
Servidor::rMsg Servidor::SendPublicMessage(){
	this->mut_ServerData.Wait();
	this->sem_ServerData.Wait();

	this->sem_ServerData.Release();
	this->mut_ServerData.Release();

	return Servidor::SUCCESS;
	return Servidor::PIPE_ERROR;
}

Servidor::rMsg Servidor::CloseChat(){
	this->mut_ServerData.Wait();
	this->sem_ServerData.Wait();

	this->sem_ServerData.Release();
	this->mut_ServerData.Release();

	return Servidor::SUCCESS;
}

Servidor::rMsg Servidor::RetrieveInformation(){
	this->mut_ServerData.Wait();
	this->sem_ServerData.Wait();

	this->sem_ServerData.Release();
	this->mut_ServerData.Release();
	return Servidor::SUCCESS;
}

Servidor::rMsg Servidor::RemoveUser(sTchar_t username){
	this->mut_ServerData.Wait();
	this->sem_ServerData.Wait();
	for (unsigned int i = 0; i < clientes.size(); i++){
		if (clientes.at(i)->GetUsername() == username){
			//terminar
			return Servidor::SUCCESS;
		}

	}
	this->sem_ServerData.Release();
	this->mut_ServerData.Release();
	return Servidor::USER_NOT_FOUND;
}

