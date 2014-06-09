#ifndef CHATCOMUNICATION_H
#define CHATCOMUNICATION_H

#define PIPENAME_SERVER _T("pipeserver") 
#define PIPE_SERVER _T("\\\\.\\pipe\\pipeserver") //PTCHAR LPTSTR PCTSTR

/*Comunicacao*/

#define BUFFER_RECORDS 50

enum commands_t {
	CRIAR_UTILIZADOR,
	ELIMINAR_UTILIZADOR, //Admin
	LOGIN,
	LOGOUT,
	LANCAR_CHAT,
	FECHAR_CHAT,
	LISTA_UTILIZADORES_TODOS,
	LISTA_UTILIZADORES_ONLINE,
	ENVIAR_MSG_PRIVADA,
	ENVIAR_MSG_PUBLICA,
	LER_INFO_INICIAL,
	DESLIGAR_SERVIDOR //Admin
};

// Dedicado aos comandos de comunicação com o servidor
typedef struct
{
	commands_t command;
	int arg_num;
	TCHAR args[4][150];
} chatbuffer_t;

#endif