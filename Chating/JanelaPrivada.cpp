#include <Windows.h>
#include "resource.h"

#include "JanelaPrivada.h"

JanelaPrivada::JanelaPrivada(Server& servidor, const sTchar_t& username) : servidor(&servidor), username(username)
{
	this->podeRedimensionar = false;
	this->BotaoEnviarId = -10;
}

JanelaPrivada::~JanelaPrivada()
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
}

void JanelaPrivada::Inicializar(HINSTANCE hInst, LPCTSTR ClassName, UINT class_size, LPCTSTR MenuName, UINT style)
{
	_WndClsEx.cbSize = sizeof(WNDCLASSEX);
	_WndClsEx.style = style;
	_WndClsEx.lpfnWndProc = CustomWindow::WndProc; //Apontar para a função de tratamento de callback que se encontra na própria classe.
	_WndClsEx.cbClsExtra = 0;
	_WndClsEx.cbWndExtra = sizeof(JanelaPrivada*);
	_WndClsEx.hInstance = hInst;
	_WndClsEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	_WndClsEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	_WndClsEx.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	_WndClsEx.lpszMenuName = MenuName;
	_WndClsEx.lpszClassName = ClassName;
	_WndClsEx.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
}

void JanelaPrivada::sendMessage(HWND hWnd, const TCHAR* msg)
{
	// Verificar se tem mensagem para enviar
	// ToDo: funcao Trim
	if (_tcscmp(msg, TEXT("")))
	{
		if (!this->servidor->getIsAutenticado()) {
			// ToDo: criar método
			sTchar_t text;
			text = TEXT("Tem que estar ligado.");
			MessageBox(hWnd, text.c_str(), TEXT("Erro"), MB_OK | MB_ICONERROR);
			return;
		}

		// Envia mensagem
		this->servidor->cEnviarMensagemPrivada(msg);
				
		// Coloca no ChatBox
		MENSAGEM ultima = LerMensagensPrivadas(); //ToDo: DLL
		AreaMensagens->addMessage(this->servidor->getLoginAutenticado(),ultima);
	}
}

void JanelaPrivada::sendCurrentMessage(HWND hWnd)
{
	if (_tcscmp(this->txtEnviar->getTexto().c_str(), TEXT("")))
	{
		sendMessage(hWnd,this->txtEnviar->getTexto().c_str());
		this->txtEnviar->clear();
	}
}

void JanelaPrivada::reset(HWND hWnd)
{
	this->servidor->reset();
	this->txtEnviar->clear();
	this->AreaMensagens->clear();
	refresh(hWnd);
}

void JanelaPrivada::refresh(HWND hWnd)
{
	HMENU menu = GetMenu(hWnd);

}

// Eventos

void JanelaPrivada::onCreate(HWND hWnd, HDC &hdc)
{
	this->MostrarElementos(hWnd);
	this->maxX = GetSystemMetrics(SM_CXSCREEN);
	this->maxY = GetSystemMetrics(SM_CYSCREEN);
	// ToDo: Validar isto
	hdc = GetDC(hWnd);
	this->memdc = CreateCompatibleDC(hdc);
}

void JanelaPrivada::onShow(HWND hWnd)
{
	// ToDo: cLerInformacaoInicial, cIniciarConversa, cDesligarConversa

	this->servidor->cIniciarConversa(username.c_str());
}

bool JanelaPrivada::onClose(HWND hWnd)
{
	this->servidor->cDesligarConversa();
	return true;
}

void JanelaPrivada::onActivate(HWND hWnd)
{
	podeRedimensionar = true;
}

void JanelaPrivada::onResize(HWND hWnd)
{
	if (podeRedimensionar) {
		this->Redimensionar(hWnd);
	}
}

void JanelaPrivada::onMove(HWND hWnd)
{
	refresh(hWnd);
}

void JanelaPrivada::onMouseWheelUp(HWND hWnd)
{
	this->AreaMensagens->scrollUp();
}

void JanelaPrivada::onMouseWheelDown(HWND hWnd)
{
	this->AreaMensagens->scrollDown();
}

