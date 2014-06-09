#pragma once

#include <vector>

#include "CustomWindow.h"
#include "../ChatingServer/Common.h"
#include "Layout.h"
#include "Button.h"
#include "EditBox.h"
#include "ChatBox.h"
#include "Controller.h"
#include "AssyncThread.h"

using namespace std;

class JanelaPrivada : public CustomWindow
{
private:
	EditBox* txtEnviar;
	HWND BotaoX;
	Button* BotaoLike;
	Button* BotaoDislike;
	Button* BotaoEnviar;
	Button* BotaoCima;
	Button* BotaoBaixo;
	int	BotaoEnviarId;
	ChatBox* AreaMensagens;

	vector<Layout*> layoutVertical;
	vector<Layout*> layoutHorizontal;

	Controller* controller;
	sTchar_t username;

	void Redimensionar(HWND hWnd);
	bool podeRedimensionar;
	AssyncThread* assyncThread;
	HANDLE privateThread;

	JanelaPrivada() { controller = new Controller; /* Memory leak mas é privado :) */ };
public:
	JanelaPrivada(Controller& controller, sTchar_t username, AssyncThread* assyncThread, HANDLE privateThread);
	~JanelaPrivada();

	void Inicializar(HINSTANCE hInst, LPCTSTR ClassName, UINT class_size,
		LPCTSTR MenuName = NULL, UINT style = CS_VREDRAW | CS_HREDRAW);

	virtual void MostrarElementos(HWND hWnd);

	void sendMessage(HWND hWnd, const TCHAR* msg);
	void sendCurrentMessage(HWND hWnd);
	void reset(HWND hWnd);
	void refresh(HWND hWnd);

	virtual int windowX() { return CW_USEDEFAULT; };
	virtual int windowY() { return CW_USEDEFAULT; };
	virtual int windowWidth() { return CW_USEDEFAULT; };
	virtual int windowHeight() { return CW_USEDEFAULT; };

	// Eventos
	virtual void onCreate(HWND hWnd, HDC &hdc);
	virtual void onShow(HWND hWnd);
	virtual bool onClose(HWND hWnd);
	virtual void onActivate(HWND hWnd);
	virtual void onResize(HWND hWnd);
	virtual void onMove(HWND hWnd);
	virtual void onMouseWheelUp(HWND hWnd);
	virtual void onMouseWheelDown(HWND hWnd);
	virtual void onPaint(HWND hWnd, HDC &hdc, RECT &rect);
	virtual void onCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
};
