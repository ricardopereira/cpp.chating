#pragma once
#include "Controlo.h"

class Botao : public Controlo
{
private:
	int static botaoIdAcum;
public:
	Botao(HINSTANCE hInstance, long px, long py, long comprimento, long largura);
	void Mostra(HWND hWnd);
	~Botao();
};

