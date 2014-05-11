#pragma once

#include "Thread.h"
#include "Server.h"

class ThreadCaixaDialogoLogin : public Thread
{
private:
	HINSTANCE hInstance;
	HWND hWndPai;
	Server* servidor;

	ThreadCaixaDialogoLogin() { servidor = new Server; /* Memory leak mas � privado :) */ };
public:
	ThreadCaixaDialogoLogin(Server& servidor);
	~ThreadCaixaDialogoLogin();

	virtual DWORD WINAPI funcaoThread(LPVOID param);

	void setHwndPai(HWND hWnd);
	void sethInstance(HINSTANCE hInstance);
};

