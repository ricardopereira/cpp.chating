#pragma once

#include <vector>
#include "Control.h"

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
public:
	ChatBox(HINSTANCE hInstance, long px, long py, long comprimento, long largura);
	~ChatBox();

	void Mostra(HWND hWnd);

	// Teste
	void doPaint(HDC hdc, HWND hWnd);

	void setBackgroundColorRGB(int r, int g, int b);

	int getScrollX();
	int getScrollY();
	void scrollUp();
	void scrollDown();

	void addMessageOnRight(const sTchar_t&  username, sTchar_t message);
	void addMessageOnLeft(const sTchar_t&  username, sTchar_t message);
	void refresh();
};