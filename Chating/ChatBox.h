#pragma once
#include "Controlo.h"

class ChatBox : public Controlo
{
private:
	int scrollX;
	int scrollY;
public:
	ChatBox(HINSTANCE hInstance, long px, long py, long comprimento, long largura);
	~ChatBox();

	void Mostra(HWND hWnd);

	// Teste
	void doPaint(HDC hdc, HWND hWnd);

	int getScrollX();
	int getScrollY();

	void scrollUp();
	void scrollDown();
};