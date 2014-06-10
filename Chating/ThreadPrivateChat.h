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
	sTchar_t username;
	AssyncThread* assyncThread;
	int flag;
	HWND publicWindowHandle;
	ThreadPrivateChat();
public:
	ThreadPrivateChat(Controller& controller, sTchar_t username, AssyncThread *assyncThread, int &flag, HWND publicWindowHandle);
	~ThreadPrivateChat();

	virtual DWORD WINAPI funcaoThread();

	void setHwndPai(HWND hWnd);
	void sethInstance(HINSTANCE hInstance);
};

