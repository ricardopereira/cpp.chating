#pragma once

#include "JanelaGenerica.h"
#include <vector>
#include "Layout.h"
#include "Botao.h"
#include "ListBox.h"
#include "CaixaTexto.h"
#include "ChatBox.h"

class JanelaPrincipal : public JanelaGenerica 
{
private:
	CaixaTexto*			txtEnviar;
	Botao*				BotaoLike;
	HWND				BotaoX;
	Botao*				BotaoDislike;
	Botao*				BotaoEnviar;
	Botao*				BotaoCima;
	Botao*				BotaoBaixo;
	int					BotaoEnviarId;
	ChatBox*			AreaMensagens;

	ListBox* ListaUtilizadores;
	std::vector<Layout*> layoutVertical;
	std::vector<Layout*> layoutHorizontal;
	Comunicacao	messenger;
	
	void Redimensionar(HWND hWnd);
	bool podeRedimensionar;
public:
	JanelaPrincipal();

	virtual LRESULT myWndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
	virtual void MostrarElementos(HWND hWnd);

	void Inicializar(
		HINSTANCE hInst,
		LPCTSTR ClassName,
		UINT class_size,
		LPCTSTR MenuName = NULL,
		UINT style = CS_VREDRAW | CS_HREDRAW
		);
};