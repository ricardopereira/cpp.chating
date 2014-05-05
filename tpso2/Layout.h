#pragma once
#include <vector>
#include <Windows.h>
class Layout
{
private:
	float percentagem;
	long pos;
	long altura;
	long largura;
	static long posXAcum;
	static long posYAcum;
public:
	static enum Alinhamento{ esquerda, centrado, direita, cima, baixo };
	Layout();
	Layout(float percentagem);
	void calcularPosicoesY(HWND hWnd);
	void calcularPosicoesX(HWND hWnd);
	long getPosicao();
	long getAltura();
	long getLargura();
	static void ReiniciarValoresAcumulados();
	~Layout();
};

