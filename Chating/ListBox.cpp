#include "ListBox.h"
#include "resource.h"

int ListBox::ListBoxIdAcum = RESERVADO_LISTBOXES_501_600;

ListBox::ListBox(HINSTANCE hInstance, long px, long py, long comprimento, long largura) : Control(hInstance, px, py, comprimento, largura)
{
	this->ControloId = ListBoxIdAcum++;
}

ListBox::~ListBox()
{
}

void ListBox::Mostra(HWND hWnd)
{
	this->hWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		TEXT("LISTBOX"),
		NULL,
		WS_VISIBLE | WS_CHILD | ES_AUTOVSCROLL | WS_VSCROLL,
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
