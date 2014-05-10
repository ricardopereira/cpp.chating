#include "ThreadCaixaDialogoLogin.h"
#include <tchar.h>
#include "resource.h"

TCHAR login[50];
TCHAR pass[50];

ThreadCaixaDialogoLogin::ThreadCaixaDialogoLogin(Server& servidor) : servidor(&servidor)
{
	ptrClasse = this;
}

ThreadCaixaDialogoLogin::~ThreadCaixaDialogoLogin()
{
}

DWORD WINAPI ThreadCaixaDialogoLogin::funcaoThread(LPVOID param) 
{
	DWORD valRetorno = DialogBox(this->hInstance, (LPCWSTR) IDD_LOGIN, this->hWndPai,
		(DLGPROC)ThreadCaixaDialogoLogin::DialogoLogin);

	if (valRetorno == 1) {
		DWORD res = this->servidor->cAutenticar(login, pass);

		if (res == 2){
			MessageBox(this->hWndPai, TEXT("Login: Administrador"), TEXT("Login"), MB_OK | MB_ICONINFORMATION);
		}
		else if (res == 1){
			TCHAR text[100];
			_stprintf_s(text, 99, _T("Login: %s"), login);
			MessageBox(this->hWndPai, text, TEXT("Login"), MB_OK | MB_ICONINFORMATION);
		}
		else {
			sTchar_t text;
			text = TEXT("Login Inválido ou servidor desligado.");
			MessageBox(this->hWndPai, text.c_str(), TEXT("Login"), MB_OK | MB_ICONINFORMATION);
		}
	}

	return 1;
}

BOOL CALLBACK ThreadCaixaDialogoLogin::DialogoLogin(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam){
	switch (messg) {
	    case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
					//GetWindowText(GetDlgItem(hWnd, IDC_USER), login, 49);
					//GetWindowText(GetDlgItem(hWnd, IDC_PASS), pass, 49);
					EndDialog(hWnd, 1);
					return 0;
					break;
				case IDCANCEL:
					EndDialog(hWnd, 0);
					return 0;
			}
			break;
	}
	return 0;
}

void ThreadCaixaDialogoLogin::setHwndPai(HWND hWnd)
{
	this->hWndPai = hWnd;
}

void ThreadCaixaDialogoLogin::sethInstance(HINSTANCE hInstance)
{
	this->hInstance = hInstance;
}