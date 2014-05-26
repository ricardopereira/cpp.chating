#include "Registry.h"
#include "Common.h"
#include <Windows.h>
#include "../ChatingDll/Dll.h"
#include "Shell.h"
//userReceiver = -2 -> Public
typedef struct msg_t {
	DWORD userSender;
	DWORD userReceiver;
	MENSAGEM mensagem;
}MSG_T;
MSG_T *messages;

DWORD nUsers=0;
DWORD nMessages=0;
UTILIZADOR *users;
//typedef struct appReg {
//
//	
//
//}AppRegistry_t;

Servidor::rMsg Registry::LoadData(
	std::vector<ClienteDados*>& clientdata,
	std::vector<Mensagens*>& msgdata
	){
	HKEY chave;
	DWORD queAconteceu;
	DWORD tipo;
	DWORD tamanho;

	//Criar/abrir uma chave em HKEY_LOCAL_MACHINE\Software\MinhaAplicacao
	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\ChatServidor"), 0, NULL, REG_OPTION_VOLATILE, ///REG_OPTION_VOLATILE->Sem persistência
		KEY_ALL_ACCESS, NULL, &chave, &queAconteceu) != ERROR_SUCCESS){


		return Servidor::REGEDIT_OK;
	}
	else
		//Se a chave foi criada, inicializar os valores
	if (queAconteceu == REG_CREATED_NEW_KEY){
		tcout << TEXT("\nChave: HKEY_LOCAL_MACHINE\\Software\\MinhaAplicação criada\n");

		nUsers++;
		RegSetValueEx(chave, TEXT("nUsers"), 0, REG_DWORD, (LPBYTE)&nUsers, sizeof(DWORD));
		nMessages = 0;
		RegSetValueEx(chave, TEXT("nMessages"), 0, REG_DWORD, (LPBYTE)&nMessages, sizeof(DWORD));


		///jogadores = (JOGADOR *)malloc(sizeof(JOGADOR)); //adicionar o administrador com as credenciais por omissão.
		//inicializarJogador(TEXT("admin"), TEXT("admin"));
		clientdata.push_back(new ClienteDados(TEXT("admin"), TEXT("admin")));
		RegSetValueEx(chave, TEXT("Users"), 0, REG_BINARY, (LPBYTE)users, sizeof(UTILIZADOR)*nUsers);

		//MessageBox(hWnd, TEXT("Joga"), TEXT("Registry"), MB_OK );
		RegCloseKey(chave);
		return Servidor::REGEDIT_FIRST_TIME;
	}
	//Se a chave foi aberta, ler os valores lá guardados
	else if (queAconteceu == REG_OPENED_EXISTING_KEY){
		//MessageBox(hWnd, TEXT("Chave: HKEY_LOCAL_MACHINE\Software\MinhaAplicacao aberta"), TEXT("Registry"), MB_OK);
		tamanho = sizeof(DWORD);
		tipo = REG_DWORD;
		RegQueryValueEx(chave, TEXT("nUsers"), NULL, &tipo, (LPBYTE)&nUsers, &tamanho);

		if (nUsers == 0){
			RegCloseKey(chave);
			return Servidor::REGEDIT_FIRST_TIME;
		}
		//users = (JOGADOR*)malloc(sizeof(JOGADOR)*nJogadores);
		users = new UTILIZADOR[nUsers];

		tamanho = sizeof(UTILIZADOR)*nUsers;
		tipo = REG_BINARY;
		RegQueryValueEx(chave, TEXT("Users"), NULL, &tipo, (LPBYTE)users, &tamanho);

		

		for (DWORD i = 0; i < nUsers; i++)
		{
			clientdata.push_back(new ClienteDados(users[i].login, users[i].password, users[i].tipo));
		}

		tamanho = sizeof(DWORD);
		tipo = REG_DWORD;
		RegQueryValueEx(chave, TEXT("nMessages"), NULL, &tipo, (LPBYTE)&nMessages, &tamanho);
		
		if (nMessages > 0){

			messages = new MSG_T[nMessages];
			tamanho = sizeof(MSG_T)*nMessages;
			tipo = REG_BINARY;
			RegQueryValueEx(chave, TEXT("Messages"), NULL, &tipo, (LPBYTE)messages, &tamanho);

			for (DWORD i = 0; i < nUsers; i++)
			{
				msgdata.push_back(new Mensagens(
					messages[i].mensagem.instante,
					messages[i].userSender,
					messages[i].userReceiver,
					messages[i].mensagem.texto
					));
			}

			delete []messages;
		
		}

		delete []users;
		RegCloseKey(chave);
		return Servidor::REGEDIT_OK;
	}

	return Servidor::REGEDIT_NOT_OK;
}
Servidor::rMsg Registry::SaveData(
	std::vector<ClienteDados*>& clientdata,
	std::vector<Mensagens*>& msgdata
	){

	HKEY chave;
	DWORD queAconteceu;

	messages = new MSG_T[msgdata.size()];
	users = new UTILIZADOR[clientdata.size()];

	for (unsigned int i = 0; i < msgdata.size(); i++)
	{
		sTchar_t message_temp = msgdata.at(i)->GetMensagem();
		_tcscpy_s(messages[i].mensagem.texto, message_temp.size(), message_temp.c_str());
		messages[i].mensagem.instante = msgdata.at(i)->GetDataMensagem();
		messages[i].userReceiver = msgdata.at(i)->GetReceiver();
		messages[i].userSender = msgdata.at(i)->GetSender();
	}

	//Criar/abrir uma chave em HKEY_LOCAL_MACHINE\Software\MinhaAplicacao
	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\ForcaServidor"), 0, NULL, REG_OPTION_VOLATILE, ///REG_OPTION_VOLATILE->Sem persistência
		KEY_ALL_ACCESS, NULL, &chave, &queAconteceu) != ERROR_SUCCESS){


		return Servidor::REGEDIT_NOT_OK;
	}
	else
	if (queAconteceu == REG_OPENED_EXISTING_KEY){
		//MessageBox(hWnd, TEXT("Chave: HKEY_LOCAL_MACHINE\Software\MinhaAplicacao aberta"), TEXT("Registry"), MB_OK);
		RegSetValueEx(chave, TEXT("Jogadores"), 0, REG_BINARY, (LPBYTE)jogadores, sizeof(JOGADOR)*nJogadores);
		RegSetValueEx(chave, TEXT("nJogadores"), 0, REG_DWORD, (LPBYTE)&nJogadores, sizeof(DWORD));

		RegCloseKey(chave);
		return Servidor::REGEDIT_OK;
	}

	return Servidor::REGEDIT_NOT_OK;


}
