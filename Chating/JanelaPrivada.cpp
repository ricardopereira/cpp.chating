#include <Windows.h>
#include "resource.h"

#include "JanelaPrivada.h"
sTchar_t titulo_p;
JanelaPrivada::JanelaPrivada(Controller& controller, const sTchar_t username, AssyncThread* assyncThread, HANDLE privateThread, int& flag) : controller(&controller), username(username)
{
	this->podeRedimensionar = false;
	this->BotaoEnviarId = -10;
	this->assyncThread = assyncThread;
	this->privateThread = privateThread;
	this->flag = flag;
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

		// Envia mensagem
		this->controller->cEnviarMensagemPrivada(msg);
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
	this->controller->reset();
	this->txtEnviar->clear();
	this->AreaMensagens->clear();
	refresh(hWnd);
}

void JanelaPrivada::refresh(HWND hWnd)
{

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
	int result;
	result = this->controller->cIniciarConversa(username.c_str(), this->flag);
	if (this->flag == 0){
		if (result == USER_BUSY){ //Utilizador ocupado, fechar janela e terminar thread
			MessageBox(this->parentHWND, TEXT("Utilizador Ocupado"), TEXT("Informação"), MB_OK | MB_ICONINFORMATION);
			SendMessage(hWnd, WM_DESTROY, 0, 0);
			//TODO: Destroy allocated memory
			ExitThread(1);
		}
	}
	else {
		sTchar_t msg = TEXT("Deseja iniciar conversa com ");
		msg.append(this->username.c_str());
		msg.append(TEXT("?"));
		DWORD result;
		result = MessageBox(this->parentHWND, msg.c_str(), TEXT("Informação"), MB_OKCANCEL | MB_ICONQUESTION | MB_APPLMODAL);
		if (result == IDCANCEL){
			this->controller->cCancelarConversa();
			SendMessage(hWnd, WM_DESTROY, 0, 0);
			//TODO: Destroy allocated memory
			ExitThread(1);
		}
	}
	oTcharStream_t temp;
	temp << TEXT("Chat Privado: ") << this->controller->getUserAutenticado().getUsername()
		<< TEXT(" - ") << this->username;

	titulo_p = temp.str();

	SetWindowText(hWnd, titulo_p.c_str());
	this->AreaMensagens->setUsername(this->controller->getUserAutenticado().getUsername());
	LerInformacaoInicial();
}

bool JanelaPrivada::onClose(HWND hWnd)
{
	this->controller->cDesligarConversa();
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

void JanelaPrivada::onCustomMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){

	case WM_USER: //Exclusivo para a janela privada
		MessageBox(hWnd, TEXT("O seu parceiro de conversação abandonou a conversa. Esta janela vai fechar"), TEXT("Aviso"), MB_OK | MB_ICONINFORMATION);
		//Desalocar objectos dinâmicos da janela privada.
		SendMessage(hWnd, WM_DESTROY, 0, 0); //Enviar uma mensagem a si própria para se destruir.
		break;
	case WM_USER + 2: //CANCELAR CHAT
		this->controller->cCancelarConversa();
		MessageBox(hWnd, TEXT("O destinatário não quis iniciar a conversa."), TEXT("Aviso"), MB_OK | MB_ICONINFORMATION);
		SendMessage(hWnd, WM_DESTROY, 0, 0);
		//TODO: Destroy allocated memory
		ExitThread(1);
		break;
	}
}

void JanelaPrivada::MostrarElementos(HWND hWnd)
{
	Layout::ReiniciarValoresAcumulados();
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
		layoutHorizontal[0]->getLargura() /*+ layoutHorizontal[1]->getAltura()*/,
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

	this->assyncThread->setPrivateMessageArea(*this->AreaMensagens);
	this->assyncThread->setPrivateWindowHandle(hWnd);
}

void JanelaPrivada::Redimensionar(HWND hWnd)
{
	Layout::ReiniciarValoresAcumulados();

	for (unsigned int i = 0; i < layoutVertical.size(); i++) {
		layoutVertical.at(i)->calcularPosicoesY(hWnd);
	}

	for (unsigned int i = 0; i < layoutHorizontal.size(); i++) {
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

	// Teste
	//this->AreaMensagens->doResize();
}

void JanelaPrivada::SetParentHWND(HWND parentHWND){
	this->parentHWND = parentHWND;
}
