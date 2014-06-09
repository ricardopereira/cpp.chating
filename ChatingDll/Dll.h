#include <windows.h>

#ifndef TPDLL_H
#define TPDLL_H

#define TAMLOGIN 15
#define TAMPASS 15
#define TAMTEXTO 100

#define NUMMSGSPUBLICAS 30
#define NUMMSGSPRIVADAS 30
#define NUMUTILIZADORES 30

#ifdef CHATINGDLL_EXPORTS
	#define DLL_IMP_API __declspec(dllexport)
#else
	#define DLL_IMP_API __declspec(dllimport)
#endif

extern "C"
{
	enum returnMessages {
		USER_NOT_REGISTERED,
		USER_NOT_FOUND,
		INCORRECT_PASSWORD,
		SUCCESS,
		SUCCESS_ADMIN,
		NO_PRIVILEDGES,
		USER_BUSY,
		PIPE_ERROR,
		REGEDIT_OK,
		REGEDIT_FIRST_TIME,
		REGEDIT_NOT_OK
	};

	enum typeMessages {
		PRIVATE_MESSAGE,
		PUBLIC_MESSAGE,
		LIST_USERS_ONLINE,
		LIST_ALL_USERS,
		USER_ONLINE,
		USER_OFFLINE,
		CLOSE_CHAT,
		_LANCARCHAT,
		_CANCELAR_CONVERSA
	};

	typedef struct {
		TCHAR login[TAMLOGIN], password[TAMPASS];
		short int tipo; //1-utilizador 2-admin
		short int estado; //0-livre 1-ocupado numa conversa privada
	} UTILIZADOR;

	typedef struct {
		short int ano, mes, dia, hora, minuto, segundo;
	} DATA;

	typedef struct {
		TCHAR texto[TAMTEXTO];
		DATA instante;
	} MENSAGEM;

	typedef struct msg_t {
		DWORD nMessages;
		DWORD messageType;
		DWORD userSender;
		DWORD userReceiver;
		MENSAGEM mensagem;
		TCHAR utilizador[TAMLOGIN];
	} MSG_T;

	typedef struct {
		MENSAGEM publicas[NUMMSGSPUBLICAS];
		MENSAGEM privadas[NUMUTILIZADORES][NUMMSGSPRIVADAS];
	} CHAT;

	//Funções a serem exportadas/importadas
	DLL_IMP_API int AbrirPipe();

	DLL_IMP_API int Autenticar(const TCHAR *login, const TCHAR *pass);
	DLL_IMP_API int Registar(const TCHAR *login, const TCHAR *pass);
	DLL_IMP_API int RemoverUtilizador(const TCHAR *login);
	DLL_IMP_API int LerListaUtilizadores();
	DLL_IMP_API int LerListaUtilizadoresRegistados();
	DLL_IMP_API int IniciarConversa(const TCHAR *utilizador, int flag);
	DLL_IMP_API int DesligarConversa();
	DLL_IMP_API int EnviarMensagemPrivada(const TCHAR *msg);
	DLL_IMP_API void EnviarMensagemPublica(const TCHAR *msg, const TCHAR *owner);
	DLL_IMP_API void LerInformacaoInicial();
	//DLL_IMP_API MENSAGEM LerMensagensPublicas();
	//DLL_IMP_API MENSAGEM LerMensagensPrivadas();
	DLL_IMP_API int Sair();
	DLL_IMP_API int Desligar();
	DLL_IMP_API void CancelarConversa();
}

#endif