#pragma once

#include "Control.h"
#include <string>

class EditBox : public Control
{
private:
	int static CaixaTextoIdAcum;
	std::basic_string<TCHAR> texto;
	HFONT hfont;
public:
	EditBox(HINSTANCE hInstance, long px, long py, long comprimento, long largura);
	~EditBox();

	void Mostra(HWND hWnd);
	std::basic_string<TCHAR> getTexto();
};