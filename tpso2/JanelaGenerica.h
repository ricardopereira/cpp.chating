#pragma once
#include <Windows.h>
#include <tchar.h>
#include "ThreadCaixaDialogoLogin.h"
#include "Comunicacao.h"

class JanelaGenerica {
private:
protected:
	JanelaGenerica(){};
	WNDCLASSEX _WndClsEx;
	HWND _hWnd;
	HINSTANCE hInst;
	long maxX,
		 maxY;
	HDC memdc;
public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT myWndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) = 0; //método abstracto
	virtual void MostrarElementos(HWND hWnd) = 0;
	void Inicializar(
		HINSTANCE hInst,
		LPCTSTR ClassName,
		UINT class_size,
		LPCTSTR MenuName = NULL,
		UINT style = CS_VREDRAW | CS_HREDRAW
		);


	void Registar();
	HWND Criar(HINSTANCE hInst, LPCTSTR wndName);
	BOOL Mostrar(int dCmdShow = SW_SHOWNORMAL);
};