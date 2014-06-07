#include "Server.h"

int pipeAberto = 0;

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

const ChatUser& Server::getLoginAutenticado()
{
	return *this->loginAutenticado;
}

void Server::reset()
{
	this->autenticado = false;
	this->privilegiosAdmin = false;
	this->loginAutenticado = NULL;
}

int Server::getTotalUtilizadores()
{
	return this->utilizadores.size();
}

int Server::getTotalUtilizadoresOnline()
{
	return this->utilizadoresOnline.size();
}

ChatUser* Server::getUtilizador(unsigned int index)
{
	if (index >= 0 && index < this->utilizadores.size())
		return utilizadores.at(index);
	else
		return NULL;
}

ChatUser* Server::getUtilizador(const TCHAR *username)
{
	for (unsigned int i = 0; i < utilizadores.size(); i++)
	{
		if (_tcscmp(utilizadores.at(i)->getUsername().c_str(), username) == 0)
			return  utilizadores.at(i);
	}
	return NULL;
}

ChatUser* Server::getUtilizadorOnline(unsigned int index)
{
	if (index >= 0 && index < this->utilizadoresOnline.size())
		return utilizadoresOnline.at(index);
	else
		return NULL;
}

void Server::deleteUtilizador(const TCHAR *username)
{
	int res = RemoverUtilizador(username);
}

ChatUser* Server::addUtilizador(const TCHAR *username)
{
	ChatUser* user = new ChatUser(username);
	utilizadores.push_back(user);
	return user;
}

ChatUser* Server::addUtilizadorOnline(const TCHAR *username)
{
	ChatUser* user = getUtilizador(username);
	if (!user)
	{
		user = addUtilizador(username);
	}
	utilizadoresOnline.push_back(user);
	user->setOnline();
	return user;
}

void Server::removeUtilizadorOnline(const TCHAR *username)
{
	ChatUser* user = getUtilizador(username);
	if (!user)
		return;

	user->setOffline();

	// ToDo: remover do vector
}

void Server::clearUtilizadoresOnline()
{
	utilizadoresOnline.clear();
}

int Server::cAutenticar(const TCHAR* login, const TCHAR *pass)
{
	if (!pipeAberto) {
		// ToDo: N�o seria melhor a DLL gerir o pipe?!
		//afinal, o recurso � da DLL, n�o da interface que apenas abusa da DLL

		pipeAberto = AbrirPipe(); //Abre o pipe para a comunicacao
	}
	int res = Autenticar(login, pass);

	if (res == SUCCESS) {
		loggedIn(login);
		// ToDo - Redundante!
		this->autenticado = true;
		this->privilegiosAdmin = false;
		return 1;
	}
	else if (res == SUCCESS_ADMIN) {
		loggedIn(login);
		// ToDo - Redundante!
		this->autenticado = true;
		this->privilegiosAdmin = true;
		return 2;
	}
	else {
		this->autenticado = false;
		this->privilegiosAdmin = false;
		this->loginAutenticado = NULL;
	}
	return 0;
}

int Server::cRegistar(const TCHAR* login, const TCHAR *pass)
{
	if (!pipeAberto){
		pipeAberto = AbrirPipe(); //Abre o pipe para a comunicacao
	}
	int res = Registar(login, pass);
	return res;
}

void Server::loggedIn(const TCHAR* username)
{
	ChatUser* user = addUtilizadorOnline(username);
	user->setOnline();

	//Administrador
	if (_tcscmp(TEXT("admin"), username) == 0)
		user->setAdmin();
	
	this->loginAutenticado = user;

	LerListaUtilizadoresRegistados();
	LerListaUtilizadores();
}

int Server::cIniciarConversa(const TCHAR *utilizador)
{ 
	return IniciarConversa(utilizador);
}

int Server::cDesligarConversa()
{ 
	return DesligarConversa();
}

int Server::cEnviarMensagemPrivada(const TCHAR *texto)
{
	return EnviarMensagemPrivada(texto);
}

void Server::cEnviarMensagemPublica(const TCHAR *texto)
{
	EnviarMensagemPublica(texto, this->getLoginAutenticado().getUsername().c_str());
	return;
}

int Server::cSair()
{ 
	int res = Sair();
	
	//if (res) ?
	reset();
	return res;
}

int Server::cDesligar()
{ 
	return 1;
}