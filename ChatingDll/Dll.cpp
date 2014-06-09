#include <windows.h>
#include <tchar.h>

#include "Dll.h"
#include "ChatComunication.h"

HANDLE hPipe;
HANDLE assyncPipe;

DLL_IMP_API int AbrirPipe() {
	hPipe = CreateFile(
		PIPENAME_SERVER,
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

int doSimpleRequest(commands_t command)
{
	// Escrever no pipe
	chatbuffer_t buffer;
	buffer.command = command;
	
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

	// Recebe a resposta
	success = ReadFile(
		hPipe,
		&buffer,
		sizeof(chatbuffer_t),
		&bytesRead,
		NULL);

	return success;
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

int RemoverUtilizador(const TCHAR *login)
{
	return 0;
}

int LerListaUtilizadores()
{
	return doSimpleRequest(commands_t::LISTA_UTILIZADORES_ONLINE);
}

int LerListaUtilizadoresRegistados()
{
	return doSimpleRequest(commands_t::LISTA_UTILIZADORES_TODOS);
}

int IniciarConversa(const TCHAR *utilizador, int flag)
{
	chatbuffer_t buffer;
	_tcscpy_s(buffer.args[0], _tcslen(utilizador)*sizeof(TCHAR), utilizador);
	buffer.command = commands_t::LANCAR_CHAT;
	buffer.arg_num = flag;

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

int DesligarConversa()
{
	// Escrever no pipe
	return doSimpleRequest(commands_t::FECHAR_CHAT);
}

int EnviarMensagemPrivada(const TCHAR *msg)
{
	// Escrever no pipe
	return 0;
}

void EnviarMensagemPublica(const TCHAR *msg, const TCHAR *owner)
{
	// Escrever no pipe
	chatbuffer_t buffer;
	buffer.command = commands_t::ENVIAR_MSG_PUBLICA;
	_tcscpy_s(buffer.args[0], _tcslen(msg)*sizeof(TCHAR), msg);
	_tcscpy_s(buffer.args[1], _tcslen(owner)*sizeof(TCHAR), owner);
	// ToDo: convém saber o login na mensagem
	
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
		return;

	// Recebe a resposta
	success = ReadFile(
		hPipe,
		&buffer,
		sizeof(chatbuffer_t),
		&bytesRead,
		NULL);
}

void LerInformacaoInicial()
{
	doSimpleRequest(commands_t::LER_INFO_INICIAL);
}

int Sair()
{
	return doSimpleRequest(commands_t::LOGOUT);
}

int Desligar()
{
	// Fecha o pipe
	CloseHandle(hPipe);
	return 0;
}
