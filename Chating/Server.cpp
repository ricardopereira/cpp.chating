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

const sTchar_t& Server::getLoginAutenticado()
{
	return this->loginAutenticado;
}

void Server::reset()
{
	this->autenticado = false;
	this->privilegiosAdmin = false;
	this->loginAutenticado = TEXT("");

	this->totalUtilizadoresOnline = 0;
	this->totalUtilizadores = 0;
}

int Server::getTotalUtilizadores()
{
	return this->totalUtilizadores;
}

int Server::getTotalUtilizadoresOnline()
{
	return this->totalUtilizadoresOnline;
}

UTILIZADOR Server::getUtilizador(int index)
{
	UTILIZADOR res;

	// Teste
	_stprintf_s(res.login, TAMLOGIN, TEXT(""));
	_stprintf_s(res.password, TAMPASS, TEXT(""));
	res.estado = 0;
	res.tipo = 0;

	if (index >= 0 && index < this->totalUtilizadores)
		return utilizadores[index];
	else
		return res;
}

UTILIZADOR Server::getUtilizadorOnline(int index)
{
	UTILIZADOR res;

	// Teste
	_stprintf_s(res.login, TAMLOGIN, TEXT(""));
	_stprintf_s(res.password, TAMPASS, TEXT(""));
	res.estado = 0;
	res.tipo = 0;

	if (index >= 0 && index < this->totalUtilizadoresOnline)
		return utilizadoresOnline[index];
	else
		return res;
}

int Server::cAutenticar(TCHAR* login, TCHAR *pass)
{
	if (!pipeAberto){
		pipeAberto = AbrirPipe(); //Abre o pipe para a comunicacao
	}
	int res = Autenticar(login, pass);

	if (res == SUCCESS) {
		this->autenticado = true;
		this->privilegiosAdmin = false;
		this->loginAutenticado = login;
		loggedIn();
		return 1;
	}
	else if (res == SUCCESS_ADMIN) {
		this->autenticado = true;
		this->privilegiosAdmin = true;
		this->loginAutenticado = login; //Administrador

		loggedIn();
		return 2;
	}
	else {
		this->autenticado = false;
		this->privilegiosAdmin = false;
		this->loginAutenticado = TEXT("");
	}
	return 0;
}

int Server::cRegistar(TCHAR* login, TCHAR *pass)
{
	if (!pipeAberto){
		pipeAberto = AbrirPipe(); //Abre o pipe para a comunicacao
	}
	int res = Registar(login, pass);
	return res;
}

void Server::loggedIn()
{
	this->totalUtilizadoresOnline = LerListaUtilizadores(this->utilizadoresOnline);
	//this->totalUtilizadores = LerListaUtilizadoresRegistados(this->utilizadores);
}

int Server::cIniciarConversa(const TCHAR *utilizador)
{ 
	// ToDo: 2 * TAMLOGIN ?!
	TCHAR userAux[TAMLOGIN];

	for (unsigned int i=0; i <= (unsigned int)_tcslen(utilizador)+1; i++) // Mais o terminador,  && i < TAMLOGIN
	{
		userAux[i] = utilizador[i];
	}

	return IniciarConversa(userAux);
}

int Server::cDesligarConversa()
{ 
	return DesligarConversa();
}

int Server::cEnviarMensagemPrivada(const TCHAR *texto)
{
	// ToDo: 2 * TAMTEXTO ?!
	TCHAR msgWithUser[TAMTEXTO];

	for (unsigned int i=0; i <= (unsigned int)_tcslen(texto)+1; i++) // Mais o terminador,  && i < TAMTEXTO
	{
		msgWithUser[i] = texto[i];
	}

	return EnviarMensagemPrivada(msgWithUser);
}

void Server::cEnviarMensagemPublica(const TCHAR *texto)
{
	// ToDo: colcoar código duplicado num método - cEnviarMensagemPrivada
	TCHAR msgWithUser[TAMTEXTO];

	for (unsigned int i=0; i <= (unsigned int)_tcslen(texto)+1; i++) // Mais o terminador,  && i < TAMTEXTO
	{
		msgWithUser[i] = texto[i];
	}

	EnviarMensagemPublica(msgWithUser);
	return;
}

void Server::cLerInformacaoInicial()
{
	CHAT info = LerInformacaoInicial();
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