#include <Windows.h>
#include <tchar.h>
#include <sstream>
#include <string>

#include "JanelaPrincipal.h"
#include "resource.h"
#include "ThreadCaixaDialogoLogin.h"

JanelaPrincipal::JanelaPrincipal()
{
	// Init
	this->podeRedimensionar = false;
	this->BotaoEnviarId = -10;
}

JanelaPrincipal::~JanelaPrincipal()
{
	// Libertar elementos layoutVertical: De uma forma
	for (unsigned int i = 0; i < layoutVertical.size(); i++)
		delete layoutVertical.at(i);
	// Libertar elementos layoutHorizontal: De outra forma
	for (vector<Layout*>::iterator it = layoutHorizontal.begin(); it != layoutHorizontal.end(); ++it)
		delete *it;

	delete txtEnviar;
	delete BotaoLike;
	delete BotaoDislike;
	delete BotaoEnviar;
	delete BotaoCima;
	delete BotaoBaixo;
	delete AreaMensagens;
	delete ListaUtilizadores;
}

void JanelaPrincipal::Inicializar(HINSTANCE hInst, LPCTSTR ClassName, UINT class_size, LPCTSTR MenuName, UINT style)
{
	_WndClsEx.cbSize = sizeof(WNDCLASSEX);
	_WndClsEx.style = style;
	_WndClsEx.lpfnWndProc = CustomWindow::WndProc; //Apontar para a função de tratamento de callback que se encontra na própria classe.
	_WndClsEx.cbClsExtra = 0;
	_WndClsEx.cbWndExtra = sizeof(JanelaPrincipal*);
	_WndClsEx.hInstance = hInst;
	_WndClsEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	_WndClsEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	_WndClsEx.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	_WndClsEx.lpszMenuName = MenuName;
	_WndClsEx.lpszClassName = ClassName;
	_WndClsEx.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
}


// Eventos

void JanelaPrincipal::onCreate(HWND hWnd, HDC &hdc)
{
	this->MostrarElementos(hWnd);
	this->maxX = GetSystemMetrics(SM_CXSCREEN);
	this->maxY = GetSystemMetrics(SM_CYSCREEN);
	hdc = GetDC(hWnd);
	this->memdc = CreateCompatibleDC(hdc);

	//Teste
	AreaMensagens->addMessageOnRight(_T("Ricardo Pereira"),_T("Isto é para dar duro"));
	AreaMensagens->addMessageOnRight(_T("Ricardo Pereira"),_T("Pode ser?"));
	AreaMensagens->addMessageOnLeft(_T("Mário Leite"),_T("Vamos a isso"));
	AreaMensagens->addMessageOnRight(_T("Ricardo Pereira"),_T("Yeah"));
	AreaMensagens->addMessageOnLeft(_T("Mário Leite"),_T("Vou fazer commit"));
	AreaMensagens->addMessageOnLeft(_T("Mário Leite"),_T("Já está"));
}

void JanelaPrincipal::onShow(HWND hWnd)
{
	//DialogBox(hInst, (LPCWSTR) IDD_LOGIN, hWnd, (DLGPROC) ThreadCaixaDialogoLogin::DialogoLogin);
}

void JanelaPrincipal::onActivate(HWND hWnd)
{
	podeRedimensionar = true;
}

void JanelaPrincipal::onResize(HWND hWnd)
{
	if (podeRedimensionar) {
		this->Redimensionar(hWnd);
	}
}

void JanelaPrincipal::onMove(HWND hWnd)
{
	refresh(hWnd);
}

void JanelaPrincipal::onMouseWheelUp(HWND hWnd)
{
	this->AreaMensagens->scrollUp();
}

void JanelaPrincipal::onMouseWheelDown(HWND hWnd)
{
	this->AreaMensagens->scrollDown();
}

void JanelaPrincipal::onPaint(HWND hWnd, HDC &hdc, RECT &rect)
{
	BitBlt(hdc, 0, 0, this->maxX, this->maxY, this->memdc, 0, 0, SRCCOPY);
	// Teste
	AreaMensagens->doPaint(hdc,hWnd);
}

