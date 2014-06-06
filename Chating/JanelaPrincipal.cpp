#include <Windows.h>
#include <tchar.h>
#include <sstream>
#include <string>

#include "JanelaPrincipal.h"
#include "resource.h"

// ToDo: Verificar com a professora se � poss�vel passar uma inst�ncia para uma DialogBox
Server *ptrServidor;

JanelaPrincipal::JanelaPrincipal()
{
	privateChat = NULL;
	assyncThread = NULL;
	// Init
	this->podeRedimensionar = false;
	this->BotaoEnviarId = -10;

	ptrServidor = &this->servidor;
}

JanelaPrincipal::~JanelaPrincipal()
{
	// Libertar elementos layoutVertical: De uma forma
	for (unsigned int i = 0; i < layoutVertical.size(); i++)
		delete layoutVertical.at(i);
	// Libertar elementos layoutHorizontal: De outra forma
	for (vector<Layout*>::iterator it = layoutHorizontal.begin(); it != layoutHorizontal.end(); ++it)
		delete *it;

	delete privateChat;
	delete assyncThread;

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
	_WndClsEx.lpfnWndProc = CustomWindow::WndProc; //Apontar para a fun��o de tratamento de callback que se encontra na pr�pria classe.
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

BOOL CALLBACK DialogLogin(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR login[TAMLOGIN], password[TAMPASS];
	int res = 0;

	switch (message) {
	case WM_INITDIALOG:
		SetDlgItemText(hWnd, IDC_USERNAME, TEXT(""));
		SetDlgItemText(hWnd, IDC_PASSWORD, TEXT(""));
		// Verificar se tem ponteiro da inst�ncia do Server
		_ASSERT(ptrServidor);
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			GetWindowText(GetDlgItem(hWnd, IDC_USERNAME), login, TAMLOGIN);
			GetWindowText(GetDlgItem(hWnd, IDC_PASSWORD), password, TAMPASS);

			if (ptrServidor)
				res = ptrServidor->cAutenticar(login, password);

			if (res == 2) {
				MessageBox(hWnd, TEXT("Login com sucesso: Administrador"), TEXT("Login"), MB_OK | MB_ICONWARNING);
				EndDialog(hWnd,IDOK);
			}
			else if (res == 1) {
				TCHAR text[TAMTEXTO];
				_stprintf_s(text, TAMTEXTO, _T("Login com sucesso: %s"), login);
				MessageBox(hWnd, text, TEXT("Login"), MB_OK | MB_ICONINFORMATION);
				EndDialog(hWnd,IDOK);
			}
			else {
				sTchar_t text;
				text = TEXT("Login inv�lido ou servidor desligado.");
				MessageBox(hWnd, text.c_str(), TEXT("Login"), MB_OK | MB_ICONERROR);
			}
			break;
		case IDCANCEL:
			EndDialog(hWnd,IDCANCEL);
			break;
		case IDC_NEWUSER:
			GetWindowText(GetDlgItem(hWnd, IDC_USERNAME), login, TAMLOGIN);
			GetWindowText(GetDlgItem(hWnd, IDC_PASSWORD), password, TAMPASS);
			res = ptrServidor->cRegistar(login, password);
			if (res == SUCCESS ){
				sTchar_t text;
				text = TEXT("Utilizador registado com sucesso. Pode proceder com a opera��o de login.");
				MessageBox(hWnd, text.c_str(), TEXT("Registar novo utilizador"), MB_OK | MB_ICONINFORMATION);
			}
			else{
				sTchar_t text;
				text = TEXT("N�o foi poss�vel registar o utilizador.");
				MessageBox(hWnd, text.c_str(), TEXT("Registar novo utilizador"), MB_OK | MB_ICONINFORMATION);
			}
			
			break;
		}
		break;
	}
	return 0;
}

