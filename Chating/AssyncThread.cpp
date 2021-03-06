#include "AssyncThread.h"
#include "../ChatingDll/Dll.h"
#include "../Logic/ChatComunication.h"

AssyncThread::AssyncThread(sTchar_t username, Controller& controller, ChatBox& messageArea, ListBox& listUserOnline, HWND publicWindowHandle)
: controller(controller), messageArea(messageArea), listUserOnline(listUserOnline)
{
	this->ptrClasse = this;
	oTcharStream_t tempText;

	TCHAR ip[TAMIP];
	controller.loadConfig(ip);

	tempText << TEXT("\\\\") << ip << TEXT("\\pipe\\") << username << TEXT('\0');

	this->pipeName = tempText.str();

	this->publicWindowHandle = publicWindowHandle;
}

AssyncThread::~AssyncThread()
{

}
void AssyncThread::setPrivateMessageArea(ChatBox &privateMessageArea){
	this->privateMessageArea = &privateMessageArea;
}

void AssyncThread::setPrivateWindowHandle(HWND hWnd){
	this->privateWindowHandle = hWnd;
}

DWORD WINAPI AssyncThread::funcaoThread(){
	
	HANDLE hPipe = INVALID_HANDLE_VALUE;
	//pointer to server class handles, and other stuff
	MSG_T buffer[BUFFER_RECORDS];
	DWORD bytesRead;
	bool online = true;

	if (!WaitNamedPipe(this->pipeName.c_str(), NMPWAIT_WAIT_FOREVER)){
		MessageBox(0, TEXT("AssyncThread: Erro conex�o ao pipe"), TEXT("Erro"), MB_OK | MB_ICONERROR);
		ExitThread(1);
		return 1; //Verify this
	}

	//Abrir o pipe
	hPipe = CreateFile(
		this->pipeName.c_str(),
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (hPipe == INVALID_HANDLE_VALUE)
		MessageBox(0, TEXT("AssyncThread: Pipe inv�lido"), TEXT("Erro"), MB_OK | MB_ICONERROR);

	while (online)
	{
		// Espera que o pipe fique dispon�vel do lado do Servidor

		int success = ReadFile(
			hPipe,
			buffer,
			sizeof(MSG_T)*BUFFER_RECORDS,
			&bytesRead,
			NULL);

		if (!success)
			return -1;
			
		switch (buffer[0].messageType){
		case LIST_ALL_USERS:
			ChatUser* user;
			for (DWORD i = 0; i < buffer[0].nMessages; i++){
				user = this->controller.addUtilizador(buffer[i].utilizador);
				if (_tcscmp(buffer[i].mensagem.texto,_T("1")) == 0)
					user->setAdmin();
			}
			break;
		case LIST_USERS_ONLINE:
			this->controller.clearUtilizadoresOnline();
			for (DWORD i = 0; i < buffer[0].nMessages; i++){
				this->controller.addUtilizadorOnline(buffer[i].utilizador);
			}
			break;
		case PRIVATE_MESSAGE:
			for (DWORD i = 0; i < buffer[0].nMessages; i++){
				this->privateMessageArea->addMessage(buffer[i].utilizador, buffer[i].mensagem);
			}
			break;
		case PUBLIC_MESSAGE:
			for (DWORD i = 0; i < buffer[0].nMessages; i++){
				this->messageArea.addMessage(buffer[i].utilizador, buffer[i].mensagem);
			}
			break;
		case USER_ONLINE:
			this->controller.addUtilizadorOnline(buffer[0].utilizador);
			forceRefresh();
			break;
		case USER_OFFLINE:
			this->controller.removeUtilizadorOnline(buffer[0].utilizador);
			forceRefresh();
			break;
		case DISCONNECT:
			online = false;
			this->controller.shutdown();
			break;
		case CLOSE_CHAT:
			SendMessage(this->privateWindowHandle, WM_USER, 0, 0);
			break;
		case _LANCARCHAT:
			this->controller.SetPrivatePartner(buffer[0].utilizador);
			SendMessage(this->publicWindowHandle, (WM_USER+1), 0,0);
			break;
		case _CANCELAR_CONVERSA:
			SendMessage(this->privateWindowHandle, WM_USER+2, 0, 0);
		}
	}

	return 1;
}

void AssyncThread::forceRefresh()
{
	listUserOnline.clear();
	for (int i = 0; i < this->controller.getTotalUtilizadoresOnline(); i++)
		listUserOnline.addString(this->controller.getUtilizadorOnline(i)->getUsername().c_str());
}