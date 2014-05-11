#pragma once

#include "Control.h"

class ListBox : public Control
{
private:
	int static ListBoxIdAcum;
public:
	ListBox(HINSTANCE hInstance, long px, long py, long comprimento, long largura);
	~ListBox();

	void Mostra(HWND hWnd);

	void addString(const TCHAR* value);
	void clear();
};