BOOL CALLBACK DialogUtilizadores(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return 1;

	case WM_INITDIALOG:
		// Verificar se tem ponteiro da inst�ncia do Server
		_ASSERT(ptrServidor);

		for (int i = 0; i < ptrServidor->getTotalUtilizadores(); i++) {
			SendDlgItemMessage(hWnd, IDC_LIST_UTILIZADORES, LB_ADDSTRING, 0, (LPARAM)ptrServidor->getUtilizador(i).login);
		}
		return 1;
	}
	return 0;
}

void JanelaPrincipal::showUtilizadores(HWND hWnd)
{
	DialogBox(hInst, (LPCWSTR)IDD_UTILIZADORES, hWnd, (DLGPROC)DialogUtilizadores);
}

void JanelaPrincipal::startPrivateChat(HWND hWnd, const sTchar_t& username)
{
	// ToDo: s� pode existir uma janela
	if (privateChat)
		delete privateChat;

	privateChat = new ThreadPrivateChat(this->servidor, username.c_str());
	privateChat->setHwndPai(hWnd);
	privateChat->sethInstance(this->hInst);
	privateChat->LancarThread();
}

void JanelaPrincipal::login(HWND hWnd)
{
	DWORD result = DialogBox(hInst, (LPCWSTR)IDD_LOGIN, hWnd, (DLGPROC)DialogLogin);

	if (result == IDOK && this->servidor.getIsAutenticado())
	{
		// ToDo: ler da instancia do Server
		CHAT chatInit = LerInformacaoInicial();
		AreaMensagens->addChat(this->servidor.getLoginAutenticado(),chatInit);

		// Lista de utilizadores
		for (int i = 0; i < this->servidor.getTotalUtilizadoresOnline(); i++)
			this->ListaUtilizadores->addString(this->servidor.getUtilizadorOnline(i).login);

		// ToDo: Verificar a situa��o do Cancelar no Login

		// Cria thread para receber mensagens
		assyncThread = new AssyncThread(servidor.getLoginAutenticado(),*this->AreaMensagens,*this->ListaUtilizadores);
		assyncThread->LancarThread();
	}
	refresh(hWnd);
}

void JanelaPrincipal::logout(HWND hWnd)
{
	oTcharStream_t res;
	LRESULT opt = MessageBox(hWnd, TEXT("Deseja fazer logout?"), TEXT("Logout"), MB_YESNO | MB_ICONQUESTION);
	if (opt == IDYES) {
		res << this->servidor.cSair(); //Teste: res.str().c_str()
		reset(hWnd);
		//MessageBox(0, TEXT("Logout com sucesso"), TEXT("Logout"), MB_OK | MB_ICONINFORMATION);
	}
}

void JanelaPrincipal::sendMessage(HWND hWnd, const TCHAR* msg)
{
	// Verificar se tem mensagem para enviar
	// ToDo: funcao Trim
	if (_tcscmp(msg, TEXT("")))
	{
		if (!this->servidor.getIsAutenticado()) {
			// ToDo: criar m�todo
			sTchar_t text;
			text = TEXT("Tem que estar ligado.");
			MessageBox(hWnd, text.c_str(), TEXT("Erro"), MB_OK | MB_ICONERROR);
			return;
		}

		// Envia mensagem
		this->servidor.cEnviarMensagemPublica(msg);
				
		// Coloca no ChatBox
		//MENSAGEM ultima = LerMensagensPublicas(); //ToDo: DLL

		// Teste
		MENSAGEM ultima;
		_tcscpy_s(ultima.texto, _tcslen(msg)*sizeof(TCHAR), msg);
		AreaMensagens->addMessagePrivate(this->servidor.getLoginAutenticado(),ultima);
	}
}

void JanelaPrincipal::sendCurrentMessage(HWND hWnd)
{
	if (_tcscmp(this->txtEnviar->getTexto().c_str(), TEXT("")))
	{
		sendMessage(hWnd,this->txtEnviar->getTexto().c_str());
		this->txtEnviar->clear();
	}
}

