#include "Botao.h"
#include "resource.h"


Controlo::Controlo(HINSTANCE hInstance, long px, long py, long comprimento, long largura){
	this->pos_x = px;
	this->pos_y = py;
	this->comprimento = comprimento;
	this->largura = largura;
	this->hInstance = hInstance;
}
HWND Controlo::getHwnd(){
	return this->hWnd;
}

void Controlo::setHwndPai(HWND hWnd){
	this->hWndPai = hWnd;
}

int Controlo::getId(){
	return this->ControloId;
}

long Controlo::getX()
{
	return pos_x;
}

long Controlo::getY()
{
	return pos_y;
}

long Controlo::getWidth()
{
	return comprimento;
}

long Controlo::getHeight()
{
	return largura;
}