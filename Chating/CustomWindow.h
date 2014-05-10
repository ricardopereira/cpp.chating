#pragma once

#include <Windows.h>
#include <tchar.h>

class CustomWindow
{
private:
protected:
	WNDCLASSEX _WndClsEx;
	HWND _hWnd;
	HINSTANCE hInst;
	long maxX,maxY;
	HDC memdc;
	CustomWindow() {};
public:
	virtual ~CustomWindow() {};

	virtual void Inicializar(HINSTANCE hInst, LPCTSTR ClassName, UINT class_size,
		LPCTSTR MenuName = NULL, UINT style = CS_VREDRAW | CS_HREDRAW) = 0;

	void Registar();
	HWND Criar(HINSTANCE hInst, LPCTSTR wndName);
	BOOL Mostrar(int dCmdShow = SW_SHOWNORMAL);

	virtual void MostrarElementos(HWND hWnd) = 0;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);

	void refresh(HWND hWnd);

	// WndProc para cada especializacao
	virtual LRESULT performWMessage(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
	// Eventos
	virtual void onCreate(HWND hWnd, HDC &hdc);
	virtual void onDestroy(HWND hWnd);
	virtual void onShow(HWND hWnd);
	virtual bool onClose(HWND hWnd);
	virtual void onActivate(HWND hWnd);
	virtual void onResize(HWND hWnd);
	virtual void onMove(HWND hWnd);
	virtual void onMouseWheelUp(HWND hWnd);
	virtual void onMouseWheelDown(HWND hWnd);
	virtual void onPaint(HWND hWnd, HDC &hdc, RECT &rect);
	virtual void onCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
};