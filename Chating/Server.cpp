#include "Server.h"

Server::Server()
{
	this->autenticado = false;
	this->privilegiosAdmin = false;
}

int Server::cAutenticar(TCHAR* login, TCHAR *pass)
{
	DWORD res = Autenticar(login, pass);

	if (res == 1) {
		this->autenticado = true;
	}
	else if (res == 2) {
		this->autenticado = true;
		this->privilegiosAdmin = true;
	}
	
	return res;
}

int Server::cLerListaUtilizadores()
{
	this->totalUtilizadores = LerListaUtilizadores(this->utilizadoresOnline);
	int x = 0;
	return 1; 
}

int Server::cLerListaUtilizadoresRegistados()
{
	this->totalUtilizadoresOnline = LerListaUtilizadoresRegistados(this->utilizadores);
	int x = 0;
	return 1; 
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
	LerInformacaoInicial();
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