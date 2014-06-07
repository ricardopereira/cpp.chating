#include "Shell.h"
#include "ThreadCliente.h"
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
				tcout << TEXT("ThreadCliente: cliente desligou-se.\n");
				//contador--; //possiveis erros
				//TERMINAR:: CloseHandle(
			}
			else
			{
				tcout << TEXT("ThreadCliente: ocorreu um erro de leitura.\n");
			}
			break;
		}

		switch (buffer.command){
			//Cada accção deve devolver um código de (in)sucesso
		case commands_t::REGISTER_NEW_USER:
			buffer.arg_num = server->RegisterUser(buffer.args[0], buffer.args[1], /*tipo*/1);
			break;
		case commands_t::LOGIN:
			tcout << TEXT("\nThreadCliente: Login: ") << buffer.args[0] << endl << TEXT("Password: ") << buffer.args[1] << TEXT("\n") << endl;
			int pos;
			result = server->Login(buffer.args[0], buffer.args[1], &pos);
			if (result == Servidor::SUCCESS || result == Servidor::SUCCESS_ADMIN)
				this->currentClient = server->getClientData(pos);

			if (result == Servidor::USER_NOT_FOUND && server->getUserCount() == 0)
				tcout << TEXT("ThreadCliente: nao foi possivel criar o registo por defeito") << endl;

			buffer.arg_num = result;
			break;
		case commands_t::LISTA_UTILIZADORES_TODOS:
			buffer.arg_num = server->getUserCount();
			break;
		case commands_t::LISTA_UTILIZADORES_ONLINE:
			buffer.arg_num = server->getUserOnlineCount();
			//server->RetrieveInformation(); //NOPE NOPE NOPE
			break;
		case commands_t::LANCAR_CHAT:
			server->LancarChat(usrname, this->currentPartner);
			break;
		case commands_t::ENVIAR_MSG_PRIVADA:
			server->SendPrivateMessage(*this->currentPartner);
			break;
		case commands_t::ENVIAR_MSG_PUBLICA:

			tcout << TEXT("\nThreadCliente: Recebeu mensagem: ") << buffer.args[0] << endl;
			server->SendPublicMessage(buffer.args[0], buffer.args[1],this->currentClient);
			break;
		case commands_t::FECHAR_CHAT:
			server->CloseChat();
			break;
		case commands_t::LER_INFO_INICIAL:
			server->RetrieveInformation();
			break;
		case commands_t::LER_MENSAGEM_PUBLICA:
			break;
		case commands_t::LER_MENSAGEM_PRIVADA:
			break;
		case commands_t::ELIMINAR_UTILIZADOR:
			server->RemoveUser(usrnametoremove);
			break;
		case commands_t::LOGOUT:
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
