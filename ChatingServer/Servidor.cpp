#include "Servidor.h"
#include "Registry.h"


Servidor::Servidor()
{
}

Servidor::~Servidor()
{
//apagar memoria alocada dinamicamente
}

void Servidor::NovaMensagem(DATA data, int user1, int user2, sTchar_t msg) { //apgar
	this->sem_ServerData.Wait();
	this->mut_ServerData.Wait();

	this->msgs.push_back(new Mensagens(data, user1, user2, msg));

	this->mut_ServerData.Release();
	this->sem_ServerData.Release();
}

void Servidor::LoadRegistry() {
	Registry::LoadData(this->clientes, this->msgs);
}

Servidor::rMsg Servidor::Login(sTchar_t username, sTchar_t password, int* pos) { 
	this->sem_ServerData.Wait();
	this->mut_ServerData.Wait();

	for (unsigned int i = 0; i < clientes.size(); i++) {
		if (clientes.at(i)->GetUsername() == username) {
			if (clientes.at(i)->GetPassword() == password) {
				
				clientes.at(i)->SetOnline();
				*pos = i;
				if (clientes.at(i)->GetTipo() == 2) {
					clientes.at(i)->CreatePrivatePipe();
					this->mut_ServerData.Release();
					this->sem_ServerData.Release();
					return Servidor::SUCCESS_ADMIN;
				}
				else{
					clientes.at(i)->CreatePrivatePipe();
					this->mut_ServerData.Release();
					this->sem_ServerData.Release();
					return Servidor::SUCCESS;
				}
			}
			else{
				this->mut_ServerData.Release();
				this->sem_ServerData.Release();
				return Servidor::INCORRECT_PASSWORD;
			}
		}
	}
	
	this->mut_ServerData.Release();
	this->sem_ServerData.Release();
	return Servidor::USER_NOT_FOUND;
}

Servidor::rMsg Servidor::RegisterUser(sTchar_t username, sTchar_t password, int type) {
	this->sem_ServerData.Wait();
	this->mut_ServerData.Wait();

	clientes.push_back(new ClienteDados(username, password, type, this->clientes.size()));

	this->mut_ServerData.Release();
	this->sem_ServerData.Release();

	//Limitar a adi��o de novos utilizadores?
	return Servidor::SUCCESS;
}

Servidor::rMsg Servidor::LancarChat(sTchar_t username, ClienteDados* partner) {
	this->sem_ServerData.Wait();
	this->mut_ServerData.Wait();

	for (unsigned int i = 0; i < clientes.size(); i++) {
		if (clientes.at(i)->GetUsername() == username) {
			partner = clientes.at(i);
			return Servidor::SUCCESS;
		}
	}

	this->mut_ServerData.Release();
	this->sem_ServerData.Release();
	partner = nullptr;
	return Servidor::USER_NOT_FOUND;
}

Servidor::rMsg Servidor::SendPrivateMessage(ClienteDados &partner) {
	this->sem_ServerData.Wait();
	this->mut_ServerData.Wait();

	this->mut_ServerData.Release();
	this->sem_ServerData.Release();

	return Servidor::SUCCESS;
	return Servidor::PIPE_ERROR;
}

Servidor::rMsg Servidor::SendUsers(ClienteDados* currentClient)
{
	if (!currentClient) 
		return Servidor::USER_NOT_FOUND;

	this->sem_ServerData.Wait();
	this->mut_ServerData.Wait();
	
	MSG_T buffer[50];
	buffer[0].nMessages = 0;
	buffer[0].messageType = LIST_ALL_USERS;

	ClienteDados* itemClient;

	for (unsigned int i = 0; i < this->clientes.size(); i++)
	{
		itemClient = clientes.at(i);

		// Limite
		if (buffer[0].nMessages == BUFFER_RECORDS)
			break;

		_tcscpy_s(buffer[buffer[0].nMessages].utilizador, itemClient->GetUsername().size()*sizeof(TCHAR), itemClient->GetUsername().c_str());
		// Total de utilizadores
		buffer[0].nMessages++;
	}

	this->SendToClient(buffer, currentClient->GetPipe());

	this->mut_ServerData.Release();
	this->sem_ServerData.Release();

	return Servidor::SUCCESS;
}

Servidor::rMsg Servidor::SendUsersOnline(ClienteDados* currentClient)
{
	if (!currentClient) 
		return Servidor::USER_NOT_FOUND;
	
	this->sem_ServerData.Wait();
	this->mut_ServerData.Wait();
	
	MSG_T buffer[50];
	buffer[0].nMessages = 0;
	buffer[0].messageType = LIST_USERS_ONLINE;

	ClienteDados* itemClient;

	for (unsigned int i = 0; i < this->clientes.size(); i++)
	{
		itemClient = clientes.at(i);

		// Limite
		if (buffer[0].nMessages == BUFFER_RECORDS)
			break;

		if (itemClient->GetIsOnline()) {
			_tcscpy_s(buffer[buffer[0].nMessages].utilizador, itemClient->GetUsername().size()*sizeof(TCHAR), itemClient->GetUsername().c_str());
			// Total de utilizadores
			buffer[0].nMessages++;
		}
	}

	this->SendToClient(buffer, currentClient->GetPipe());

	this->mut_ServerData.Release();
	this->sem_ServerData.Release();

	return Servidor::SUCCESS;
}

