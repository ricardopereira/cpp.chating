#include "Shell.h"
#include "ThreadCliente.h"
#include "ChatComunication.h"

ThreadCliente::ThreadCliente(HANDLE hPipe)
{
	this->ptrClasse = this;
	this->hPipe = hPipe;
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
		case ThreadCliente::LOGIN:
			break;
		case ThreadCliente::LANCAR_CHAT:
			break;
		case ThreadCliente::ENVIAR_MSG_PRIVADA:
			break;
		case ThreadCliente::ENVIAR_MSG_PUBLICA:
			break;
		case ThreadCliente::FECHAR_CHAT:
			break;
		case ThreadCliente::LER_INFO_INICIAL:
			break;
		case ThreadCliente::CRIAR_USER:
			break;
		case ThreadCliente::LER_MENSAGEM_PUBLICA:
			break;
		case ThreadCliente::LER_MENSAGEM_PRIVADA:
			break;
		case ThreadCliente::ELIMINAR_UTILIZADOR:
			break;
		}


	}
	
	return 1;
}

