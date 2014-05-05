#pragma once
#include "Controlo.h"

class ListBox : public Controlo
{
private:
	int static ListBoxIdAcum;
public:
	ListBox(HINSTANCE hInstance, long px, long py, long comprimento, long largura);
	~ListBox();
	void Mostra(HWND hWnd);
};