void JanelaPrivada::onPaint(HWND hWnd, HDC &hdc, RECT &rect)
{
	BitBlt(hdc, 0, 0, this->maxX, this->maxY, this->memdc, 0, 0, SRCCOPY);
	// ToDo: Ter em conta o redimensionar
	AreaMensagens->doPaint(hdc,hWnd);
}

void JanelaPrivada::onCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// Um comando
	switch (LOWORD(wParam)) {
	case ID_CHAT_SAIR:
		DestroyWindow(hWnd);
		break;

	default:
		if (wParam == this->BotaoEnviar->getId()) {
			sendCurrentMessage(hWnd);
		}
		else if (wParam == this->BotaoLike->getId()) {
			sendMessage(hWnd,TEXT(":D "));
		}
		else if (wParam == this->BotaoDislike->getId()) {
			sendMessage(hWnd,TEXT(":(  "));
		}
		else if (wParam == this->BotaoCima->getId()) {
			this->AreaMensagens->scrollUp();
		}
		else if (wParam == this->BotaoBaixo->getId()) {
			this->AreaMensagens->scrollDown();
		}
	}
}

void JanelaPrivada::MostrarElementos(HWND hWnd)
{
	layoutVertical.push_back(new Layout(0.10f));
	layoutVertical.push_back(new Layout(0.70f));
	layoutVertical.push_back(new Layout(0.05f));
	layoutVertical.push_back(new Layout(0.15f));

	layoutHorizontal.push_back(new Layout(0.95f));
	layoutHorizontal.push_back(new Layout(0.05f));

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
	this->AreaMensagens->setHwndPai(hWnd);
	this->AreaMensagens->Mostra(hWnd);

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
	this->BotaoLike->setHwndPai(hWnd);
	this->BotaoLike->Mostra(hWnd);

	this->BotaoDislike = new Button(
		this->hInst,
		layoutHorizontal[1]->getPosicao() - 100, //x
		layoutVertical[2]->getPosicao(), //y
		50, //comp
		layoutVertical[2]->getAltura() //larg
		);
	this->BotaoDislike->setTextoBotao(TEXT("Dislike"));
	this->BotaoDislike->setHwndPai(hWnd);
	this->BotaoDislike->Mostra(hWnd);

	this->BotaoEnviar = new Button(
		this->hInst,
		layoutHorizontal[1]->getPosicao() - 50, //x
		layoutVertical[2]->getPosicao(), //y
		50, //comp
		layoutVertical[2]->getAltura() //larg
		);
	this->BotaoEnviar->setTextoBotao(TEXT("Enviar"));
	this->BotaoEnviar->setHwndPai(hWnd);
	this->BotaoEnviar->Mostra(hWnd);

	this->BotaoCima = new Button(
		this->hInst,
		layoutHorizontal[1]->getPosicao(),
		layoutVertical[1]->getPosicao(),
		layoutHorizontal[1]->getLargura(),
		50
		);
	this->BotaoCima->setTextoBotao(TEXT("/\\"));
	this->BotaoCima->setHwndPai(hWnd);
	this->BotaoCima->Mostra(hWnd);

	this->BotaoBaixo = new Button(
		this->hInst,
		layoutHorizontal[1]->getPosicao(),
		layoutVertical[2]->getPosicao() - 10,
		layoutHorizontal[1]->getLargura(),
		50
		);
	this->BotaoBaixo->setTextoBotao(TEXT("\\/"));
	this->BotaoBaixo->setHwndPai(hWnd);
	this->BotaoBaixo->Mostra(hWnd);
}

void JanelaPrivada::Redimensionar(HWND hWnd)
{
	Layout::ReiniciarValoresAcumulados();

	for (unsigned int i = 0; i < layoutVertical.size(); i++){
		layoutVertical.at(i)->calcularPosicoesY(hWnd);

	}
	for (unsigned int i = 0; i < layoutHorizontal.size(); i++){
		layoutHorizontal.at(i)->calcularPosicoesX(hWnd);
	}

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
}