void JanelaPrincipal::onCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	oTcharStream_t xpto;
	ThreadCaixaDialogoLogin *thLogin;

	// Um comando
	switch (LOWORD(wParam)) {
	case ID_CHAT_SAIR:
		this->MostrarElementos(this->_hWnd);
		DestroyWindow(hWnd);
		break;

	case ID_CHAT_LOGIN:
		thLogin = new ThreadCaixaDialogoLogin(this->servidor);
		thLogin->setHwndPai(hWnd);
		thLogin->sethInstance(this->hInst);
		thLogin->LancarThread();
		break;

	case ID_CHAT_LOGOUT:
		xpto << this->servidor.cSair();
		MessageBox(0, xpto.str().c_str(), TEXT("YO"), MB_OK);
		break;

	default:
		if (wParam == this->BotaoEnviar->getId()) {
			// Verificar se tem mensagem para enviar
			// ToDo: funcao Trim
			if (_tcscmp(this->txtEnviar->getTexto().c_str(), TEXT("")))
			{
				// Coloca no ChatBox
				AreaMensagens->addMessageOnRight(servidor.getLoginAutenticado(),this->txtEnviar->getTexto());
				this->txtEnviar->clear();
			}
		}
		else if (wParam == this->BotaoCima->getId()) {
			this->AreaMensagens->scrollUp();
		}
		else if (wParam == this->BotaoBaixo->getId()) {
			this->AreaMensagens->scrollDown();
		}
	}
}

void JanelaPrincipal::MostrarElementos(HWND hWnd) {

	layoutVertical.push_back(new Layout(0.10f));
	layoutVertical.push_back(new Layout(0.70f));
	layoutVertical.push_back(new Layout(0.05f));
	layoutVertical.push_back(new Layout(0.15f));

	layoutHorizontal.push_back(new Layout(0.80f));
	layoutHorizontal.push_back(new Layout(0.025f));
	layoutHorizontal.push_back(new Layout(0.175f));


	for (unsigned int i = 0; i < layoutVertical.size(); i++) {
		layoutVertical.at(i)->calcularPosicoesY(hWnd);
	}

	for (unsigned int i = 0; i < layoutHorizontal.size(); i++) {
		layoutHorizontal.at(i)->calcularPosicoesX(hWnd);
	}

	this->AreaMensagens = new ChatBox(
		this->hInst,
		layoutHorizontal[0]->getPosicao(),
		layoutVertical[1]->getPosicao(),
		layoutHorizontal[0]->getLargura(),
		layoutVertical[1]->getAltura()
		);
	this->AreaMensagens->Mostra(hWnd);

	this->ListaUtilizadores = new ListBox(
		this->hInst,
		layoutHorizontal[2]->getPosicao(),
		layoutVertical[1]->getPosicao(),
		layoutHorizontal[2]->getLargura(),
		layoutVertical[1]->getAltura() + layoutVertical[2]->getAltura() + layoutVertical[3]->getAltura()
		);
	this->ListaUtilizadores->Mostra(hWnd);

	this->txtEnviar = new EditBox(
		this->hInst,
		layoutHorizontal[0]->getPosicao(),
		layoutVertical[3]->getPosicao(),
		layoutHorizontal[0]->getLargura() + layoutHorizontal[1]->getAltura(),
		layoutVertical[3]->getAltura()
		);
	this->txtEnviar->setHwndPai(hWnd);
	this->txtEnviar->Mostra(hWnd);

	this->BotaoLike = new Button(
		this->hInst,
		layoutHorizontal[1]->getPosicao() - 150, //x
		layoutVertical[2]->getPosicao(), //y
		50,
		layoutVertical[2]->getAltura() //larg
		);
	this->BotaoLike->setTextoBotao(TEXT("Like"));
	this->BotaoLike->Mostra(hWnd);

	this->BotaoDislike = new Button(
		this->hInst,
		layoutHorizontal[1]->getPosicao() - 100, //x
		layoutVertical[2]->getPosicao(), //y
		50, //comp
		layoutVertical[2]->getAltura() //larg
		);
	this->BotaoDislike->setTextoBotao(TEXT("Dislike"));
	this->BotaoDislike->Mostra(hWnd);

	this->BotaoEnviar = new Button(
		this->hInst,
		layoutHorizontal[1]->getPosicao() - 50, //x
		layoutVertical[2]->getPosicao(), //y
		50, //comp
		layoutVertical[2]->getAltura() //larg
		);
	this->BotaoEnviar->setTextoBotao(TEXT("Enviar"));
	this->BotaoEnviar->Mostra(hWnd);

	this->BotaoCima = new Button(
		this->hInst,
		layoutHorizontal[1]->getPosicao(),
		layoutVertical[1]->getPosicao(),
		layoutHorizontal[1]->getLargura(),
		50
		);
	this->BotaoCima->setTextoBotao(TEXT("/\\"));
	this->BotaoCima->Mostra(hWnd);

	this->BotaoBaixo = new Button(
		this->hInst,
		layoutHorizontal[1]->getPosicao(),
		layoutVertical[2]->getPosicao() - 10,
		layoutHorizontal[1]->getLargura(),
		50
		);
	this->BotaoBaixo->setTextoBotao(TEXT("\\/"));
	this->BotaoBaixo->Mostra(hWnd);
}

