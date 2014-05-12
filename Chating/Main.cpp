// Controlos das vers�es do windows vista/7/8
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include "resource.h"

#include "JanelaPrincipal.h"

// Nome da classe da janela (para programas de uma s� janela, normalmente este 
// nome � igual ao do pr�prio programa)
// "szprogName" � usado mais abaixo na defini��o das propriedades da classe da janela
TCHAR *szProgName = TEXT("Chating");

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	// MSG � uma estrutura definida no Windows para as mensagens
	MSG lpMsg;

	JanelaPrincipal startWindow;
	startWindow.Inicializar(hInst, szProgName, sizeof(JanelaPrincipal*), MAKEINTRESOURCE(IDR_MENU));
	startWindow.Registar();
	startWindow.Criar(hInst, TEXT("Chat P�blico"));
	startWindow.Mostrar();

	while (GetMessage(&lpMsg,NULL,0,0)) {
		TranslateMessage(&lpMsg);			// Pr�-processamento da mensagem
											// p.e. obter c�digo ASCII da tecla
											// premida
		DispatchMessage(&lpMsg);			// Enviar a mensagem traduzida de volta
											// ao Windows, que aguarda at� que a 
											// possa reenviar � fun��o de tratamento
											// da janela, CALLBACK WndProc (mais 
											// abaixo)
	}

	// Retorna-se sempre o par�metro "wParam" da estrutura "lpMsg"
	return((int)lpMsg.wParam);
}