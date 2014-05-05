#pragma once
#include <windows.h>

//---------------------------------------------------------------------------
class WApplication
{
public:
	// Este construtor inicializa a aplica��o
	WApplication(HINSTANCE hInst, LPCTSTR ClasName,
		WNDPROC WndPrc, LPCTSTR MenuName = NULL, UINT style = CS_VREDRAW | CS_HREDRAW);

	// Registo da estrutura (class) da janela
	void Register();

protected:
	// Vari�vel global da estrutura (class) da janela
	WNDCLASSEX _WndClsEx;
};
//---------------------------------------------------------------------------