void JanelaPrincipal::reset(HWND hWnd)
{
	this->servidor.reset();
	this->ListaUtilizadores->clear();
	this->txtEnviar->clear();
	this->AreaMensagens->clear();
	refresh(hWnd);
}

void JanelaPrincipal::refresh(HWND hWnd)
{
	HMENU menu = GetMenu(hWnd);

	if (this->servidor.getIsAutenticado())
	{
		// Login com sucesso
		EnableMenuItem(menu, ID_CHAT_LOGIN, MF_DISABLED);
		EnableMenuItem(menu, ID_CHAT_LOGOUT, MF_ENABLED);

		if (this->servidor.getIsAdministrador())
		{
			EnableMenuItem(menu, ID_ADMINISTRADOR_UTILIZADORES, MF_ENABLED);
		}
		else
			EnableMenuItem(menu, ID_ADMINISTRADOR_UTILIZADORES, MF_DISABLED);
	}
	else {
		// Sem login
		HMENU menu = GetMenu(hWnd);
		EnableMenuItem(menu, ID_CHAT_LOGIN, MF_ENABLED);
		EnableMenuItem(menu, ID_CHAT_LOGOUT, MF_DISABLED);
	}
}


// Eventos

void JanelaPrincipal::onCreate(HWND hWnd, HDC &hdc)
{
	this->MostrarElementos(hWnd);
	this->maxX = GetSystemMetrics(SM_CXSCREEN);
	this->maxY = GetSystemMetrics(SM_CYSCREEN);
	// ToDo: Validar isto
	hdc = GetDC(hWnd);
	this->memdc = CreateCompatibleDC(hdc);
}

void JanelaPrincipal::onShow(HWND hWnd)
{
	login(hWnd);
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
	// ToDo: Ter em conta o redimensionar
	AreaMensagens->doPaint(hdc,hWnd);
}

void JanelaPrincipal::onCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// Um comando
	switch (LOWORD(wParam)) {
	case ID_CHAT_SAIR:
		DestroyWindow(hWnd);
		break;

	case ID_CHAT_LOGIN:
		login(hWnd);
		break;

	case ID_CHAT_LOGOUT:
		logout(hWnd);
		login(hWnd);
		break;

	case ID_ADMINISTRADOR_UTILIZADORES:
		showUtilizadores(hWnd);
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
		else if (LOWORD(wParam) == this->ListaUtilizadores->getId()) {
			// Verificar duplo-clique
			if (HIWORD(wParam) == LBN_DBLCLK) {
				//Preencher caixa de texto com o texto selecionado da listbox
				int i = SendDlgItemMessage(hWnd, IDC_LIST_UTILIZADORES, LB_GETCURSEL, 0, 0);
				TCHAR usernameAux[30];
				SendDlgItemMessage(hWnd, this->ListaUtilizadores->getId(), LB_GETTEXT, i, (LPARAM)usernameAux);
				// Abre chat privado
				startPrivateChat(hWnd,usernameAux);
			}
		}
	}
}

void JanelaPrincipal::MostrarElementos(HWND hWnd)
{
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
	this->AreaMensagens->setHwndPai(hWnd);
	this->AreaMensagens->Mostra(hWnd);

	this->ListaUtilizadores = new ListBox(
		this->hInst,
		layoutHorizontal[2]->getPosicao(),
		layoutVertical[1]->getPosicao(),
		layoutHorizontal[2]->getLargura(),
		layoutVertical[1]->getAltura() + layoutVertical[2]->getAltura() + layoutVertical[3]->getAltura()
		);
	this->ListaUtilizadores->setHwndPai(hWnd);
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
	/*SetWindowPos(
		this->AreaMensagens->getHwnd(),
		0,
		layoutHorizontal[0]->getPosicao(),
		layoutVertical[1]->getPosicao(),
		layoutHorizontal[0]->getLargura(),
		layoutVertical[1]->getAltura(),
		SWP_NOSENDCHANGING | SWP_NOACTIVATE | SWP_NOZORDER
		); */

	//BringWindowToTop(txtEnviar->getHwnd());
}
