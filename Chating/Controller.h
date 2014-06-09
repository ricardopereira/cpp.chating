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
	int shutdown();

	void loadPublicInformation();

	void cEnviarMensagemPublica(const TCHAR *texto);

	// Chat Privado
	int cIniciarConversa(const TCHAR *utilizador);
	int cDesligarConversa();
	int cEnviarMensagemPrivada(const TCHAR *texto);

	void reset();

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

	bool getIsAutenticado();
	bool getIsAdministrador();
	const ChatUser& getUserAutenticado();
};