// Controlos das versões do windows vista/7/8
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include "resource.h"

#include "JanelaPrincipal.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	// MSG é uma estrutura definida no Windows para as mensagens
	MSG lpMsg;

	JanelaPrincipal startWindow;
	startWindow.Inicializar(hInst, TEXT("JanelaPrincipal"), sizeof(JanelaPrincipal*), MAKEINTRESOURCE(IDR_MENU));
	startWindow.Registar();
	startWindow.Criar(hInst, TEXT("Chat Público"));
	startWindow.Mostrar();

	while (GetMessage(&lpMsg,NULL,0,0)) {
		TranslateMessage(&lpMsg);			// Pré-processamento da mensagem
											// p.e. obter código ASCII da tecla
											// premida
		DispatchMessage(&lpMsg);			// Enviar a mensagem traduzida de volta
											// ao Windows, que aguarda até que a 
											// possa reenviar à função de tratamento
											// da janela, CALLBACK WndProc (mais 
											// abaixo)
	}

	// Retorna-se sempre o parâmetro "wParam" da estrutura "lpMsg"
	return((int)lpMsg.wParam);
}