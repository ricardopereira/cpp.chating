#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"") //Controlos das vers�es do windows vista/7/8

#include <windows.h>
#include "resource.h"
#include "JanelaPrincipal.h"
#include "JanelaPrivada.h"

// Nome da classe da janela (para programas de uma s� janela, normalmente este 
// nome � igual ao do pr�prio programa)
// "szprogName" � usado mais abaixo na defini��o das propriedades 
// da classe da janela
TCHAR *szProgName = TEXT("Base");

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, 
				   LPSTR lpCmdLine, int nCmdShow) {
	MSG lpMsg;			// MSG � uma estrutura definida no Windows para as mensagens


	JanelaPrincipal janelaPrincipal;
	janelaPrincipal.Inicializar(hInst, szProgName, sizeof(JanelaPrincipal*), MAKEINTRESOURCE(ID_MENU));
	janelaPrincipal.Registar();
	janelaPrincipal.Criar(hInst, TEXT("Exemplo de Janela Principal em C++"));
	janelaPrincipal.Mostrar();


	//WWindow Wnd;
	//Wnd.Create(hInst, szProgName, TEXT("Exemplo de Janela Principal em C++"), (HWND)HWND_DESKTOP);
	//Wnd.Show();

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
	/*
	//� mais SEGURO o seguinte ciclo de recep��o de mensagens, para saber se houve um erro
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

	return((int)lpMsg.wParam);		// Retorna-se sempre o par�metro "wParam" da
								// estrutura "lpMsg"

}




