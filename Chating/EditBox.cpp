#include "EditBox.h"
#include "resource.h"

int EditBox::CaixaTextoIdAcum = RESERVADO_CAIXATEXTO_601_700;

EditBox::EditBox(HINSTANCE hInstance, long px, long py, long comprimento, long largura) : Control(hInstance, px, py, comprimento, largura)
{
	this->ControloId = CaixaTextoIdAcum++;
	hfont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
}

EditBox::~EditBox()
{
}

void EditBox::Mostra(HWND hWnd)
{
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
	SendMessage(this->hWnd, WM_SETFONT, (WPARAM)hfont, MAKELPARAM(TRUE, 0));
}

std::basic_string<TCHAR> EditBox::getTexto()
{
	TCHAR tmp[500];
	//GetWindowText(GetDlgItem(this->hWndPai, this->ControloId), tmp, 500);
	GetDlgItemText(this->hWndPai, this->ControloId, tmp, 500);
	this->texto = std::basic_string<TCHAR>(tmp);
	return this->texto;
}

void EditBox::setTexto(LPCWSTR texto)
{
	SetDlgItemText(this->hWndPai, this->ControloId, texto);
}

void EditBox::clear()
{
	SetDlgItemText(this->hWndPai, this->ControloId, TEXT(""));
}

