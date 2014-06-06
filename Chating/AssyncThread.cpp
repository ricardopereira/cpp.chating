#include "AssyncThread.h"
#include "../ChatingDll/Dll.h"

AssyncThread::AssyncThread(sTchar_t username, ChatBox& messageArea, ListBox& listUsers)
: messageArea(messageArea), listUsers(listUsers)
{
	this->ptrClasse = this;
	oTcharStream_t tempText;

	tempText << TEXT("\\\\.\\pipe\\") << username;

	this->pipeName = tempText.str();
}

AssyncThread::~AssyncThread()
{
}

DWORD WINAPI AssyncThread::funcaoThread(){
	
	
	
	HANDLE hPipe = INVALID_HANDLE_VALUE;
	//pointer to server class handles, and other stuff
	MSG_T buffer[50];
	DWORD bytesRead;

	// ToDo: Pipe com Read e Write?!

	hPipe = CreateFile( //A criação do pipe
		this->pipeName.c_str(),
		GENERIC_READ |
		GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (hPipe == INVALID_HANDLE_VALUE)
		MessageBox(0, TEXT("AssyncThread: Pipe inválido"), TEXT("Erro"), MB_OK | MB_ICONERROR);

	while (1)
	{
		// Espera que o pipe fique disponível do lado do Servidor
		if (!WaitNamedPipe(this->pipeName.c_str(), NMPWAIT_WAIT_FOREVER))
			MessageBox(0, TEXT("AssyncThread: Erro conexão ao pipe"), TEXT("Erro"), MB_OK | MB_ICONERROR);
		//			if(conectado){
		else {
			int success = ReadFile(
				hPipe,
				&buffer,
				sizeof(MSG_T)*50,
				&bytesRead,
				NULL);
			if (!success)
				return -1;
			
			switch (buffer[0].messageType){
			case LIST_ALL_USERS:
				break;
			case LIST_USERS_ONLINE:
				break;
			case PRIVATE_MESSAGE:
				break;
			case PUBLIC_MESSAGE:
				for (DWORD i = 0; i < buffer[0].nMessages; i++){
					this->messageArea.addMessage(buffer[i].utilizador, buffer[i].mensagem);
				}
				break;
			
			}
		}
	}
}
