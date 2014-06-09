#include "ThreadPrivateChat.h"
#include <tchar.h>
#include "resource.h"
#include "JanelaPrivada.h"

ThreadPrivateChat::ThreadPrivateChat() : username(*(new sTchar_t(TEXT(""))))
{
	controller = NULL;
}

ThreadPrivateChat::ThreadPrivateChat(Controller& controller, sTchar_t username, AssyncThread* assyncThread) : controller(&controller), username(username)
{
	ptrClasse = this;
	this->assyncThread = assyncThread;
	
}

ThreadPrivateChat::~ThreadPrivateChat()
{
}

DWORD WINAPI ThreadPrivateChat::funcaoThread()
{
	if (!this->controller) return 0;

	// MSG é uma estrutura definida no Windows para as mensagens
	MSG lpMsg;

	hInstance = GetModuleHandle(NULL);
	
	JanelaPrivada prvChat(*this->controller,this->username, this->assyncThread, this->handleThread);
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