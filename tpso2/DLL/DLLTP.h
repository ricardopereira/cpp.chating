#include <windows.h>
#include <tchar.h>
#define TAMLOGIN 15
#define TAMPASS 15
#define TAMTEXTO 256
#define NUMMSGSPUBLICAS 100
#define NUMMSGSPRIVADAS 100
#define NUMUTILIZADORES 100

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DLL_IMP_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DLL_IMP_API functions as being imported from a DLL, whereas this DLL sees symbols

#include <windows.h>

//Definir uma constante para facilitar a leitura do prot�tipo da fun��o
//Este .h deve ser inclu�do no projecto que o vai usar (modo impl�cito)

#define DllExport extern "C"

//Esta macro � definida pelo sistema caso estejamos na DLL (<DLL_IMP>_EXPORTS definida)
//ou na app (<DLL_IMP>_EXPORTS n�o definida) onde DLL_IMP � o nome deste projecto
#ifdef DLLTP_EXPORTS
#define DLL_IMP_API DllExport __declspec(dllexport)
#else
#define DLL_IMP_API __declspec(dllimport)
#endif

extern "C"{

	typedef struct{
		TCHAR login[TAMLOGIN], password[TAMPASS];
		int tipo; //1-utilizador 2-admin
		int estado; //0-livre 1-ocupado numa conversa privada
	}UTILIZADOR;

	typedef struct{
		int ano, mes, dia, hora, minuto, segundo;
	}DATA;

	typedef struct{
		TCHAR texto[TAMTEXTO];
		DATA instante;
	}MENSAGEM;


	typedef struct{
		MENSAGEM publicas[NUMMSGSPUBLICAS];
		MENSAGEM privadas[NUMUTILIZADORES][NUMUTILIZADORES][NUMMSGSPRIVADAS];
	}CHATGLOBAL;



	typedef struct{
		MENSAGEM publicas[NUMMSGSPUBLICAS];
		MENSAGEM privadas[NUMUTILIZADORES][NUMMSGSPUBLICAS];
	}CHAT;



	DLL_IMP_API int Autenticar(TCHAR *login, TCHAR *pass);
	/* Valida o acesso a um dado login e password de um dado utilizador.
	Esta fun��o retorna a valida��o (sucesso ou insucesso) e ainda se o
	utilizador em causa � administrador.*/
	DLL_IMP_API int LerListaUtilizadores(UTILIZADOR *utilizadores);
	/* Recebe (por argumento) informa��o actualizada de forma aut�noma
	sobre os jogadores online.*/
	DLL_IMP_API int LerListaUtilizadoresRegistados(UTILIZADOR *utilizadores);
	/*Opcional, s� para saber que utilizadores existem e quais s�o os seus detalhes*/
	DLL_IMP_API int IniciarConversa(TCHAR *utilizador);
	/* Pede permiss�o para iniciar conversa privada com um dado
	utilizador. Caso este esteja livre � aceite, caso esteja j� em
	conversa privada com outro utilizador o pedido � recusado.*/
	DLL_IMP_API int DesligarConversa();
	/* Comunica ao servidor que deseja terminar a conversa que mant�m
	actualmente em privado com um outro utilizador. As novas mensagens
	enviadas em privado por este ou pelo outro ser�o rejeitadas. Esta
	fun��o devolve um c�digo de erro, caso este utilizador n�o mantinha
	qualquer conversa privada.*/
	DLL_IMP_API int EnviarMensagemPrivada(TCHAR *msg);
	/*Enviar a mensagem ao utilizador com quem mantemos a conversa
	privada. Caso o utilizador destinat�rio j� tenha desligado (ou este)
	esta conversa, a fun��o devolve um c�digo de erro.*/
	DLL_IMP_API void EnviarMensagemP�blica(TCHAR *msg);
	/*Enviar a mensagem a todos os utilizadores online.*/
	DLL_IMP_API CHAT LerInformacaoInicial();
	/* Recebe toda informa��o hist�rica necess�ria para apresenta��o nas
	janelas de conversa p�blica e privada.*/
	DLL_IMP_API MENSAGEM LerMensagensPublicas();
	/*Recebe informa��o autonomamente que � enviada pelo servidor e
	acrescenta no fundo da janela de conversa p�blica.*/
	DLL_IMP_API MENSAGEM LerMensagensPrivadas();
	/*Recebe informa��o autonomamente que � enviada pelo servidor e
	acrescenta no fundo da janela de conversa privada.*/
	DLL_IMP_API int Sair();
	/* Envia o pedido de sa�da (logout) do sistema ao servidor. */
	DLL_IMP_API int Desligar();
	/* Envia o pedido de encerramento do sistema ao servidor. */
}