#include "WinApp.hpp"

//---------------------------------------------------------------------------
WApplication::WApplication(HINSTANCE hInst, LPCTSTR ClsName,
	WNDPROC WndPrc, LPCTSTR MenuName, UINT style)
{
	//Definição das características da janela "_WndClsEx"
	_WndClsEx.cbSize = sizeof(WNDCLASSEX);
	_WndClsEx.style = style;
	_WndClsEx.lpfnWndProc = WndPrc;
	_WndClsEx.cbClsExtra = 0;
	_WndClsEx.cbWndExtra = 0;
	_WndClsEx.hInstance = hInst;
	_WndClsEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	_WndClsEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	_WndClsEx.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	_WndClsEx.lpszMenuName = MenuName;
	_WndClsEx.lpszClassName = ClsName;
	_WndClsEx.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
}
//---------------------------------------------------------------------------
void WApplication::Register()
{
	//Registar a classe "_WndClsEx" no Windows
	RegisterClassEx(&_WndClsEx);
}
//---------------------------------------------------------------------------