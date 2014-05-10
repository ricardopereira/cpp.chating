#include "Control.h"
#include "resource.h"

Control::Control(HINSTANCE hInstance, long px, long py, long comprimento, long largura)
{
	this->pos_x = px;
	this->pos_y = py;
	this->comprimento = comprimento;
	this->largura = largura;
	this->hInstance = hInstance;
}

HWND Control::getHwnd(){
	return this->hWnd;
}

void Control::setHwndPai(HWND hWnd)
{
	this->hWndPai = hWnd;
}

int Control::getId(){
	return this->ControloId;
}

long Control::getX()
{
	return pos_x;
}

long Control::getY()
{
	return pos_y;
}

long Control::getWidth()
{
	return comprimento;
}

long Control::getHeight()
{
	return largura;
}