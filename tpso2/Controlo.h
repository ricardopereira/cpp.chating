#pragma once
#include <Windows.h>

class Controlo{
protected:
	long pos_x;
	long pos_y;
	long comprimento;
	long largura;
	HWND hWnd;
	HWND hWndPai;
	int ControloId;
	HINSTANCE hInstance;
	Controlo(){};
	Controlo(HINSTANCE hInstance, long px, long py, long comprimento, long largura);
public:
	
	virtual void Mostra(HWND hWnd)=0;
	HWND getHwnd();
	void setHwndPai(HWND hWnd);
	int getId();
};