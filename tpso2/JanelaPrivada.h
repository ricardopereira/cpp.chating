#pragma once
#include "JanelaGenerica.h"

class JanelaPrivada : public JanelaGenerica{
private:

public:
	virtual LRESULT myWndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) ;
};
