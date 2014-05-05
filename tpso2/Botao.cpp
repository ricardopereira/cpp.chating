#include "Botao.h"
#include "resource.h"

int Botao::botaoIdAcum = RESERVADO_BOTOES_400_500;
Botao::Botao(HINSTANCE hInstance, long px, long py, long comprimento, long largura)
:Controlo(hInstance, px, py, comprimento, largura)
{
	this->ControloId = botaoIdAcum++;
}

Botao::~Botao()
{
}
void Botao::Mostra(HWND hWnd){
	this->hWnd = CreateWindowEx(
		0,
		TEXT("BUTTON"),
		TEXT("Submeter Letra"),
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
}



