#ifndef CHATCOMUNICATION_H
#define CHATCOMUNICAtion_H
#define pipeName _T("\\\\.\\pipe\\pipeserver") //PTCHAR LPTSTR PCTSTR

/*Comunicacao*/

typedef struct 
{
	int command;
	int arg_num;
	TCHAR args[4][150];
} chatbuffer_t;

#endif