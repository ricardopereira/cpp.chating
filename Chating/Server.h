#pragma once

#include "../ChatingDll/Dll.h"
#include "../ChatingServer/Common.h"
#include "../Logic/ChatUser.h"
#include <sstream>
#include <vector>

class Server
{
private:
	bool autenticado;
	bool privilegiosAdmin;
	ChatUser* loginAutenticado;

	std::vector<ChatUser*> utilizadores;
	std::vector<ChatUser*> utilizadoresOnline;

	void loggedIn(const TCHAR* username);
public:
	Server();

	int cAutenticar(const TCHAR* login, const TCHAR* pass);
	int cRegistar(const TCHAR* login, const TCHAR* pass);

	void cLerInformacaoInicial();

	void cEnviarMensagemPublica(const TCHAR *texto);

	// Privado
	int cIniciarConversa(const TCHAR *utilizador);
	int cDesligarConversa();
	int cEnviarMensagemPrivada(const TCHAR *texto);

	int cSair();
	int cDesligar();

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
	const ChatUser& getLoginAutenticado();
};