#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"") //Controlos das versões do windows vista/7/8

#include <windows.h>
#include "resource.h"
#include "JanelaPrincipal.h"
#include "JanelaPrivada.h"

// Nome da classe da janela (para programas de uma só janela, normalmente este 
// nome é igual ao do próprio programa)
// "szprogName" é usado mais abaixo na definição das propriedades 
// da classe da janela
TCHAR *szProgName = TEXT("Base");

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, 
				   LPSTR lpCmdLine, int nCmdShow) {
	MSG lpMsg;			// MSG é uma estrutura definida no Windows para as mensagens


	JanelaPrincipal janelaPrincipal;
	janelaPrincipal.Inicializar(hInst, szProgName, sizeof(JanelaPrincipal*), MAKEINTRESOURCE(ID_MENU));
	janelaPrincipal.Registar();
	janelaPrincipal.Criar(hInst, TEXT("Exemplo de Janela Principal em C++"));
	janelaPrincipal.Mostrar();


	//WWindow Wnd;
	//Wnd.Create(hInst, szProgName, TEXT("Exemplo de Janela Principal em C++"), (HWND)HWND_DESKTOP);
	//Wnd.Show();

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
	/*
	//É mais SEGURO o seguinte ciclo de recepção de mensagens, para saber se houve um erro
	BOOL bRet;
	while( (bRet = GetMessage( &lpMsg, NULL, 0, 0 )) != 0)
	{ 
    if (bRet == -1)
    {
        // handle the error and possibly exit
    }
    else
    {
        TranslateMessage(&lpMsg); 
        DispatchMessage(&lpMsg); 
    }
	}*/

	return((int)lpMsg.wParam);		// Retorna-se sempre o parâmetro "wParam" da
								// estrutura "lpMsg"

}




