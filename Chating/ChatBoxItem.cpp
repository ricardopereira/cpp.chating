#include "resource.h"
#include "ChatBoxItem.h"

ChatBoxItem::ChatBoxItem(sTchar_t username, sTchar_t message, sTchar_t info, int isOwner)
{
	this->username = username;
	this->message = message;
	this->info = info;
	this->isOwner = isOwner;
}

ChatBoxItem::~ChatBoxItem()
{

}

sTchar_t ChatBoxItem::getUsername()
{
	return username;
}

sTchar_t ChatBoxItem::getMessage()
{
	return message;
}

sTchar_t ChatBoxItem::getInfo()
{
	return info;
}

int ChatBoxItem::getIsOwner()
{
	return isOwner;
}

void ChatBoxItem::doPaint(HDC hdc, HWND hWnd, ChatBox &chat, int index)
{
	RECT rc;
	int cellHeight = 60;
	int margin = 10;
	int messageMargin = 100;
	UINT userFormat;
	UINT messageFormat;

	// Define o posicionamento da mensagem
	// Margem Esquerda
	if (isOwner)
		rc.left = chat.getX() + margin + messageMargin;
	else
		rc.left = chat.getX() + margin;
	// Margem topo
	rc.top = chat.getHeight() - cellHeight - (index * cellHeight) + chat.getScrollY();
	// Margem direita 
	if (isOwner)
		rc.right = chat.getWidth() - margin;
	else
		rc.right = chat.getWidth() - margin - messageMargin;
	// Margem fundo
	rc.bottom = chat.getHeight() - margin - (index * cellHeight) + chat.getScrollY();

	// Limites
	if (rc.top < chat.getY())
		return;
	if (rc.bottom > chat.getHeight())
		return;

	// Pinta a região da mensagem
	if (isOwner)
		FillRect(hdc, &rc, CreateSolidBrush(RGB(200,200,255)));
	else
		FillRect(hdc, &rc, CreateSolidBrush(RGB(200,200,200)));

	// Determina o posicionamento do texto
	if (isOwner) {
		userFormat = DT_RIGHT | DT_TOP | DT_SINGLELINE;
		messageFormat = DT_RIGHT | DT_BOTTOM | DT_SINGLELINE;
	} else {
		userFormat = DT_LEFT | DT_TOP | DT_SINGLELINE;
		messageFormat = DT_LEFT | DT_BOTTOM | DT_SINGLELINE;
	}

	// Escrever as linhas de informação
	SetTextColor(hdc, RGB(255,0,0));
	DrawText(hdc, username.c_str(), username.length(), &rc, userFormat);
	SetTextColor(hdc, RGB(0,0,0));
	DrawText(hdc, message.c_str(), message.length(), &rc, messageFormat);
}