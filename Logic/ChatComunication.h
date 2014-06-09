#ifndef CHATCOMUNICATION_H
#define CHATCOMUNICATION_H
#define PIPENAME_SERVER _T("\\\\.\\pipe\\pipeserver") //PTCHAR LPTSTR PCTSTR

/*Comunicacao*/

#define BUFFER_RECORDS 50

enum commands_t {
	REGISTER_NEW_USER,
	LOGIN,
	LANCAR_CHAT,
	LISTA_UTILIZADORES_TODOS,
	LISTA_UTILIZADORES_ONLINE,
	ENVIAR_MSG_PRIVADA,
	ENVIAR_MSG_PUBLICA,
	FECHAR_CHAT,
	LER_INFO_INICIAL,
	CRIAR_USER,
	DESLIGAR,
	//LER_MENSAGEM_PRIVADA,
	//LER_MENSAGEM_PUBLICA,
	ELIMINAR_UTILIZADOR,
	LOGOUT,
	CANCELAR_CONVERSA
};

// Dedicado aos comandos de comunicação com o servidor
typedef struct
{
	commands_t command;
	int arg_num;
	TCHAR args[4][150];
} chatbuffer_t;

#endif