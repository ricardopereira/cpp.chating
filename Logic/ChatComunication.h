#ifndef CHATCOMUNICATION_H
#define CHATCOMUNICAtion_H
#define pipeName _T("\\\\.\\pipe\\pipeserver") //PTCHAR LPTSTR PCTSTR

/*Valores maximos*/
#define MAXCOMMAND 50
#define MAXCOMMANDARGS 5

/*Comunicacao*/
typedef struct recRequest Request;
typedef struct 
{
	int command;
	int arg_num;
	TCHAR args[4][150];

} chatbuffer_t;
struct recRequest {
    int pid;
    char username[15];
    char password[15];
    char command[MAXCOMMAND];
};

#endif