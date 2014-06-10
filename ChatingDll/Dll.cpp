#include <windows.h>
#include <tchar.h>

#include "Dll.h"
#include "ChatComunication.h"

HANDLE hPipe;

DLL_IMP_API int AbrirPipe(const TCHAR* ip) {

	TCHAR ipserver[TAMIP];

	// "\\\\.\\pipe\\pipeserver"
	_stprintf_s(ipserver, TAMIP, TEXT("\\\\%s\\pipe\\%s"), ip, PIPENAME_SERVER);

	hPipe = CreateFile(
		ipserver,
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
	buffer.command = commands_t::LOGIN;
	
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
	buffer.command = commands_t::CRIAR_UTILIZADOR;

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

bool RemoverUtilizador(const TCHAR *login)
{
	chatbuffer_t buffer;
	_tcscpy_s(buffer.args[0], _tcslen(login)*sizeof(TCHAR), login);
	buffer.command = commands_t::ELIMINAR_UTILIZADOR;

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
		return false;
	// Resposta
	success = ReadFile(
		hPipe,
		&buffer,
		sizeof(chatbuffer_t),
		&bytesRead,
		NULL);
	if (!success)
		return false;

	if (buffer.arg_num == SUCCESS)
		return true;
	else
		return false;
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

int EnviarMensagemPrivada(const TCHAR *msg, const TCHAR *owner)
{
	// Escrever no pipe
	chatbuffer_t buffer;
	buffer.command = commands_t::ENVIAR_MSG_PRIVADA;
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
		return 0;

	// Recebe a resposta
	success = ReadFile(
		hPipe,
		&buffer,
		sizeof(chatbuffer_t),
		&bytesRead,
		NULL);
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

int Sair(const TCHAR* utilizador)
{
	// Escrever no pipe
	chatbuffer_t buffer;
	buffer.command = commands_t::LOGOUT;
	_tcscpy_s(buffer.args[0], _tcslen(utilizador)*sizeof(TCHAR), utilizador);
	
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

	// Fecha o pipe
	CloseHandle(hPipe);
	return success;
}

int Desligar()
{
	doSimpleRequest(commands_t::DESLIGAR_SERVIDOR);
	// Fecha o pipe
	CloseHandle(hPipe);
	return 0;
}

void CancelarConversa()
{
	doSimpleRequest(commands_t::CANCELAR_CONVERSA);
}
