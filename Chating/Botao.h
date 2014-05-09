#pragma once
#include "Controlo.h"
#include "Comunicacao.h"

class Botao : public Controlo
{
private:
	int static botaoIdAcum;
	sTchar_t texto;
	HFONT hfont;
public:
	Botao(HINSTANCE hInstance, long px, long py, long comprimento, long largura);
	void setTextoBotao(const sTchar_t texto);
	void Mostra(HWND hWnd);
	~Botao();
};

