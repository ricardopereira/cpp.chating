#pragma once
#include <windows.h>

//---------------------------------------------------------------------------
class WWindow
{
public:
	// Utilizaremos um classe por defeito para construir a janela
	WWindow();
	// O m�todo Create() ser� utilizado para inicializar a janela
	HWND Create(HINSTANCE hinst,
		LPCTSTR clsname,
		LPCTSTR wndname,
		HWND parent = NULL,
		DWORD dStyle = WS_OVERLAPPEDWINDOW,
		DWORD dXStyle = 0L,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int width = CW_USEDEFAULT,
		int height = CW_USEDEFAULT);

	// M�todo para mostrar a janela
	BOOL Show(int dCmdShow = SW_SHOWNORMAL);

	// desenhar interface
	void desenharInterface();

	//Uma vez que cada janela � do tipo HWND, utilizaremos 
	//um modo de reconhecer o handle da janela quando utilizado 
	//na aplica��o
	operator HWND();

protected:
	//Este ser� o handle global dispon�vel para esta e para outras janelas
	HWND _hwnd;
};
//---------------------------------------------------------------------------