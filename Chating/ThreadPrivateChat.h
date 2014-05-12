#pragma once

#include "Thread.h"
#include "Server.h"

class ThreadPrivateChat : public Thread
{
private:
	HINSTANCE hInstance;
	HWND hWndPai;

	Server* servidor;
	const TCHAR* username;

	ThreadPrivateChat();
public:
	ThreadPrivateChat(Server& servidor, const TCHAR* username);
	~ThreadPrivateChat();

	virtual DWORD WINAPI funcaoThread();

	void setHwndPai(HWND hWnd);
	void sethInstance(HINSTANCE hInstance);
};

