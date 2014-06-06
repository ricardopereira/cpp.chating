#include <windows.h>
#include <tchar.h>

#include "Dll.h"
#include "ChatComunication.h"
HANDLE hPipe;
HANDLE assyncPipe;
enum commands_t {
	REGISTER_NEW_USER,
	LOGIN,
	LANCAR_CHAT,
	ENVIAR_MSG_PRIVADA,
	ENVIAR_MSG_PUBLICA,
	FECHAR_CHAT,
	LER_INFO_INICIAL,
	CRIAR_USER,
	DESLIGAR,
	LER_MENSAGEM_PRIVADA,
	LER_MENSAGEM_PUBLICA,
	ELIMINAR_UTILIZADOR,
	LOGOUT
};


DLL_IMP_API int AbrirPipe(){
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
	return 1;
}
int Autenticar(const TCHAR *login, const TCHAR *pass)
{
	
	chatbuffer_t buffer;
	_tcscpy_s(buffer.args[0], _tcslen(login)*sizeof(TCHAR), login);
	_tcscpy_s(buffer.args[1], _tcslen(pass)*sizeof(TCHAR), pass);
	buffer.command = LOGIN;
	
	PTCHAR msg = TEXT("Ligacao com sucesso");
	//DWORD msgBytes;
	DWORD bytesSent;
	DWORD bytesRead;
	BOOL success = 0;
	// Envio de pedido
	success = WriteFile(hPipe,
		&buffer, //message
		sizeof(chatbuffer_t), //message length
		&bytesSent, //bytes written
		NULL); //not overlapped

	if (!success) 
		return -1;
	success = ReadFile(
		hPipe,
		&buffer,
		sizeof(chatbuffer_t),
		&bytesRead,
		NULL);
	if (!success)
		return -1;
	
	return buffer.arg_num;
}
int Registar(const TCHAR *login, const TCHAR *pass)
{
	
	chatbuffer_t buffer;
	_tcscpy_s(buffer.args[0], _tcslen(login)*sizeof(TCHAR), login);
	_tcscpy_s(buffer.args[1], _tcslen(pass)*sizeof(TCHAR), pass);
	buffer.command = REGISTER_NEW_USER;

	PTCHAR msg = TEXT("Ligacao com sucesso");
	//DWORD msgBytes;
	DWORD bytesSent;
	DWORD bytesRead;
	BOOL success = 0;

	// Envio de pedido
	success = WriteFile(hPipe,
		&buffer, //message
		sizeof(chatbuffer_t), //message length
		&bytesSent, //bytes written
		NULL); //not overlapped

	if (!success)
		return -1;
	success = ReadFile(
		hPipe,
		&buffer,
		sizeof(chatbuffer_t),
		&bytesRead,
		NULL);
	if (!success)
		return -1;

	return buffer.arg_num;
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
	// Fecha o pipe
	CloseHandle(hPipe);
	return 0;
}
