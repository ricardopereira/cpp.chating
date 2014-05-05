#include "JanelaPrivada.h"
#include <Windows.h>
#include "resource.h"

LRESULT JanelaPrivada::myWndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam){
	switch (messg) {
	case WM_CREATE:
		MessageBox(NULL, TEXT("pqp"), TEXT("pqp"), MB_OK);
		break;

	case WM_DESTROY:	// Destruir a janela e terminar o programa
		// "PostQuitMessage(Exit Status)"
		PostQuitMessage(0);

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)){
		case ID_FICHEIRO_SAIR:
			DestroyWindow(hWnd);
			break;

		}

	default:


		// Neste exemplo, para qualquer outra mensagem (p.e. "minimizar", "maximizar",
		// "restaurar") não é efectuado nenhum processamento, apenas se segue 
		// o "default" do Windows DefWindowProc()
		return(DefWindowProc(hWnd, messg, wParam, lParam));
		break;
	}
	return(0);

};
