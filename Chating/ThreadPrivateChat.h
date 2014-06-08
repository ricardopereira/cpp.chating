#pragma once

#include "Thread.h"
#include "Controller.h"
#include "AssyncThread.h"

class ThreadPrivateChat : public Thread
{
private:
	HINSTANCE hInstance;
	HWND hWndPai;

	Controller* controller;
	const TCHAR* username;
	AssyncThread* assyncThread;

	ThreadPrivateChat();
public:
	ThreadPrivateChat(Controller& controller, const TCHAR* username, AssyncThread *assyncThread);
	~ThreadPrivateChat();

	virtual DWORD WINAPI funcaoThread();

	void setHwndPai(HWND hWnd);
	void sethInstance(HINSTANCE hInstance);
};

