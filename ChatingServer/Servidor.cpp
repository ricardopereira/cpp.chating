#include "Servidor.h"
#include "Registry.h"


Servidor::Servidor()
{
}

Servidor::~Servidor()
{
//apagar memoria alocada dinamicamente
}



void Servidor::LoadRegistry() {
	this->sem_ServerData.Wait();
	this->mut_ServerData.Wait();
	Registry::LoadData(this->clientes, this->msgs);
	this->mut_ServerData.Release();
	this->sem_ServerData.Release();
}

void Servidor::SaveRegistry(){
	this->sem_ServerData.Wait();
	this->mut_ServerData.Wait();
	
	Registry::SaveData(this->clientes, this->msgs);
	
	this->mut_ServerData.Release();
	this->sem_ServerData.Release();
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

	//Limitar a adição de novos utilizadores?
	return Servidor::SUCCESS;
}

Servidor::rMsg Servidor::LancarChat(sTchar_t username, ClienteDados* partner) {
	this->mut_ServerData.Wait();
	this->sem_ServerData.Wait();

	for (unsigned int i = 0; i < clientes.size(); i++) {
		if (clientes.at(i)->GetUsername() == username) {
			partner = clientes.at(i);
			return Servidor::SUCCESS;
		}
	}

	this->sem_ServerData.Release();
	this->mut_ServerData.Release();
	partner = nullptr;
	return Servidor::USER_NOT_FOUND;
}

Servidor::rMsg Servidor::SendPrivateMessage(ClienteDados &partner) {
	this->mut_ServerData.Wait();
	this->sem_ServerData.Wait();

	this->sem_ServerData.Release();
	this->mut_ServerData.Release();

	return Servidor::SUCCESS;
	return Servidor::PIPE_ERROR;
}

Servidor::rMsg Servidor::SendPublicMessage(sTchar_t message, sTchar_t owner, ClienteDados* cliente){
	this->sem_ServerData.Wait();
	this->mut_ServerData.Wait();
	SYSTEMTIME hora;
	GetSystemTime(&hora);
	DATA dataActual;
	dataActual.ano = hora.wYear;
	dataActual.dia = hora.wDay;
	dataActual.mes = hora.wMonth;
	dataActual.hora = hora.wHour;
	dataActual.minuto = hora.wMinute;
	dataActual.segundo = hora.wSecond;
	

	this->msgs.push_back(new Mensagens(dataActual, cliente->GetId(), -1, message));
	
	MSG_T buffer[50];
	buffer[0].mensagem.instante = dataActual;
	_tcscpy_s(buffer[0].mensagem.texto, message.size() *sizeof(TCHAR), message.c_str());
	buffer[0].messageType = PUBLIC_MESSAGE;
	buffer[0].nMessages = 1;
	_tcscpy_s(buffer[0].utilizador, owner.size()*sizeof(TCHAR), owner.c_str());
	
	for (unsigned int i = 0; i < this->clientes.size(); i++)
	{
		if (this->clientes.at(i)->GetIsOnline()){
			this->SendToClient(buffer, clientes.at(i)->GetPipe());
		}
	}
	

	this->mut_ServerData.Release();
	this->sem_ServerData.Release();

	return Servidor::SUCCESS;
	return Servidor::PIPE_ERROR;
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
	std::vector<Mensagens*> publicMessages;
	for (unsigned int i = 0; i < this->msgs.size(); i++)
	{
		if (this->msgs.at(i)->GetReceiver() == -1) { //-1 => mensagens públicas 
			publicMessages.push_back(this->msgs.at(i));
		}

	
	}
	int start = 0;
	MSG_T buffer[50];
	
	if (publicMessages.size() > 50){
		start = publicMessages.size() - 50; //Devolver apenas as últimas 50 mensagens
		buffer[0].nMessages = 50;
	}
	else{
		buffer[0].nMessages = publicMessages.size();
	}
	int k = 0;
	for (unsigned int i = start; i < publicMessages.size(); i++)
	{
		for (unsigned int j = 0; j < this->clientes.size(); j++)
		{
			if (publicMessages.at(i)->GetSender() == this->clientes.at(j)->GetId()){
				
				_tcscpy_s(buffer[k].utilizador, this->clientes.at(j)->GetUsername().size()*sizeof(TCHAR), this->clientes.at(j)->GetUsername().c_str());
				break;
			}
		}
		buffer[k].mensagem.instante = publicMessages.at(i)->GetDataMensagem();
		_tcscpy_s(buffer[k].mensagem.texto, publicMessages.at(i)->GetMensagem.size() *sizeof(TCHAR), publicMessages.at(i)->GetMensagem.c_str());
		buffer[k].messageType = PUBLIC_MESSAGE;


		k++;
	}
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