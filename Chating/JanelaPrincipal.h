#pragma once

#include <vector>

#include "CustomWindow.h"
#include "Common.h"
#include "Layout.h"
#include "Button.h"
#include "ListBox.h"
#include "EditBox.h"
#include "ChatBox.h"

using namespace std;

class JanelaPrincipal : public CustomWindow
{
private:
	EditBox*			txtEnviar;
	HWND				BotaoX;
	Button*				BotaoLike;
	Button*				BotaoDislike;
	Button*				BotaoEnviar;
	Button*				BotaoCima;
	Button*				BotaoBaixo;
	int					BotaoEnviarId;
	ChatBox*			AreaMensagens;

	ListBox* ListaUtilizadores;
	vector<Layout*> layoutVertical;
	vector<Layout*> layoutHorizontal;
	Server servidor;
	
	void Redimensionar(HWND hWnd);
	bool podeRedimensionar;
public:
	JanelaPrincipal();
	~JanelaPrincipal();

	void Inicializar(HINSTANCE hInst, LPCTSTR ClassName, UINT class_size,
		LPCTSTR MenuName = NULL, UINT style = CS_VREDRAW | CS_HREDRAW);

	virtual void MostrarElementos(HWND hWnd);
	void login(HWND hWnd);
	void logout(HWND hWnd);
	void sendCurrentMessage(HWND hWnd);
	void reset();

	// Eventos
	virtual void onCreate(HWND hWnd, HDC &hdc);
	virtual void onShow(HWND hWnd);
	virtual void onActivate(HWND hWnd);
	virtual void onResize(HWND hWnd);
	virtual void onMove(HWND hWnd);
	virtual void onMouseWheelUp(HWND hWnd);
	virtual void onMouseWheelDown(HWND hWnd);
	virtual void onPaint(HWND hWnd, HDC &hdc, RECT &rect);
	virtual void onCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
};
