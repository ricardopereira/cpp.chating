#pragma once

#include <Windows.h>

class Control
{
protected:
	long pos_x;
	long pos_y;
	long comprimento;
	long largura;
	HWND hWnd;
	HWND hWndPai;
	int ControloId;
	HINSTANCE hInstance;

	Control() {};
	Control(HINSTANCE hInstance, long px, long py, long comprimento, long largura);
public:
	virtual void Mostra(HWND hWnd)=0;
	HWND getHwnd();
	void setHwndPai(HWND hWnd);
	int getId();

	long getX();
	long getY();
	long getWidth();
	long getHeight();
};