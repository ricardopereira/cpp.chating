#include "ThreadCaixaDialogoLogin.h"
#include <tchar.h>
#include "resource.h"

ThreadCaixaDialogoLogin::ThreadCaixaDialogoLogin(Server& servidor) : servidor(&servidor)
{
	ptrClasse = this;
}

ThreadCaixaDialogoLogin::~ThreadCaixaDialogoLogin()
{
}

DWORD WINAPI ThreadCaixaDialogoLogin::funcaoThread(LPVOID param)
{
	//DWORD result = DialogBox(this->hInstance, (LPCWSTR) IDD_LOGIN, this->hWndPai, (DLGPROC)DialogLogin);

	// ToDo: Discutir com o M�rio sobre este ponto
	//RP - Penso que n�o seja boa pr�tica

	return 1;
}

void ThreadCaixaDialogoLogin::setHwndPai(HWND hWnd)
{
	this->hWndPai = hWnd;
}

void ThreadCaixaDialogoLogin::sethInstance(HINSTANCE hInstance)
{
	this->hInstance = hInstance;
}