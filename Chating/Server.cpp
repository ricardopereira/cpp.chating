#include "Server.h"

Server::Server()
{
	reset();
}

bool Server::getIsAutenticado()
{
	return this->autenticado;
}

bool Server::getIsAdministrador()
{
	return this->privilegiosAdmin;
}

const sTchar_t& Server::getLoginAutenticado()
{
	return this->loginAutenticado;
}

void Server::reset()
{
	this->autenticado = false;
	this->privilegiosAdmin = false;
	this->loginAutenticado = TEXT("");
}

int Server::cAutenticar(TCHAR* login, TCHAR *pass)
{
	int res = Autenticar(login, pass);

	if (res == 1) {
		this->autenticado = true;
		this->privilegiosAdmin = false;
		this->loginAutenticado = login;

		this->totalUtilizadoresOnline = LerListaUtilizadores(this->utilizadoresOnline);
		this->totalUtilizadores = LerListaUtilizadoresRegistados(this->utilizadores);
	}
	else if (res == 2) {
		this->autenticado = true;
		this->privilegiosAdmin = true;
		this->loginAutenticado = login; //Administrador
	}
	else {
		this->autenticado = false;
		this->privilegiosAdmin = false;
		this->loginAutenticado = TEXT("");
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

int Server::cEnviarMensagemPrivada(const TCHAR *texto)
{
	return 1;
}

void Server::cEnviarMensagemPublica(const TCHAR *texto)
{
	// ToDo: 2 * TAMTEXTO ?!
	TCHAR msgWithUser[TAMTEXTO];

	for (unsigned int i=0; i <= (unsigned int)_tcslen(texto)+1; i++) // Mais o terminador,  && i < TAMTEXTO
	{
		msgWithUser[i] = texto[i];
	}

	// ToDo: Está a colocar um espaço a mais?
	EnviarMensagemPública(msgWithUser);
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