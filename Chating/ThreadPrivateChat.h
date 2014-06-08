#pragma once

#include "Thread.h"
#include "Controller.h"

class ThreadPrivateChat : public Thread
{
private:
	HINSTANCE hInstance;
	HWND hWndPai;

	Controller* controller;
	const TCHAR* username;

	ThreadPrivateChat();
public:
	ThreadPrivateChat(Controller& controller, const TCHAR* username);
	~ThreadPrivateChat();

	virtual DWORD WINAPI funcaoThread();

	void setHwndPai(HWND hWnd);
	void sethInstance(HINSTANCE hInstance);
};

