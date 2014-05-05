#pragma once
#include "Thread.h"
#include "Comunicacao.h"
class ThreadCaixaDialogoLogin : public Thread
{

private:
	HINSTANCE hInstance;
	HWND hWndPai;
	Comunicacao& messenger;
	ThreadCaixaDialogoLogin();
public:
	ThreadCaixaDialogoLogin(Comunicacao& messenger);
	~ThreadCaixaDialogoLogin();

	virtual DWORD WINAPI funcaoThread(LPVOID param);
	static BOOL CALLBACK DialogoLogin(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
	void setHwndPai(HWND hWnd);
	void sethInstance(HINSTANCE hInstance);
};

