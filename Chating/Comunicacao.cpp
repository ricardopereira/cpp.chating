#include "Comunicacao.h"

Comunicacao::Comunicacao()
{
	this->autenticado = false;
	this->privilegiosAdmin = false;
}

int Comunicacao::cAutenticar(TCHAR* login, TCHAR *pass)
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

int Comunicacao::cLerListaUtilizadores()
{
	this->totalUtilizadores = LerListaUtilizadores(this->utilizadoresOnline);
	int x = 0;
	return 1; 
}

int Comunicacao::cLerListaUtilizadoresRegistados()
{
	this->totalUtilizadoresOnline = LerListaUtilizadoresRegistados(this->utilizadores);
	int x = 0;
	return 1; 
}

int Comunicacao::cIniciarConversa(TCHAR *utilizador) { return 1; }

int Comunicacao::cDesligarConversa() { return 1; }

int Comunicacao::cEnviarMensagemPrivada(TCHAR *msg) { return 1; }

void Comunicacao::cEnviarMensagemPública(TCHAR *msg)
{
	return;
}

void Comunicacao::cLerInformacaoInicial()
{
	LerInformacaoInicial();
	return;
}

void Comunicacao::cLerMensagensPublicas()
{
	MENSAGEM temp = LerMensagensPublicas();
	int x = 0;
}

void Comunicacao::cLerMensagensPrivadas(){
	MENSAGEM temp = LerMensagensPrivadas();
	int x = 0;
}

int Comunicacao::cSair()
{ 
	return Sair();
}

int Comunicacao::cDesligar(){ return 1; }