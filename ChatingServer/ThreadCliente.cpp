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
	}
	
	return 1;
}

