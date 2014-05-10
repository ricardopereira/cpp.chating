#include "Button.h"
#include "resource.h"

int Button::botaoIdAcum = RESERVADO_BOTOES_400_500;

Button::Button(HINSTANCE hInstance, long px, long py, long comprimento, long largura) : Control(hInstance, px, py, comprimento, largura)
{
	this->ControloId = botaoIdAcum++;
	hfont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
}

Button::~Button()
{
}

void Button::setTextoBotao(const sTchar_t texto)
{
	this->texto = texto;
}

void Button::Mostra(HWND hWnd)
{
	this->hWnd = CreateWindowEx(
		0,
		TEXT("BUTTON"),
		this->texto.c_str(),
		WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		this->pos_x,
		this->pos_y,
		this->comprimento,
		this->largura,
		hWnd,
		(HMENU)this->ControloId,
		hInstance,
		NULL
		);
	SendMessage(this->hWnd, WM_SETFONT, (WPARAM)hfont, MAKELPARAM(TRUE, 0));
}



