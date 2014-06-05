#include "AssyncThread.h"
#include "../ChatingDll/Dll.h"

AssyncThread::AssyncThread(sTchar_t username)
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
		MessageBox(0, TEXT("ERRO: pipe"), TEXT("Erro"), MB_OK | MB_ICONERROR);///////

	while (1)
	{
		if (!WaitNamedPipe(this->pipeName.c_str(), NMPWAIT_WAIT_FOREVER))
			MessageBox(0, TEXT("ERRO: Erro conexão"), TEXT("Erro"), MB_OK | MB_ICONERROR);
		//			if(conectado){
		else{
			int success = ReadFile(
				hPipe,
				&buffer,
				sizeof(MSG_T)*50,
				&bytesRead,
				NULL);
			if (!success)
				return -1;
		
		}
	}


}