Servidor::rMsg Servidor::SendUserGoOnline(ClienteDados* cliente)
{
	if (!cliente) 
		return Servidor::USER_NOT_FOUND;

	this->sem_ServerData.Wait();
	this->mut_ServerData.Wait();
	
	MSG_T buffer[1];
	buffer[0].nMessages = 0;
	buffer[0].messageType = typeMessages::USER_ONLINE;
	_tcscpy_s(buffer[0].utilizador, cliente->GetUsername().size()*sizeof(TCHAR), cliente->GetUsername().c_str());

	// Broadcast
	for (unsigned int i = 0; i < this->clientes.size(); i++)
	{
		if (this->clientes.at(i)->GetIsOnline()){
			this->SendToClient(buffer, clientes.at(i)->GetPipe());
		}
	}

	this->SendToClient(buffer, cliente->GetPipe());

	this->mut_ServerData.Release();
	this->sem_ServerData.Release();

	return Servidor::SUCCESS;
}

Servidor::rMsg Servidor::SendUserGoOffline(ClienteDados* cliente)
{
	if (!cliente) 
		return Servidor::USER_NOT_FOUND;

	this->sem_ServerData.Wait();
	this->mut_ServerData.Wait();
	
	MSG_T buffer[1];
	buffer[0].nMessages = 0;
	buffer[0].messageType = typeMessages::USER_OFFLINE;
	_tcscpy_s(buffer[0].utilizador, cliente->GetUsername().size()*sizeof(TCHAR), cliente->GetUsername().c_str());

	// Broadcast
	for (unsigned int i = 0; i < this->clientes.size(); i++)
	{
		if (this->clientes.at(i)->GetIsOnline()){
			this->SendToClient(buffer, clientes.at(i)->GetPipe());
		}
	}

	this->SendToClient(buffer, cliente->GetPipe());

	this->mut_ServerData.Release();
	this->sem_ServerData.Release();

	return Servidor::SUCCESS;
}

Servidor::rMsg Servidor::SendPublicMessage(sTchar_t message, sTchar_t owner, ClienteDados* cliente){
	this->sem_ServerData.Wait();
	this->mut_ServerData.Wait();

	// Instante atual
	SYSTEMTIME hora;
	GetSystemTime(&hora);
	DATA dataActual;
	dataActual.ano = hora.wYear;
	dataActual.dia = hora.wDay;
	dataActual.mes = hora.wMonth;
	dataActual.hora = hora.wHour;
	dataActual.minuto = hora.wMinute;
	dataActual.segundo = hora.wSecond;
	
	// Guarda mensagem
	this->msgs.push_back(new Mensagens(dataActual, cliente->GetId(), -1, message));
	
	// Prepara mensagem
	MSG_T buffer[50];
	buffer[0].mensagem.instante = dataActual;
	_tcscpy_s(buffer[0].mensagem.texto, message.size() *sizeof(TCHAR), message.c_str());
	buffer[0].messageType = PUBLIC_MESSAGE;
	buffer[0].nMessages = 1;
	_tcscpy_s(buffer[0].utilizador, owner.size()*sizeof(TCHAR), owner.c_str());
	
	// Broadcast
	for (unsigned int i = 0; i < this->clientes.size(); i++)
	{
		if (this->clientes.at(i)->GetIsOnline()){
			this->SendToClient(buffer, clientes.at(i)->GetPipe());
		}
	}

	this->mut_ServerData.Release();
	this->sem_ServerData.Release();

	return Servidor::SUCCESS;
}

Servidor::rMsg Servidor::CloseChat() {
	this->sem_ServerData.Wait();
	this->mut_ServerData.Wait();

	this->mut_ServerData.Release();
	this->sem_ServerData.Release();

	return Servidor::SUCCESS;
}

Servidor::rMsg Servidor::RetrieveInformation() {
	this->sem_ServerData.Wait();
	this->mut_ServerData.Wait();

	this->mut_ServerData.Release();
	this->sem_ServerData.Release();

	return Servidor::SUCCESS;
}

Servidor::rMsg Servidor::RemoveUser(sTchar_t username) {
	this->sem_ServerData.Wait();
	this->mut_ServerData.Wait();

	for (unsigned int i = 0; i < clientes.size(); i++) {
		if (clientes.at(i)->GetUsername() == username) {
			//terminar
			return Servidor::SUCCESS;
		}
	}

	this->mut_ServerData.Release();
	this->sem_ServerData.Release();

	return Servidor::USER_NOT_FOUND;
}

int Servidor::getUserCount()
{
	return clientes.size();
}

int Servidor::getUserOnlineCount()
{
	int count = 0;
	for (unsigned int i = 0; i < clientes.size(); i++)
		if (clientes.at(i)->GetIsOnline())
			count++;
	return count;
}

int Servidor::SendToClient(MSG_T* buffer, HANDLE hPipe){
	
	DWORD leituraEscritaSucesso;
	DWORD bytesEscritos;
	
	leituraEscritaSucesso = WriteFile(hPipe,
		buffer, //message
		sizeof(MSG_T)*50, //message length
		&bytesEscritos, //bytes written
		NULL); //not overlapped

	return 1;
}

ClienteDados* Servidor::getClientData(int& pos){

	return this->clientes.at(pos);
}