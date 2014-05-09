#pragma once
#include "Controlo.h"
#include <string>

class CaixaTexto : public Controlo
{
private:
	int static CaixaTextoIdAcum;
	std::basic_string<TCHAR> texto;
	HFONT hfont;
public:
	CaixaTexto(HINSTANCE hInstance, long px, long py, long comprimento, long largura);
	void Mostra(HWND hWnd);
	~CaixaTexto();
	std::basic_string<TCHAR> getTexto();
};

