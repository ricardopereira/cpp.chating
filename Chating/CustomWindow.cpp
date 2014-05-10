#include "CustomWindow.h"
#include "resource.h"
#include <crtdbg.h>

// Função não membro (auxiliar)
LRESULT CALLBACK internalWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	// Get a window pointer associated with this window
	CustomWindow *ptr = (CustomWindow*)GetWindowLong(hWnd, GWL_USERDATA);
	// It should be valid, assert so
	_ASSERT(ptr);
	// Redirect messages to the window procedure of the associated window
	return ptr->performWMessage(hWnd, msg, wp, lp);
}

// MainWnd
HWND CustomWindow::Criar(HINSTANCE hInst, LPCTSTR wndName)
{
	this->_hWnd = CreateWindowEx(0L, _WndClsEx.lpszClassName, wndName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, (HWND)HWND_DESKTOP, NULL, hInst, this);

	// Se a janela não foi criada
	if (this->_hWnd != NULL) {
		SetWindowLongPtr(this->_hWnd, GWLP_USERDATA, (long)this);
		this->hInst = hInst;
		return _hWnd;
	}
	return NULL;
}

// WinApp
//void CustomWindow::Inicializar(HINSTANCE hInst, LPCTSTR ClassName, UINT class_size, LPCTSTR MenuName, UINT style)

void CustomWindow::Registar()
{
	//Registar a classe "_WndClsEx" no Windows
	RegisterClassEx(&_WndClsEx);
}

BOOL CustomWindow::Mostrar(int dCmdShow)
{
	// Mostrar a janela
	if (!ShowWindow(_hWnd, dCmdShow))	// "hWnd"= handler da janela, devolvido 
		return FALSE;					// por "CreateWindow"; "dCmdShow"= modo de
	// exibição (p.e. normal, modal); é passado
	// como parâmetro de WinMain()

	if (!UpdateWindow(_hWnd))			// Refrescar a janela (Windows envia à janela
		return FALSE;					// uma mensagem para pintar, mostrar dados,
	// (refrescar), etc)
	return TRUE;
}

LRESULT CALLBACK CustomWindow::WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam){
	LPCREATESTRUCT cs;
	switch (messg) {
	case WM_NCCREATE:
		cs = (LPCREATESTRUCT) lParam;
		// Associar o ponteiro
		SetWindowLongPtr(hWnd, GWL_USERDATA, (long)cs->lpCreateParams);
		// Alterar função de tratamento de eventos 
		SetWindowLongPtr(hWnd, GWL_WNDPROC, (long)internalWndProc);
		// Encaminhar WM_NCCREATE para a função membro de  tratamento de eventos
		return internalWndProc(hWnd, messg, wParam, lParam);
		break;
	default:
		// Neste exemplo, para qualquer outra mensagem (p.e. "minimizar", "maximizar",
		// "restaurar") não é efectuado nenhum processamento, apenas se segue 
		// o "default" do Windows DefWindowProc()
		return(DefWindowProc(hWnd, messg, wParam, lParam));
		break;
	}
	return(0);
}