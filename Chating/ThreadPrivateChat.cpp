#include "ThreadPrivateChat.h"
#include <tchar.h>
#include "resource.h"
#include "JanelaPrivada.h"

ThreadPrivateChat::ThreadPrivateChat(Server& servidor) : servidor(&servidor)
{
	ptrClasse = this;
}

ThreadPrivateChat::~ThreadPrivateChat()
{
}

DWORD WINAPI ThreadPrivateChat::funcaoThread()
{
	// MSG é uma estrutura definida no Windows para as mensagens
	MSG lpMsg;
	
	JanelaPrivada prvChat;
	prvChat.Inicializar(hInstance, TEXT("Chating") /*constante*/, sizeof(JanelaPrivada*), MAKEINTRESOURCE(IDR_MENU1));
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