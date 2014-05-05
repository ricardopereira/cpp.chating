#pragma once
#include "DLL/DLLTP.h"
#include <string>
#include <sstream>
typedef std::basic_string<TCHAR> sTchar_t;
typedef std::basic_ostringstream<TCHAR> oTcharStream_t;

class Comunicacao{
private:
	bool autenticado = false;
	bool privilegiosAdmin = false;
	UTILIZADOR utilizadores[NUMUTILIZADORES];
	UTILIZADOR utilizadoresOnline[NUMUTILIZADORES];
	int totalUtilizadores;
	int totalUtilizadoresOnline;
public:
	Comunicacao();
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

};