void JanelaPrincipal::Redimensionar(HWND hWnd)
{
	Layout::ReiniciarValoresAcumulados();

	for (unsigned int i = 0; i < layoutVertical.size(); i++){
		layoutVertical.at(i)->calcularPosicoesY(hWnd);

	}
	for (unsigned int i = 0; i < layoutHorizontal.size(); i++){
		layoutHorizontal.at(i)->calcularPosicoesX(hWnd);
	}

	SetWindowPos(
		this->ListaUtilizadores->getHwnd(),
		0,
		layoutHorizontal[2]->getPosicao(),
		layoutVertical[1]->getPosicao(),
		layoutHorizontal[2]->getLargura(),
		layoutVertical[1]->getAltura() + layoutVertical[2]->getAltura() + layoutVertical[3]->getAltura(),
		SWP_NOSENDCHANGING | SWP_NOACTIVATE | SWP_NOZORDER
		);

	SetWindowPos(
		this->txtEnviar->getHwnd(),
		0,
		layoutHorizontal[0]->getPosicao(),
		layoutVertical[3]->getPosicao(),
		layoutHorizontal[0]->getLargura(),
		layoutVertical[3]->getAltura(),
		SWP_NOSENDCHANGING | SWP_NOACTIVATE | SWP_NOZORDER
		);

	SetWindowPos(
		this->BotaoLike->getHwnd(),
		0,
		layoutHorizontal[1]->getPosicao() - 150, //x
		layoutVertical[2]->getPosicao(), //y
		50/*layoutHorizontal[1]->getLargura()*/, //comp
		layoutVertical[2]->getAltura(), //larg
		SWP_NOSENDCHANGING | SWP_NOACTIVATE | SWP_NOZORDER
		);

	SetWindowPos(
		this->BotaoDislike->getHwnd(),
		0,
		layoutHorizontal[1]->getPosicao() - 100, //x
		layoutVertical[2]->getPosicao(), //y
		50, //comp
		layoutVertical[2]->getAltura(), //larg
		SWP_NOSENDCHANGING | SWP_NOACTIVATE | SWP_NOZORDER
		);

	SetWindowPos(
		this->BotaoEnviar->getHwnd(),
		0,
		layoutHorizontal[1]->getPosicao() - 50, //x
		layoutVertical[2]->getPosicao(), //y
		50, //comp
		layoutVertical[2]->getAltura(), //larg
		SWP_NOSENDCHANGING | SWP_NOACTIVATE | SWP_NOZORDER
		);

	SetWindowPos(
		this->BotaoCima->getHwnd(),
		0,
		layoutHorizontal[1]->getPosicao(), //x
		layoutVertical[1]->getPosicao(), //y
		layoutHorizontal[1]->getLargura(), //comp
		50, //larg
		SWP_NOSENDCHANGING | SWP_NOACTIVATE | SWP_NOZORDER
		);

	SetWindowPos(
		this->BotaoBaixo->getHwnd(),
		0,
		layoutHorizontal[1]->getPosicao(),
		layoutVertical[2]->getPosicao() - 20,
		layoutHorizontal[1]->getLargura(),
		50,
		SWP_NOSENDCHANGING | SWP_NOACTIVATE | SWP_NOZORDER
		);

	//BringWindowToTop(txtEnviar->getHwnd());
}
