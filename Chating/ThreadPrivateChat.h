#pragma once

#include "Thread.h"
#include "Server.h"

class ThreadPrivateChat : public Thread
{
private:
	HINSTANCE hInstance;
	HWND hWndPai;
	Server* servidor;

	ThreadPrivateChat() { servidor = new Server; /* Memory leak mas é privado :) */ };
public:
	ThreadPrivateChat(Server& servidor);
	~ThreadPrivateChat();

	virtual DWORD WINAPI funcaoThread();

	void setHwndPai(HWND hWnd);
	void sethInstance(HINSTANCE hInstance);
};

