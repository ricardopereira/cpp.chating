#include "Server.h"

Server::Server()
{
	this->autenticado = false;
	this->privilegiosAdmin = false;
	// Default
	this->loginAutenticado = _T("Default");
}

bool Server::getIsAutenticado()
{
	return this->autenticado;
}

const sTchar_t& Server::getLoginAutenticado()
{
	return this->loginAutenticado;
}

int Server::cAutenticar(TCHAR* login, TCHAR *pass)
{
	int res = Autenticar(login, pass);

	if (res == 1) {
		this->autenticado = true;
		this->privilegiosAdmin = false;

		this->totalUtilizadoresOnline = LerListaUtilizadores(this->utilizadoresOnline);
		this->totalUtilizadores = LerListaUtilizadoresRegistados(this->utilizadores);
	}
	else if (res == 2) {
		this->autenticado = true;
		this->privilegiosAdmin = true;
	}
	else {
		this->autenticado = false;
		this->privilegiosAdmin = false;
	}
	return res;
}

int Server::cIniciarConversa(TCHAR *utilizador)
{ 
	return 1;
}

int Server::cDesligarConversa()
{ 
	return 1;
}

int Server::cEnviarMensagemPrivada(TCHAR *msg)
{ 
	return 1;
}

void Server::cEnviarMensagemPública(TCHAR *msg)
{
	return;
}

void Server::cLerInformacaoInicial()
{
	CHAT info = LerInformacaoInicial();
	return;
}

void Server::cLerMensagensPublicas()
{
	MENSAGEM temp = LerMensagensPublicas();
	int x = 0;
}

void Server::cLerMensagensPrivadas()
{
	MENSAGEM temp = LerMensagensPrivadas();
	int x = 0;
}

int Server::cSair()
{ 
	return Sair();
}

int Server::cDesligar()
{ 
	return 1;
}