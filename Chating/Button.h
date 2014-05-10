#pragma once

#include "Control.h"
#include "Common.h"

class Button : public Control
{
private:
	int static botaoIdAcum;
	sTchar_t texto;
	HFONT hfont;
public:
	Button(HINSTANCE hInstance, long px, long py, long comprimento, long largura);
	~Button();

	void setTextoBotao(const sTchar_t texto);
	void Mostra(HWND hWnd);
};

