#include "CustomWindow.h"
#include "resource.h"
#include <crtdbg.h>

// Função não membro (auxiliar)
LRESULT CALLBACK internalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	RECT rect;
	short zDelta;
	// onPaint: Ponteiro para estrutura de WM_PAINT
	PAINTSTRUCT ps;

	// Get a window pointer associated with this window
	CustomWindow *ptr = (CustomWindow*)GetWindowLong(hWnd, GWL_USERDATA);
	// It should be valid, assert so
	_ASSERT(ptr);
	
	// Eventos de cada mensagem
	switch (message) {
	case WM_CREATE:
		ptr->onCreate(hWnd,hdc);
		break;

	case WM_DESTROY:
		ptr->onDestroy(hWnd);
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		if (ptr->onClose(hWnd))
			DestroyWindow(hWnd);
		break;

	case WM_ACTIVATE:
		ptr->onActivate(hWnd);
		break;

	case WM_SHOWWINDOW:
		//wParam is TRUE, the window is being shown
		if ((int)wParam == 1)
			ptr->onShow(hWnd);
		break;

	case WM_SIZE:
		ptr->onResize(hWnd);
		break;

	case WM_MOVE:
		ptr->onMove(hWnd);
		break;

	case WM_MOUSEWHEEL:
		zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (zDelta > 0) {
			ptr->onMouseWheelUp(hWnd);
		} else {
			ptr->onMouseWheelDown(hWnd);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);

		ptr->onPaint(hWnd,hdc,rect);

		EndPaint(hWnd, &ps);
		break;

	case WM_COMMAND:
		ptr->onCommand(hWnd,wParam,lParam);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	// Personalizado - redirect messages to the window procedure of the associated window
	return ptr->performWMessage(hWnd, message, wParam, lParam);
}

// MainWnd
HWND CustomWindow::Criar(HINSTANCE hInst, LPCTSTR wndName)
{
	this->_hWnd = CreateWindowEx(0L, _WndClsEx.lpszClassName, wndName, WS_OVERLAPPEDWINDOW, windowX(), windowY(), windowWidth(),
		windowHeight(), (HWND)HWND_DESKTOP, NULL, hInst, this);

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

LRESULT CALLBACK CustomWindow::WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
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

void CustomWindow::refresh(HWND hWnd)
{
	InvalidateRect(hWnd, NULL, 1);
}

// WndProc para cada especializacao

LRESULT CustomWindow::performWMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return(0);
}

// Eventos

void CustomWindow::onCreate(HWND hWnd, HDC &hdc)
{

}

void CustomWindow::onDestroy(HWND hWnd)
{

}

void CustomWindow::onShow(HWND hWnd)
{

}

bool CustomWindow::onClose(HWND hWnd)
{
	return true;
}

void CustomWindow::onActivate(HWND hWnd)
{

}

void CustomWindow::onResize(HWND hWnd)
{

}

void CustomWindow::onMove(HWND hWnd)
{

}

void CustomWindow::onMouseWheelUp(HWND hWnd)
{

}

void CustomWindow::onMouseWheelDown(HWND hWnd)
{

}

void CustomWindow::onPaint(HWND hWnd, HDC &hdc, RECT &rect)
{

}

void CustomWindow::onCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{

}