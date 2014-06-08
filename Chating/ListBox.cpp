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
		WS_VISIBLE | WS_CHILD | ES_AUTOVSCROLL | WS_VSCROLL | LBS_HASSTRINGS | LBS_NOTIFY, //LBS_HASSTRINGS, LBS_SORT
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

void ListBox::refresh()
{
	InvalidateRect(hWnd, NULL, TRUE);
}

void ListBox::addString(const TCHAR* value)
{
	SendDlgItemMessage(this->hWndPai, this->ControloId, LB_ADDSTRING, 0, (LPARAM)value);

	// Teste
	//LRESULT res;
	//if (res == LB_ERR || res == LBN_ERRSPACE)
}

void ListBox::clear()
{
	HWND hlb = GetDlgItem(this->hWndPai, this->ControloId);
	if (IsWindow(hlb)) {
		int count = SendMessage(hlb,LB_GETCOUNT,0,0);
		if (count == LB_ERR)
			return;
		int idx = count - 1;
		while (count)
			count = SendMessage(hlb,LB_DELETESTRING,(WPARAM)idx--,(LPARAM)0);
	}
}