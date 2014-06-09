#include "Shell.h"
#include "ThreadCliente.h"
#include "Servidor.h"
#include "Mensagens.h"

ThreadCliente::ThreadCliente(HANDLE hPipe, Servidor* server)
{
	this->ptrClasse = this;
	this->hPipe = hPipe;
	this->server = server;
	this->currentClient = NULL;
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
				tcout << getInfo() << TEXT("disconnected") << endl;
			}
			else
			{
				tcout << getInfo() << TEXT("read error" << endl);
			}
			break;
		}

		switch (buffer.command){
			//Cada accção deve devolver um código de (in)sucesso
		case commands_t::CRIAR_UTILIZADOR:
			buffer.arg_num = server->RegisterUser(buffer.args[0], buffer.args[1], /*tipo*/1);
			break;
		case commands_t::ELIMINAR_UTILIZADOR:
			server->ShutdownUser(buffer.args[0]);
			buffer.arg_num = server->RemoveUser(buffer.args[0]);
			break;
		case commands_t::LOGIN:
			int pos;
			result = server->Login(buffer.args[0], buffer.args[1], &pos);

			// Login com sucesso
			if (result == Servidor::SUCCESS || result == Servidor::SUCCESS_ADMIN)
			{
				this->currentClient = server->getClientData(pos);
				server->UserGoOnline(this->currentClient);
				tcout << getInfo() << TEXT("logged in - ") << buffer.args[0] << endl;
			}
			else if (result == Servidor::USER_NOT_FOUND && server->getUserCount() == 0)
				tcout << getInfo() << TEXT("no users") << endl;

			buffer.arg_num = result;
			break;
		case commands_t::LOGOUT:

			if (server->ExistUser(buffer.args[0]))
			{
				result = server->Logout(this->currentClient->GetUsername());
			}
			else 
				this->currentClient = new ClienteDados(buffer.args[0],TEXT(""),1,-1); // Removido pelo Admin
			
			// Logout com sucesso
			if (result == Servidor::SUCCESS)
			{
				server->UserGoOffline(this->currentClient);
				tcout << getInfo() << TEXT("logged out - ") << this->currentClient->GetUsername() << endl;
				this->currentClient = NULL;
			}

			buffer.arg_num = result;
			break;
		case commands_t::LISTA_UTILIZADORES_TODOS:
			buffer.arg_num = server->getUserCount();
			server->SendUsers(this->currentClient);
			break;
		case commands_t::LISTA_UTILIZADORES_ONLINE:
			buffer.arg_num = server->getUserOnlineCount();
			server->SendUsersOnline(this->currentClient);
			break;
		case commands_t::LANCAR_CHAT:
			server->LancarChat(buffer.args[0], this->currentPartner);
			break;
		case commands_t::ENVIAR_MSG_PRIVADA:
			server->SendPrivateMessage(*this->currentPartner);
			break;
		case commands_t::ENVIAR_MSG_PUBLICA:
			tcout << getInfo() << TEXT("message: ") << buffer.args[0] << endl;
			server->SendPublicMessage(buffer.args[0], buffer.args[1], this->currentClient);
			break;
		case commands_t::FECHAR_CHAT:
			server->CloseChat();
			break;
		case commands_t::LER_INFO_INICIAL:
			server->RetrieveInformation(this->currentClient);
			break;
		}

		leituraEscritaSucesso = WriteFile(hPipe,
			&buffer, //message
			sizeof(chatbuffer_t), //message length
			&bytesEscritos, //bytes written
			NULL); //not overlapped

		if (buffer.command == commands_t::LOGOUT)
			powerOff = true;
	}

	// Forca o envio das ultimas alteracoes
	FlushFileBuffers(this->hPipe);
	// Desconectar
	if (!DisconnectNamedPipe(this->hPipe)) {
		tcout << getInfo() << TEXT("error on disconnecting pipe");
		ExitThread(-1);
	}
	// Fechar instancia do pipe
	CloseHandle(this->hPipe);

	return 1;
}

sTchar_t ThreadCliente::getInfo()
{
	oTcharStream_t stream;
	if (this->currentClient)
		stream << TEXT("ThreadCliente") << this->currentClient->GetId() << TEXT(": ");
	else
		stream << TEXT("ThreadCliente") << this->threadID << TEXT(": ");
	return stream.str();
}
