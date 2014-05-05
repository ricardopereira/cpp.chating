#pragma once
#include <Windows.h>
class registo
{
public:
	registo();
	~registo();

	static int guardarDados(HWND hWnd, void *dummy);
	static int carregarDados(HWND hWnd, void *dummy);
};

