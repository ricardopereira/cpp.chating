#include "resource.h"
#include "Common.h"
#include "ChatBox.h"
#include "ChatBoxItem.h"

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
	for (unsigned int i = 0; i < messages.size(); i++)
		delete messages.at(i);
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
	if (this->scrollY == 0) 
		return;
	this->scrollY -= 10;
	refresh();
}

void ChatBox::scrollDown()
{
	this->scrollY += 10;
	refresh();
}

void ChatBox::refresh()
{
	InvalidateRect(hWnd,&rect,1);
}

void ChatBox::addMessageOnRight(const sTchar_t& username, sTchar_t message)
{
	//SYSTEMTIME hour;
	//GetSystemTime(&hour);

	messages.push_back(new ChatBoxItem(username,message,_T(""),1));
	refresh();
}

void ChatBox::addMessageOnLeft(const sTchar_t& username, sTchar_t message)
{
	messages.push_back(new ChatBoxItem(username,message,_T(""),0));
	refresh();
}

void ChatBox::doPaint(HDC hdc, HWND hWnd)
{
	SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
	SetBkMode(hdc, TRANSPARENT);
	
	// Area total das mensagens
	FillRect(hdc, &rect, backgroundColor);
	//Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

	// Pinta cada mensagem
	for (int idx = 0, i = messages.size()-1; i >= 0; i--, idx++)
		messages.at(i)->doPaint(hdc,hWnd,*this,idx);
}