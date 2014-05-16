#include <windows.h>
#include <tchar.h>

#include "Dll.h"
#include "ChatComunication.h"

int Autenticar(const TCHAR *login, const TCHAR *pass)
{
	HANDLE hPipe;
	chatbuffer_t buffer;

    hPipe = CreateFile(
        pipeName,
        GENERIC_READ | GENERIC_WRITE,
        0, //no sharing
        NULL, //default security attributes
        OPEN_EXISTING, //opens existing pipe
        0, //default attributes
        NULL); //no template file

    if (hPipe == INVALID_HANDLE_VALUE)
        return 0;

    if (GetLastError() == ERROR_PIPE_BUSY) 
        return 0;

	PTCHAR msg = TEXT("Ligacao com sucesso");
	//DWORD msgBytes;
	DWORD bytesSent;
	BOOL success = 0;

	// Envio de pedido
	success = WriteFile(hPipe,
		&buffer, //message
		sizeof(buffer), //message length
		&bytesSent, //bytes written
		NULL); //not overlapped

	if (!success) 
		return -1;

	// Fecha o pipe
	CloseHandle(hPipe);

	return 1;
}

int LerListaUtilizadores(UTILIZADOR *utilizadores)
{
	// Ler do pipe
	return 0;
}

int IniciarConversa(const TCHAR *utilizador)
{
	// Escrever no pipe
	return 0;
}

int DesligarConversa()
{
	// Escrever no pipe
	return 0;
}

int EnviarMensagemPrivada(const TCHAR *msg)
{
	// Escrever no pipe
	return 0;
}

void EnviarMensagemPublica(const TCHAR *msg)
{
	// Escrever no pipe
}

CHAT LerInformacaoInicial()
{
	// Ler do pipe
	CHAT dumb;
	return dumb;
}

MENSAGEM LerMensagensPublicas()
{
	// Ler do pipe
	MENSAGEM dumb;
	return dumb;
}

MENSAGEM LerMensagensPrivadas()
{
	// Ler do pipe
	MENSAGEM dumb;
	return dumb;
}

int Sair()
{
	return 0;
}

int Desligar()
{
	return 0;
}