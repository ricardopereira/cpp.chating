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
public:
	Server();

	int cAutenticar(TCHAR* login, TCHAR* pass);
	void cLerInformacaoInicial();

	void cEnviarMensagemPublica(const TCHAR *texto);
	void cLerMensagensPublicas();

	// Privado
	int cIniciarConversa(TCHAR *utilizador);
	int cDesligarConversa();
	int cEnviarMensagemPrivada(const TCHAR *texto);
	void cLerMensagensPrivadas();

	int cSair();
	int cDesligar();

	void reset();

	bool getIsAutenticado();
	bool getIsAdministrador();
	const sTchar_t& getLoginAutenticado();
};