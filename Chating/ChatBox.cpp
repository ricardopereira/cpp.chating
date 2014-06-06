#include "resource.h"
#include "../ChatingServer/Common.h"
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
	destroyMessages();
}

void ChatBox::destroyMessages()
{
	for (unsigned int i = 0; i < messages.size(); i++)
		delete messages.at(i);
	messages.clear();
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

void ChatBox::clear()
{
	destroyMessages();
	refresh();
}

void ChatBox::addChat(const sTchar_t& userOwner, CHAT chat)
{
	// ToDo: diferenciar de publico e privado
	int linha = 0;
	// Mensagens
	for (; _tcscmp(chat.publicas[linha].texto, TEXT("")) && linha <= NUMMSGSPUBLICAS; linha++) {
		// Adicionar mensagem
		addMessage(userOwner,chat.publicas[linha]);
	}
}

void ChatBox::addMessage(const sTchar_t& userOwner, MENSAGEM msg)
{
	TCHAR delimiter = ':';
	bool userIsReady = false;
	sTchar_t user;
	sTchar_t message;

	// Interpretar mensagem do servidor
	//Retirar o nome do utilizador e a mensagem
	for (unsigned int i=0; i <= (unsigned int)_tcslen(msg.texto); i++)
	{
		if (!userIsReady && msg.texto[i] == delimiter) {
			userIsReady = true;
			continue;
		}
		if (userIsReady && message.size() == 0 && msg.texto[i-1] == delimiter)
			continue;
		if (msg.texto[i] == '\0')
			continue;

		if (userIsReady)
			// Mensagem
			message.push_back(msg.texto[i]);
		else
			// Utilizador
			user.push_back(msg.texto[i]);
	}

	if (user == userOwner)
		// Mensagem do próprio
		addMessageOnRight(user,message);
	else
		// Mensagem de outro
		addMessageOnLeft(user,message);

	// ToDo: adicionar o Instante no item da mensagem

	//_stprintf_s(str, 2 * TAMTEXTO, TEXT("(%02d/%02d/%d-%02d:%02d:%02d) %s"), chatInit.publicas[linha].instante.dia,
	//	mychat.publicas[linha].instante.mes, mychat.publicas[linha].instante.ano, mychat.publicas[linha].instante.hora,
	//	mychat.publicas[linha].instante.minuto, mychat.publicas[linha].instante.segundo, );
}

void ChatBox::addMessagePrivate(const sTchar_t& userOwner, MENSAGEM msg)
{
	// Mensagem do próprio
	addMessageOnRight(userOwner,msg.texto);
}

void ChatBox::addMessageOnRight(const sTchar_t& username, sTchar_t message)
{
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

void ChatBox::doResize()
{

}