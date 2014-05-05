#include "registo.h"
#include <vector>

//namespace std;


registo::registo()
{
}


registo::~registo()
{
}

int registo::carregarDados(HWND hWnd, void *dummy){

	HKEY chave;
	DWORD queAconteceu;
	DWORD tipo;
	DWORD tamanho;
	//Criar/abrir uma chave em HKEY_LOCAL_MACHINE\Software\MinhaAplicacao
	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\ForcaCliente"), 0, NULL, REG_OPTION_VOLATILE, ///REG_OPTION_VOLATILE->Sem persistência
		KEY_ALL_ACCESS, NULL, &chave, &queAconteceu) != ERROR_SUCCESS)
		MessageBox(hWnd, TEXT("Erro ao criar/abrir chave"), TEXT("Registry"), MB_OK);
	else
		//Se a chave foi criada, inicializar os valores
	if (queAconteceu == REG_CREATED_NEW_KEY){
		MessageBox(hWnd, TEXT("Chave: HKEY_LOCAL_MACHINE\\Software\\MinhaAplicação criada"), TEXT("Registry"), MB_OK);

		//Inicializar estrutura
		/*pJogador->hPipe = NULL;
		_tcscpy_s(pJogador->nomePipe, 30, TEXT(""));*/
		/*pJogador->NpalavrasDescobertas = 10;
		pJogador->numeroPartidas = 20;
		_tcscpy_s(pJogador->password, 20, TEXT("admin"));
		_tcscpy_s(pJogador->username, 20, TEXT("admin"));
		pJogador->login_efectuado = FALSE;*/


	/*	RegSetValueEx(chave, TEXT("Jogador"), 0, REG_BINARY, (LPBYTE)pJogador, sizeof(JOGADOR));*/

		//MessageBox(hWnd, TEXT("Joga"), TEXT("Registry"), MB_OK );
		RegCloseKey(chave);
		/*return REGEDIT_NAO_INI;*/
	}
	//Se a chave foi aberta, ler os valores lá guardados
	else if (queAconteceu == REG_OPENED_EXISTING_KEY){
		//MessageBox(hWnd, TEXT("Chave: HKEY_LOCAL_MACHINE\Software\MinhaAplicacao aberta"), TEXT("Registry"), MB_OK);
		/*tamanho = sizeof(JOGADOR);*/
		tipo = REG_BINARY;
		/*RegQueryValueEx(chave, TEXT("Jogador"), NULL, &tipo, (LPBYTE)pJogador, &tamanho);*/


		MessageBox(hWnd, TEXT("Dados do último inicio de sessão carregados! "), TEXT("Lido do Registry"), MB_OK | MB_ICONINFORMATION);
		RegCloseKey(chave);
		/*return REGEDIT_OK;*/
	}
	return 0;
}

	int registo::guardarDados(HWND hWnd, void *dummy){
		return 1;
	}
