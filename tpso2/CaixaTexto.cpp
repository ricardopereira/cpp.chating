#include "CaixaTexto.h"
#include "resource.h"

int CaixaTexto::CaixaTextoIdAcum = RESERVADO_CAIXATEXTO_601_700;
CaixaTexto::CaixaTexto(HINSTANCE hInstance, long px, long py, long comprimento, long largura)
:Controlo(hInstance, px, py, comprimento, largura)
{
	this->ControloId = CaixaTextoIdAcum++;
}

CaixaTexto::~CaixaTexto()
{
}
void CaixaTexto::Mostra(HWND hWnd){
	this->hWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		TEXT("EDIT"),
		TEXT(""),
		WS_VISIBLE | WS_CHILD | ES_MULTILINE,
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

std::basic_string<TCHAR> CaixaTexto::getTexto(){
	TCHAR tmp[500];
	GetWindowText(GetDlgItem(this->hWndPai, this->ControloId), tmp, 500);
	this->texto = std::basic_string<TCHAR>(tmp);
	return this->texto;
}



