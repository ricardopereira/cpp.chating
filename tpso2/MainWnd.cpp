#include "MainWnd.hpp"

//---------------------------------------------------------------------------
WWindow::WWindow()
{
	// Se a classe da janela � declarada com um construtor por defeito
	//este anula o handle da janela
	_hwnd = NULL;
}
//---------------------------------------------------------------------------
HWND WWindow::Create(HINSTANCE hinst,
	LPCTSTR clsname,
	LPCTSTR wndname,
	HWND parent,
	DWORD dStyle,
	DWORD dXStyle,
	int x,
	int y,
	int width,
	int height)
{

	// Criar uma nova janela
	_hwnd = CreateWindowEx(dXStyle, clsname, wndname, dStyle, x, y, width,
		height, parent, NULL, hinst, this);

	// Se a janela n�o foi criada
	if (_hwnd != NULL)
		return _hwnd;

	return NULL;
}
//---------------------------------------------------------------------------
BOOL WWindow::Show(int dCmdShow)
{
	// ============================================================================
	// Mostrar a janela
	// ============================================================================
	if (!ShowWindow(_hwnd, dCmdShow))	// "hWnd"= handler da janela, devolvido 
		return FALSE;					// por "CreateWindow"; "dCmdShow"= modo de
										// exibi��o (p.e. normal, modal); � passado
										// como par�metro de WinMain()

	if (!UpdateWindow(_hwnd))			// Refrescar a janela (Windows envia � janela
		return FALSE;					// uma mensagem para pintar, mostrar dados,
										// (refrescar), etc)
	return TRUE;
}

void WWindow::desenharInterface(){
	
}
//---------------------------------------------------------------------------
WWindow::operator HWND()
{
	//Uma vez que cada janela � do tipo HWND, utilizaremos 
	//um modo de reconhecer o handle da janela quando utilizado 
	//na aplica��o
	return _hwnd;
}
//---------------------------------------------------------------------------