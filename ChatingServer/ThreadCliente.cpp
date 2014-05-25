#include "Shell.h"
#include "ThreadCliente.h"
#include "ChatComunication.h"
#include "Servidor.h"
#include "Mensagens.h"
ThreadCliente::ThreadCliente(HANDLE hPipe, Servidor* server)
{
	this->ptrClasse = this;
	this->hPipe = hPipe;
	this->server = server;
}

ThreadCliente::~ThreadCliente()
{

}

DWORD WINAPI ThreadCliente::funcaoThread() {
	chatbuffer_t buffer;
	bool powerOff = false;
	BOOL leituraEscritaSucesso = false;
	DWORD bytesLidos = 0;
	int temp_command; //substituir pelo campo correspondente conforme a estrutura.
	sTchar_t pass = TEXT(""); //temp
	sTchar_t usrname = TEXT(""); //temp
	sTchar_t usrnametoremove = TEXT(""); //temp
	
	while (!powerOff) {
		
		leituraEscritaSucesso = ReadFile(
			hPipe,
			&buffer,
			sizeof(chatbuffer_t),
			&bytesLidos,
			NULL);

		if (!leituraEscritaSucesso || bytesLidos == 0)
		{
			if (GetLastError() == ERROR_BROKEN_PIPE)
			{
				tcout << TEXT("ThreadCliente: Cliente desligou-se.\n");
				//contador--; //possiveis erros
				//TERMINAR:: CloseHandle(
			}
			else
			{
				tcout << TEXT("ThreadAtendeCliente: Ocorreu um erro de leitura.\n");
			}
			break;
		}

		switch(temp_command){
		//Cada accção deve devolver um código de (in)sucesso
		case ThreadCliente::REGISTER_NEW_USER:
			server->RegisterUser(usrname, pass);
			break;
		case ThreadCliente::LOGIN:
			server->Login(usrname, pass, this->currentClient);
			break;
		case ThreadCliente::LANCAR_CHAT:
			server->LancarChat(usrname, this->currentPartner);
			break;
		case ThreadCliente::ENVIAR_MSG_PRIVADA:
			server->SendPrivateMessage(*this->currentPartner);
			break;
		case ThreadCliente::ENVIAR_MSG_PUBLICA:
			server->SendPublicMessage();
			break;
		case ThreadCliente::FECHAR_CHAT:
			server->CloseChat();
			break;
		case ThreadCliente::LER_INFO_INICIAL:
			server->RetrieveInformation();
			break;
		case ThreadCliente::LER_MENSAGEM_PUBLICA:
			break;
		case ThreadCliente::LER_MENSAGEM_PRIVADA:
			break;
		case ThreadCliente::ELIMINAR_UTILIZADOR:
			server->RemoveUser(usrnametoremove);
			break;
		case ThreadCliente::LOGOUT:
			break;
		}


	}
	
	return 1;
}

