#include <Windows.h>
#include <tchar.h>
#include <sstream>
#include <string>

#include "JanelaPrincipal.h"
#include "resource.h"
#include "ThreadCaixaDialogoLogin.h"

JanelaPrincipal::JanelaPrincipal() {
	// Init
	this->podeRedimensionar = false;
	this->BotaoEnviarId = -10;
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

LRESULT JanelaPrincipal::performWMessage(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT PtStc; // Ponteiro para estrutura de WM_PAINT
	oTcharStream_t xpto;
	short zDelta;

	// Teste
	ThreadCaixaDialogoLogin *thLogin;

	switch (messg) {
	case WM_CREATE:
		this->MostrarElementos(hWnd);
		this->maxX = GetSystemMetrics(SM_CXSCREEN);
		this->maxY = GetSystemMetrics(SM_CYSCREEN);
		hdc = GetDC(hWnd);
		this->memdc = CreateCompatibleDC(hdc);

		//messenger.cLerInformacaoInicial();
		//messenger.cLerMensagensPublicas(); //ok
		//messenger.cLerMensagensPrivadas(); //ok

		break;
	case WM_ACTIVATE:
		podeRedimensionar = true;
		break;

	case WM_DESTROY:	// Destruir a janela e terminar o programa
		// "PostQuitMessage(Exit Status)"
		PostQuitMessage(0);

		break;
	case WM_SIZE:
		if (podeRedimensionar){
			this->Redimensionar(hWnd);
		}
		break;

	case WM_MOVE:						// Detectar que a janela se moveu
		InvalidateRect(hWnd, NULL, 1);	// Gerar WM_PAINT para actualizar 
		// as coordenadas visíveis na janela
		break;

	case WM_MOUSEWHEEL:
		zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (zDelta > 0) {
			this->AreaMensagens->scrollUp();
		} else {
			this->AreaMensagens->scrollDown();
		}
		InvalidateRect(hWnd, NULL, 1);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &PtStc);
		BitBlt(hdc, 0, 0, this->maxX, this->maxY, this->memdc, 0, 0, SRCCOPY);

		// Teste
		AreaMensagens->doPaint(hdc,hWnd);

		EndPaint(hWnd, &PtStc);
		break;

	case WM_COMMAND:
		// Um comando
		switch (LOWORD(wParam)){
		case ID_CHAT_SAIR:
			this->MostrarElementos(this->_hWnd);
			DestroyWindow(hWnd);
			break;

		case ID_CHAT_LOGIN:

			thLogin = new ThreadCaixaDialogoLogin(this->servidor);
			thLogin->setHwndPai(hWnd);
			thLogin->sethInstance(this->hInst);
			thLogin->LancarThread();

			//if (Autenticar(_T("admin"),_T("admin")))
			//	DialogBox(hInst, (LPCWSTR) IDD_LOGIN, hWnd, (DLGPROC) ThreadCaixaDialogoLogin::DialogoLogin);

			break;

		case ID_CHAT_LOGOUT:
			xpto << this->servidor.cSair();
			MessageBox(0, xpto.str().c_str(), TEXT("YO"), MB_OK);
			break;

		default:
			if (wParam == this->BotaoEnviar->getId()) {
				MessageBox(0, this->txtEnviar->getTexto().c_str(), TEXT("YO"), MB_OK);
			}
			else if (wParam == this->BotaoLike->getId()) {
				this->AreaMensagens->scrollUp();
				InvalidateRect(hWnd, NULL, 1);
			}
			else if (wParam == this->BotaoDislike->getId()) {
				this->AreaMensagens->scrollDown();
				InvalidateRect(hWnd, NULL, 1);
			}
		}
	default:
		// Neste exemplo, para qualquer outra mensagem (p.e. "minimizar", "maximizar",
		// "restaurar") não é efectuado nenhum processamento, apenas se segue 
		// o "default" do Windows DefWindowProc()
		return(DefWindowProc(hWnd, messg, wParam, lParam));
		break;
	}
	return(0);
}

void JanelaPrincipal::MostrarElementos(HWND hWnd) {

	layoutVertical.push_back(new Layout(0.10f));
	layoutVertical.push_back(new Layout(0.70f));
	layoutVertical.push_back(new Layout(0.05f));
	layoutVertical.push_back(new Layout(0.15f));

	layoutHorizontal.push_back(new Layout(0.80f));
	layoutHorizontal.push_back(new Layout(0.025f));
	layoutHorizontal.push_back(new Layout(0.175f));


	for (unsigned int i = 0; i < layoutVertical.size(); i++){
		layoutVertical.at(i)->calcularPosicoesY(hWnd);

	}
	for (unsigned int i = 0; i < layoutHorizontal.size(); i++){
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
