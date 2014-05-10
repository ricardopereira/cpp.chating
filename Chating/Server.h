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
	int cLerListaUtilizadores();
	int cLerListaUtilizadoresRegistados();
	int cIniciarConversa(TCHAR *utilizador);
	int cDesligarConversa();
	int cEnviarMensagemPrivada(TCHAR *msg);
	void cEnviarMensagemPública(TCHAR *msg);
	void cLerInformacaoInicial();
	void cLerMensagensPublicas();
	void cLerMensagensPrivadas();
	int cSair();
	int cDesligar();

	bool getIsAutenticado();
	const sTchar_t& getLoginAutenticado();
};