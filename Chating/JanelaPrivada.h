#pragma once

#include "CustomWindow.h"

class JanelaPrivada : public CustomWindow
{
private:
public:
	void Inicializar(HINSTANCE hInst, LPCTSTR ClassName, UINT class_size,
		LPCTSTR MenuName = NULL, UINT style = CS_VREDRAW | CS_HREDRAW);

	virtual LRESULT performWMessage(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
};
