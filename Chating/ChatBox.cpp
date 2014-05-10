#include "resource.h"
#include "ChatBox.h"
#include "ChatBoxItem.h"
#include "Common.h"

ChatBox::ChatBox(HINSTANCE hInstance, long px, long py, long comprimento, long largura) : Control(hInstance, px, py, comprimento, largura)
{
	this->ControloId = 0; //Unico por janela
	this->rect.left = this->pos_x; //posição inicial x
	this->rect.top = this->pos_y; //posição inicial y
	this->rect.right = this->comprimento; //posição final x
	this->rect.bottom = this->largura; //posição final y
	this->scrollX = 0;
	this->scrollY = 0;
	// Default
	setBackgroundColorRGB(232,233,232);
}

ChatBox::~ChatBox()
{

}

void ChatBox::Mostra(HWND hWnd)
{
	this->hWnd = CreateWindowEx(
		0,
		TEXT("CHATBOX"),
		TEXT(""),
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

void ChatBox::setBackgroundColorRGB(int r, int g, int b)
{
	this->backgroundColor = CreateSolidBrush(RGB(r,g,b));
}

int ChatBox::getScrollX()
{
	return this->scrollX;
}

int ChatBox::getScrollY()
{
	return this->scrollY;
}

void ChatBox::scrollUp()
{
	this->scrollY -= 10;
	InvalidateRect(hWnd,&rect,1);
}

void ChatBox::scrollDown()
{
	this->scrollY += 10;
	InvalidateRect(hWnd,&rect,1);
}

void ChatBox::doPaint(HDC hdc, HWND hWnd)
{
	ChatBoxItem item1(_T("Ricardo Pereira"),_T("Isto é para dar duro"),_T("05-05-2014 05:08"),1);
	ChatBoxItem item2(_T("Ricardo Pereira"),_T("Pode ser?"),_T("05-05-2014 05:12"),1);
	ChatBoxItem item3(_T("Mário Leite"),_T("Vamos a isso"),_T("05-05-2014 05:12"),0);
	ChatBoxItem item4(_T("Ricardo Pereira"),_T("Yeah"),_T("05-05-2014 05:12"),1);
	ChatBoxItem item5(_T("Mário Leite"),_T("Vou fazer commit"),_T("05-05-2014 05:12"),0);
	ChatBoxItem item6(_T("Mário Leite"),_T("Já está"),_T("05-05-2014 05:12"),0);

	SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
	SetBkMode(hdc, TRANSPARENT);
	
	FillRect(hdc, &rect, backgroundColor);
	//Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

	// Teste
	item6.doPaint(hdc,hWnd,*this,0);
	item5.doPaint(hdc,hWnd,*this,1);
	item4.doPaint(hdc,hWnd,*this,2);
	item3.doPaint(hdc,hWnd,*this,3);
	item2.doPaint(hdc,hWnd,*this,4);
	item1.doPaint(hdc,hWnd,*this,5);
}