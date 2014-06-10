#pragma once

#include "../ChatingDll/Dll.h"
#include "../ChatingServer/Common.h"
#include "../Logic/ChatUser.h"
#include <sstream>
#include <vector>

class Controller
{
private:
	ChatUser* userAutenticado;
	sTchar_t privatePartner;

	std::vector<HWND> observers;
	std::vector<ChatUser*> utilizadores;
	std::vector<ChatUser*> utilizadoresOnline;

	void loggedIn(const TCHAR* username, bool isAdmin=false);
protected:
	void destroyUtilizadores();
public:
	Controller();
	~Controller();

	int login(const TCHAR* login, const TCHAR* pass);
	int logout();
	int signUp(const TCHAR* login, const TCHAR* pass);

	void shutdown();
	void shutdownServer();
	void reset();

	void loadConfig(TCHAR* ipserver);
	bool saveConfig(TCHAR* ipserver);

	// Chat Publico
	void cEnviarMensagemPublica(const TCHAR *texto);
	void loadPublicInformation();

	// Chat Privado
	int cIniciarConversa(const TCHAR *utilizador, int flag=0);
	int cDesligarConversa();
	int cEnviarMensagemPrivada(const TCHAR *texto);
	void cCancelarConversa();

	void deleteUtilizador(const TCHAR *username); // Só para admin
	int getTotalUtilizadores();
	int getTotalUtilizadoresOnline();

	ChatUser* addUtilizador(const TCHAR *username);
	ChatUser* addUtilizadorOnline(const TCHAR *username);
	void removeUtilizadorOnline(const TCHAR *username);
	ChatUser* getUtilizador(unsigned int index);
	ChatUser* getUtilizador(const TCHAR *username);
	ChatUser* getUtilizadorOnline(unsigned int index);
	void clearUtilizadoresOnline();
	sTchar_t GetPrivatePartner()const;
	void SetPrivatePartner(const sTchar_t privatePartner);
	bool getIsAutenticado();
	bool getIsAdministrador();
	const ChatUser& getUserAutenticado();

	void addObserver(HWND hWnd);
};