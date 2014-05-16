#pragma once

#include <vector>
#include "Control.h"
#include "Server.h"

class ChatBoxItem;

using namespace std;

class ChatBox : public Control
{
private:
	RECT rect;
	HBRUSH backgroundColor;
	int scrollX;
	int scrollY;

	vector<ChatBoxItem*> messages;
protected:
	void addMessageOnRight(const sTchar_t&  username, sTchar_t message);
	void addMessageOnLeft(const sTchar_t&  username, sTchar_t message);
	void destroyMessages();
public:
	ChatBox(HINSTANCE hInstance, long px, long py, long comprimento, long largura);
	~ChatBox();

	void Mostra(HWND hWnd);

	void doPaint(HDC hdc, HWND hWnd);
	void doResize();

	void setBackgroundColorRGB(int r, int g, int b);

	int getScrollX();
	int getScrollY();
	void scrollUp();
	void scrollDown();

	void addChat(const sTchar_t& userOwner, CHAT chat);
	void addMessage(const sTchar_t& userOwner, MENSAGEM msg);
	void addMessagePrivate(const sTchar_t& userOwner, MENSAGEM msg);

	void refresh();
	void clear();
};