#include "Registry.h"
#include "Common.h"
#include <Windows.h>
#include "../ChatingDll/Dll.h"
#include "Shell.h"
//userReceiver = -2 -> Public

MSG_T *messages;

DWORD nUsers=0;
DWORD nMessages=0;

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
	UTILIZADOR *users;

	//Criar/abrir uma chave em HKEY_LOCAL_MACHINE\Software\MinhaAplicacao
	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\ChatServidor"), 0, NULL, REG_OPTION_VOLATILE, ///REG_OPTION_VOLATILE->Sem persistência
		KEY_ALL_ACCESS, NULL, &chave, &queAconteceu) != ERROR_SUCCESS){

		return Servidor::REGEDIT_OK;
	}
	else
		//Se a chave foi criada, inicializar os valores
	if (queAconteceu == REG_CREATED_NEW_KEY){
		tcout << TEXT("\nChave: HKEY_LOCAL_MACHINE\\Software\\ChatServidor criada\n");

		nUsers++;
		RegSetValueEx(chave, TEXT("nUsers"), 0, REG_DWORD, (LPBYTE)&nUsers, sizeof(DWORD));
		nMessages = 0;
		RegSetValueEx(chave, TEXT("nMessages"), 0, REG_DWORD, (LPBYTE)&nMessages, sizeof(DWORD));

		clientdata.push_back(new ClienteDados(TEXT("admin"), TEXT("admin"), 2, clientdata.size()));
		users = new UTILIZADOR[nUsers];
		_tcscpy_s(users[0].login, clientdata.at(0)->GetUsername().size() *sizeof(TCHAR), clientdata.at(0)->GetUsername().c_str());
		_tcscpy_s(users[0].password, clientdata.at(0)->GetPassword().size() *sizeof(TCHAR), clientdata.at(0)->GetPassword().c_str());
		users[0].tipo = 2;
		RegSetValueEx(chave, TEXT("Users"), 0, REG_BINARY, (LPBYTE)users, sizeof(UTILIZADOR)*nUsers);
		delete []users;

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
			clientdata.push_back(new ClienteDados(users[i].login, users[i].password, users[i].tipo, clientdata.size()));
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
	else
	{
		tcout << TEXT("\nRegisto não foi criado\n");
	}
	return Servidor::REGEDIT_NOT_OK;
}

Servidor::rMsg Registry::SaveData(
	std::vector<ClienteDados*>& clientdata,
	std::vector<Mensagens*>& msgdata
	){

	HKEY chave;
	DWORD queAconteceu;
	UTILIZADOR *users;

	messages = new MSG_T[msgdata.size()];
	users = new UTILIZADOR[clientdata.size()];
	nMessages = msgdata.size();
	nUsers = clientdata.size();
	for (unsigned int i = 0; i < nMessages; i++)
	{
		sTchar_t message_temp = msgdata.at(i)->GetMensagem();
		_tcscpy_s(messages[i].mensagem.texto, message_temp.size()*sizeof(TCHAR), message_temp.c_str());
		messages[i].mensagem.instante = msgdata.at(i)->GetDataMensagem();
		messages[i].userReceiver = msgdata.at(i)->GetReceiver();
		messages[i].userSender = msgdata.at(i)->GetSender();
	}

	for (unsigned int i = 0; i < nUsers; i++)
	{
		sTchar_t username_temp = clientdata.at(i)->GetUsername().c_str();
		sTchar_t password_temp = clientdata.at(i)->GetPassword().c_str();
		_tcscpy_s(users[i].login, username_temp.size()*sizeof(TCHAR), username_temp.c_str());
		_tcscpy_s(users[i].password, password_temp.size()*sizeof(TCHAR), password_temp.c_str());
		users[i].tipo = clientdata.at(i)->GetTipo();
	
	}

	/////TERMINAR ABAIXO
	//Criar/abrir uma chave em HKEY_LOCAL_MACHINE\Software\MinhaAplicacao
	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\ForcaServidor"), 0, NULL, REG_OPTION_VOLATILE, ///REG_OPTION_VOLATILE->Sem persistência
		KEY_ALL_ACCESS, NULL, &chave, &queAconteceu) != ERROR_SUCCESS){


		return Servidor::REGEDIT_NOT_OK;
	}
	else if (queAconteceu == REG_OPENED_EXISTING_KEY){
		//MessageBox(hWnd, TEXT("Chave: HKEY_LOCAL_MACHINE\Software\MinhaAplicacao aberta"), TEXT("Registry"), MB_OK);
		
		RegSetValueEx(chave, TEXT("Messages"), 0, REG_BINARY, (LPBYTE)messages, sizeof(MSG_T)*nMessages);
		RegSetValueEx(chave, TEXT("nMessages"), 0, REG_DWORD, (LPBYTE)&nMessages, sizeof(DWORD));
		
		RegSetValueEx(chave, TEXT("Users"), 0, REG_BINARY, (LPBYTE)users, sizeof(UTILIZADOR)*nUsers);
		RegSetValueEx(chave, TEXT("nUsers"), 0, REG_DWORD, (LPBYTE)&nUsers, sizeof(DWORD));

		RegCloseKey(chave);
		return Servidor::REGEDIT_OK;
	}

	return Servidor::REGEDIT_NOT_OK;
}
