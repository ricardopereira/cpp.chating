#include "Botao.h"
#include "resource.h"

int Botao::botaoIdAcum = RESERVADO_BOTOES_400_500;

Botao::Botao(HINSTANCE hInstance, long px, long py, long comprimento, long largura) : Controlo(hInstance, px, py, comprimento, largura)
{
	this->ControloId = botaoIdAcum++;
	hfont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
}

Botao::~Botao()
{
}

void Botao::setTextoBotao(const sTchar_t texto)
{
	this->texto = texto;
}

void Botao::Mostra(HWND hWnd)
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



