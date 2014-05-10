#pragma once

#include <vector>

#include "CustomWindow.h"
#include "Layout.h"
#include "Button.h"
#include "ListBox.h"
#include "EditBox.h"
#include "ChatBox.h"
#include "Server.h"

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
	std::vector<Layout*> layoutVertical;
	std::vector<Layout*> layoutHorizontal;
	Server servidor;
	
	void Redimensionar(HWND hWnd);
	bool podeRedimensionar;
public:
	JanelaPrincipal();

	void Inicializar(HINSTANCE hInst, LPCTSTR ClassName, UINT class_size,
		LPCTSTR MenuName = NULL, UINT style = CS_VREDRAW | CS_HREDRAW);

	virtual LRESULT performWMessage(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
	virtual void MostrarElementos(HWND hWnd);
};
