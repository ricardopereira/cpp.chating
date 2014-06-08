#include "Server.h"

int pipeAberto = 0;

Server::Server()
{
	reset();
}

Server::~Server()
{
	for (unsigned int i = 0; i < utilizadores.size(); i++)
	{
		delete utilizadores.at(i);
	}
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
	if (_tcscmp(username,TEXT("")) == 0)
		return;

	// ToDo: delete

	int res = RemoverUtilizador(username);
}

ChatUser* Server::addUtilizador(const TCHAR *username)
{
	if (_tcscmp(username,TEXT("")) == 0)
		return NULL;
	ChatUser* user = getUtilizador(username);
	if (!user)
	{
		user = new ChatUser(username);
		utilizadores.push_back(user);
	}
	return user;
}

ChatUser* Server::addUtilizadorOnline(const TCHAR *username)
{
	if (_tcscmp(username,TEXT("")) == 0)
		return NULL;

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

	for (unsigned int i = 0; i < utilizadoresOnline.size(); i++)
	{
		if (utilizadoresOnline.at(i) == user) {
			utilizadoresOnline.erase(utilizadoresOnline.begin()+i);
			user->setOffline();
			return;
		}
	}
}

void Server::clearUtilizadoresOnline()
{
	utilizadoresOnline.clear();
}

int Server::login(const TCHAR* login, const TCHAR *pass)
{
	if (_tcscmp(login,TEXT("")) == 0 || _tcscmp(pass,TEXT("")) == 0)
		return 0;

	if (!pipeAberto) {
		// ToDo: Não seria melhor a DLL gerir o pipe?!
		//afinal, o recurso é da DLL, não da interface que apenas abusa da DLL

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
		loggedIn(login,true);
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

int Server::signUp(const TCHAR* login, const TCHAR *pass)
{
	if (_tcscmp(login,TEXT("")) == 0)
		return 0;
	if (!pipeAberto){
		pipeAberto = AbrirPipe(); //Abre o pipe para a comunicacao
	}
	int res = Registar(login, pass);
	return res;
}

void Server::loggedIn(const TCHAR* username, bool isAdmin)
{
	ChatUser* user = addUtilizadorOnline(username);
	user->setOnline();

	if (isAdmin)
		user->setAdmin();
	
	this->loginAutenticado = user;
}

void Server::loggedOut(const TCHAR* username)
{
	// ToDo
	this->loginAutenticado = NULL;
}

void Server::loadPublicInformation()
{
	LerListaUtilizadores();
	LerListaUtilizadoresRegistados();
	LerInformacaoInicial();
}

int Server::cIniciarConversa(const TCHAR *utilizador)
{
	if (_tcscmp(utilizador,TEXT("")) == 0)
		return 0;
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

int Server::logout()
{ 
	if (!this->getIsAutenticado())
		return 0;

	int res = Sair();
	
	//if (res) ?
	reset();
	return res;
}

int Server::shutdown()
{ 
	return 1;
}