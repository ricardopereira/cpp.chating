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
	return this->userAutenticado != NULL;
}

bool Controller::getIsAdministrador()
{
	if (!getIsAutenticado())
		return false;
	return this->userAutenticado->getIsAdmin();
}

const ChatUser& Controller::getUserAutenticado()
{
	return *this->userAutenticado;
}

void Controller::reset()
{
	this->userAutenticado = NULL;

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

	ChatUser* user = getUtilizador(username);
	// Nao e permitido eliminar o administrador
	if (!user)
		return;
	if (user->getIsAdmin())
		return;

	if (!RemoverUtilizador(username))
		return;

	user->setOffline();
	// Remove da memória
	removeUtilizadorOnline(username);
	for (unsigned int i = 0; i < utilizadores.size(); i++)
	{
		if (utilizadores.at(i) == user) {
			delete user;
			utilizadores.erase(utilizadores.begin()+i);
			break;
		}
	}
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

sTchar_t Controller::GetPrivatePartner()const{
	return this->privatePartner;
}
void Controller::SetPrivatePartner(const sTchar_t privatePartner){
	this->privatePartner = privatePartner;
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
		TCHAR ip[TAMIP];
		loadConfig(ip);
		pipeAberto = AbrirPipe(ip); //Abre o pipe para a comunicacao
	}
	int res = Autenticar(login, pass);

	if (res == SUCCESS) {
		loggedIn(login);
		return 1;
	}
	else if (res == SUCCESS_ADMIN) {
		loggedIn(login,true);
		return 2;
	}
	else {
		this->userAutenticado = NULL;
	}
	return 0;
}

int Controller::signUp(const TCHAR* login, const TCHAR *pass)
{
	if (_tcscmp(login,TEXT("")) == 0)
		return 0;
	if (!pipeAberto) {
		TCHAR ip[TAMIP];
		loadConfig(ip);
		//Abre o pipe para a comunicacao
		pipeAberto = AbrirPipe(ip);
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
	
	this->userAutenticado = user;
}

void Controller::loadPublicInformation()
{
	LerListaUtilizadores();
	LerListaUtilizadoresRegistados();
	LerInformacaoInicial();
}

int Controller::cIniciarConversa(const TCHAR *utilizador, int flag)
{
	if (_tcscmp(utilizador,TEXT("")) == 0)
		return 0;
	return IniciarConversa(utilizador, flag);
}

int Controller::cDesligarConversa()
{ 
	return DesligarConversa();
}

int Controller::cEnviarMensagemPrivada(const TCHAR *texto)
{
	return EnviarMensagemPrivada(texto, this->getUserAutenticado().getUsername().c_str());
}

void Controller::cCancelarConversa()
{
	CancelarConversa();
}

void Controller::cEnviarMensagemPublica(const TCHAR *texto)
{
	EnviarMensagemPublica(texto, this->getUserAutenticado().getUsername().c_str());
}

int Controller::logout()
{ 
	if (!this->getIsAutenticado())
		return 0;

	int res = Sair(this->getUserAutenticado().getUsername().c_str());
	
	//if (res) ?
	reset();
	return res;
}

void Controller::shutdown()
{ 
	for (unsigned int i = 0; i < observers.size(); i++)
	{
		PostMessage(observers.at(0), WM_CLOSE, 0, 0);
	}
}

void Controller::shutdownServer()
{
	Desligar();
}

void Controller::addObserver(HWND hWnd)
{
	observers.push_back(hWnd);
}

void Controller::loadConfig(TCHAR* ipserver)
{
	HKEY key;
	DWORD res;
	DWORD size = TAMIP;

	// Default
	oTcharStream_t ip;
	ip << TEXT("127.0.0.1");

	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE,TEXT("Software\\Chating"),0, NULL, REG_OPTION_VOLATILE,
		KEY_ALL_ACCESS, NULL, &key, &res) != ERROR_SUCCESS)
	{
		_tcscpy_s(ipserver, _tcslen(ip.str().c_str())*sizeof(TCHAR), ip.str().c_str());
		return;
	}
	//Se a chave foi criada, inicializar os valores
	else if (res == REG_CREATED_NEW_KEY) {
		_tcscpy_s(ipserver, _tcslen(ip.str().c_str())*sizeof(TCHAR), ip.str().c_str());
	}
	//Se a chave foi aberta, ler os valores lá guardados
	else if (res == REG_OPENED_EXISTING_KEY) {
		RegQueryValueEx(key, TEXT("IPServer"), NULL, NULL, (LPBYTE)ipserver, &size);
		ipserver[size/sizeof(TCHAR)]='\0';
	}
	RegCloseKey(key);
}

bool Controller::saveConfig(TCHAR* ipserver)
{
	HKEY key;
	DWORD res;
	DWORD size = TAMIP;

	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE,TEXT("Software\\Chating"),0, NULL, REG_OPTION_VOLATILE,
		KEY_ALL_ACCESS, NULL, &key, &res) != ERROR_SUCCESS)
	{
		return false;
	}
	else if (res == REG_CREATED_NEW_KEY || res == REG_OPENED_EXISTING_KEY)
	{
		RegSetValueEx(key, TEXT("IPServer"), 0, REG_SZ, (LPBYTE)ipserver, _tcslen(ipserver)*sizeof(TCHAR));
	}
	RegCloseKey(key);
	return true;
}