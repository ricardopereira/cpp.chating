#include "Layout.h"
#include <tchar.h>

long Layout::posXAcum = 10;
long Layout::posYAcum = 10;

Layout::Layout()
{
}


Layout::~Layout()
{
}
Layout::Layout(float percentagem){
	this->percentagem = percentagem;
}
void Layout::calcularPosicoesY(HWND hWnd){
	RECT dimensoes;
	GetClientRect(hWnd, &dimensoes);
	this->altura = static_cast<long>((dimensoes.bottom - 2) * this->percentagem);

	this->pos = static_cast<long>(this->posYAcum);

	this->posYAcum += altura;
	TCHAR text[100];
	text[0] = TEXT('\0');
	//_stprintf_s(text, 99,_T("Comprimento: %ld, Largura: %ld, Percentagem: %f"), dimensoes.right, dimensoes.bottom, this->percentagem);
	//MessageBox(hWnd, text, TEXT("YO"), MB_OK);
}

void Layout::calcularPosicoesX(HWND hWnd){
	RECT dimensoes;
	GetClientRect(hWnd, &dimensoes);
	this->largura = static_cast<long>((dimensoes.right - 3) * this->percentagem);

	this->pos = static_cast<long>(this->posXAcum);

	this->posXAcum += this->largura;
	//_stprintf_s(text, 50,_T("Comprimento: %ld, Largura: %ld"), dimensoes.left, dimensoes.bottom);
	//MessageBox(hWnd, text, TEXT("YO"), MB_OK);
}
long Layout::getPosicao(){
	return this->pos;
}

long Layout::getAltura(){
	return this->altura;
}
long Layout::getLargura(){
	return this->largura;
}

void Layout::ReiniciarValoresAcumulados(){
	Layout::posXAcum = 10;
	Layout::posYAcum = 10;
}

