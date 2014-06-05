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
	DWORD bytesEscritos = 0;
	int temp_command; //substituir pelo campo correspondente conforme a estrutura.
	sTchar_t pass = TEXT(""); //temp
	sTchar_t usrname = TEXT(""); //temp
	sTchar_t usrnametoremove = TEXT(""); //temp
	int result;
	
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
				tcout << TEXT("ThreadCliente: Ocorreu um erro de leitura.\n");
			}
			break;
		}

		switch (buffer.command){
			//Cada acc��o deve devolver um c�digo de (in)sucesso
		case ThreadCliente::REGISTER_NEW_USER:
			buffer.arg_num = server->RegisterUser(buffer.args[0], buffer.args[1], /*tipo*/1);
			break;
		case ThreadCliente::LOGIN:
			tcout << TEXT("\nThreadCliente: Login: ") << buffer.args[0] << endl << TEXT("Password: ") << buffer.args[1] << TEXT("\n");
			result = server->Login(buffer.args[0], buffer.args[1], this->currentClient);

			if (result == Servidor::USER_NOT_FOUND && server->getUserCount() == 0)
				tcout << TEXT("ThreadCliente: n�o foi poss�vel criar o registo");

			buffer.arg_num = result;
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

		leituraEscritaSucesso = WriteFile(hPipe,
			&buffer, //message
			sizeof(chatbuffer_t), //message length
			&bytesEscritos, //bytes written
			NULL); //not overlapped
	}
	
	return 1;
}

