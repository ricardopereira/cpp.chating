#include "ThreadPrivateChat.h"
#include <tchar.h>
#include "resource.h"
#include "JanelaPrivada.h"

ThreadPrivateChat::ThreadPrivateChat()
{
	servidor = NULL;
	username = NULL;
}

ThreadPrivateChat::ThreadPrivateChat(Server& servidor, const TCHAR* username) : servidor(&servidor), username(username)
{
	ptrClasse = this;
}

ThreadPrivateChat::~ThreadPrivateChat()
{
}

DWORD WINAPI ThreadPrivateChat::funcaoThread()
{
	if (!this->servidor) return 0;

	// MSG é uma estrutura definida no Windows para as mensagens
	MSG lpMsg;

	hInstance = GetModuleHandle(NULL);
	
	JanelaPrivada prvChat(*this->servidor,this->username);
	prvChat.Inicializar(hInstance, TEXT("JanelaPrivada"), sizeof(JanelaPrivada*), NULL);
	prvChat.Registar();
	prvChat.Criar(hInstance, TEXT("Chat Privado"));
	prvChat.Mostrar();

	while (GetMessage(&lpMsg,NULL,0,0)) {
		TranslateMessage(&lpMsg);
		DispatchMessage(&lpMsg);
	}
	return 1;
}

void ThreadPrivateChat::setHwndPai(HWND hWnd)
{
	this->hWndPai = hWnd;
}

void ThreadPrivateChat::sethInstance(HINSTANCE hInstance)
{
	this->hInstance = hInstance;
}