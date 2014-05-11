#pragma once

#include "DLL/DLLTP.h"
#include "Common.h"
#include <sstream>

class Server
{
private:
	bool autenticado;
	bool privilegiosAdmin;
	sTchar_t loginAutenticado;
	UTILIZADOR utilizadores[NUMUTILIZADORES];
	UTILIZADOR utilizadoresOnline[NUMUTILIZADORES];
	int totalUtilizadores;
	int totalUtilizadoresOnline;

	void loggedIn();
public:
	Server();

	int cAutenticar(TCHAR* login, TCHAR* pass);
	void cLerInformacaoInicial();

	void cEnviarMensagemPublica(const TCHAR *texto);

	// Privado
	int cIniciarConversa(TCHAR *utilizador);
	int cDesligarConversa();
	int cEnviarMensagemPrivada(const TCHAR *texto);

	int cSair();
	int cDesligar();

	void reset();

	int getTotalUtilizadores();
	int getTotalUtilizadoresOnline();

	UTILIZADOR getUtilizador(int index);
	UTILIZADOR getUtilizadorOnline(int index);

	bool getIsAutenticado();
	bool getIsAdministrador();
	const sTchar_t& getLoginAutenticado();
};