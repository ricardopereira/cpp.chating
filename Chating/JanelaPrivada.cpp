#include "JanelaPrivada.h"
#include <Windows.h>
#include "resource.h"

LRESULT JanelaPrivada::myWndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	switch (messg) {
	case WM_CREATE:
		MessageBox(NULL, TEXT("Privado"), TEXT("texto"), MB_OK);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:

		break;
	default:
		// Neste exemplo, para qualquer outra mensagem (p.e. "minimizar", "maximizar",
		// "restaurar") não é efectuado nenhum processamento, apenas se segue 
		// o "default" do Windows DefWindowProc()
		return(DefWindowProc(hWnd, messg, wParam, lParam));
		break;
	}
	return(0);
};
