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
	virtual void Inicializar(HINSTANCE hInst, LPCTSTR ClassName, UINT class_size,
		LPCTSTR MenuName = NULL, UINT style = CS_VREDRAW | CS_HREDRAW) = 0;

	void Registar();
	HWND Criar(HINSTANCE hInst, LPCTSTR wndName);
	BOOL Mostrar(int dCmdShow = SW_SHOWNORMAL);

	virtual void MostrarElementos(HWND hWnd) = 0;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);

	// WndProc para cada especializacao
	virtual LRESULT performWMessage(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) = 0;
};