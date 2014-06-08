#include "Controller.h"

int pipeAberto = 0;

Controller::Controller()
{
	reset();
}

Controller::~Controller()
{
	destroyUtilizadores();
}

void Controller::destroyUtilizadores()
{
	for (unsigned int i = 0; i < utilizadores.size(); i++)
	{
		delete utilizadores.at(i);
	}
}

bool Controller::getIsAutenticado()
{
	// ToDo
	return this->autenticado;
}

bool Controller::getIsAdministrador()
{
	// ToDo
	return this->privilegiosAdmin;
}

const ChatUser& Controller::getLoginAutenticado()
{
	return *this->loginAutenticado;
}

void Controller::reset()
{
	this->autenticado = false;
	this->privilegiosAdmin = false;
	this->loginAutenticado = NULL;

	utilizadoresOnline.clear();
	utilizadores.clear();
	destroyUtilizadores();
}

int Controller::getTotalUtilizadores()
{
	return this->utilizadores.size();
}

int Controller::getTotalUtilizadoresOnline()
{
	return this->utilizadoresOnline.size();
}

ChatUser* Controller::getUtilizador(unsigned int index)
{
	if (index >= 0 && index < this->utilizadores.size())
		return utilizadores.at(index);
	else
		return NULL;
}

ChatUser* Controller::getUtilizador(const TCHAR *username)
{
	for (unsigned int i = 0; i < utilizadores.size(); i++)
	{
		if (_tcscmp(utilizadores.at(i)->getUsername().c_str(), username) == 0)
			return  utilizadores.at(i);
	}
	return NULL;
}

ChatUser* Controller::getUtilizadorOnline(unsigned int index)
{
	if (index >= 0 && index < this->utilizadoresOnline.size())
		return utilizadoresOnline.at(index);
	else
		return NULL;
}

void Controller::deleteUtilizador(const TCHAR *username)
{
	if (_tcscmp(username,TEXT("")) == 0)
		return;

	// ToDo: delete

	int res = RemoverUtilizador(username);
}

ChatUser* Controller::addUtilizador(const TCHAR *username)
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

ChatUser* Controller::addUtilizadorOnline(const TCHAR *username)
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

void Controller::removeUtilizadorOnline(const TCHAR *username)
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

void Controller::clearUtilizadoresOnline()
{
	utilizadoresOnline.clear();
}

int Controller::login(const TCHAR* login, const TCHAR *pass)
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

int Controller::signUp(const TCHAR* login, const TCHAR *pass)
{
	if (_tcscmp(login,TEXT("")) == 0)
		return 0;
	if (!pipeAberto){
		pipeAberto = AbrirPipe(); //Abre o pipe para a comunicacao
	}
	int res = Registar(login, pass);
	return res;
}

void Controller::loggedIn(const TCHAR* username, bool isAdmin)
{
	ChatUser* user = addUtilizadorOnline(username);
	user->setOnline();

	if (isAdmin)
		user->setAdmin();
	
	this->loginAutenticado = user;
}

void Controller::loadPublicInformation()
{
	LerListaUtilizadores();
	LerListaUtilizadoresRegistados();
	LerInformacaoInicial();
}

int Controller::cIniciarConversa(const TCHAR *utilizador)
{
	if (_tcscmp(utilizador,TEXT("")) == 0)
		return 0;
	return IniciarConversa(utilizador);
}

int Controller::cDesligarConversa()
{ 
	return DesligarConversa();
}

int Controller::cEnviarMensagemPrivada(const TCHAR *texto)
{
	return EnviarMensagemPrivada(texto);
}

void Controller::cEnviarMensagemPublica(const TCHAR *texto)
{
	EnviarMensagemPublica(texto, this->getLoginAutenticado().getUsername().c_str());
	return;
}

int Controller::logout()
{ 
	if (!this->getIsAutenticado())
		return 0;

	int res = Sair();
	
	//if (res) ?
	reset();
	return res;
}

int Controller::shutdown()
{ 
	return 1;
}