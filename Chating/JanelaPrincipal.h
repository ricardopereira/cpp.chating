#pragma once

#include <vector>

#include "CustomWindow.h"
#include "../ChatingServer/Common.h"
#include "Layout.h"
#include "Button.h"
#include "ListBox.h"
#include "EditBox.h"
#include "ChatBox.h"
#include "ThreadPrivateChat.h"
#include "AssyncThread.h"

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

	ListBox*			ListaUtilizadores;
	vector<Layout*>		layoutVertical;
	vector<Layout*>		layoutHorizontal;
	Controller			controller;

	AssyncThread*		assyncThread;
	ThreadPrivateChat*	privateChat;
	
	void Redimensionar(HWND hWnd);
	bool podeRedimensionar;

	void refreshData();
public:
	JanelaPrincipal();
	~JanelaPrincipal();

	void Inicializar(HINSTANCE hInst, LPCTSTR ClassName, UINT class_size,
		LPCTSTR MenuName = NULL, UINT style = CS_VREDRAW | CS_HREDRAW);

	virtual void MostrarElementos(HWND hWnd);
	void login(HWND hWnd);
	void logout(HWND hWnd);
	void sendMessage(HWND hWnd, const TCHAR* msg);
	void sendCurrentMessage(HWND hWnd);
	void reset(HWND hWnd);
	void refresh(HWND hWnd);
	void showUtilizadores(HWND hWnd);
	void startPrivateChat(HWND hWnd, sTchar_t username, int flag=0);
	void showConfig(HWND hWnd);


	// Eventos
	virtual void onCreate(HWND hWnd, HDC &hdc);
	virtual bool onClose(HWND hWnd);
	virtual void onShow(HWND hWnd);
	virtual void onActivate(HWND hWnd);
	virtual void onResize(HWND hWnd);
	virtual void onMove(HWND hWnd);
	virtual void onMouseWheelUp(HWND hWnd);
	virtual void onMouseWheelDown(HWND hWnd);
	virtual void onPaint(HWND hWnd, HDC &hdc, RECT &rect);
	virtual void onCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual void onCustomMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};
