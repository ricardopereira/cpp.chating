#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <aclapi.h>
#include <strsafe.h>

#include "Shell.h"
#include "ChatComunication.h"
#include "ThreadCliente.h"
#include "ThreadSaveRegistry.h"
#include "Servidor.h"
#include "Security.h"

using namespace std;

void comandStart();

int main()
{
	Shell sh;

	//UNICODE: By default, windows console does not process wide characters. 
	//Probably the simplest way to enable that functionality is to call _setmode:
#ifdef UNICODE 
	//_setmode(_fileno(stdin), _O_WTEXT); 
	//_setmode(_fileno(stdout), _O_WTEXT); 
#endif

	// Iniciar a shell
	sh.open();
	do
	{
		// Ler comando
		if (sh.readCommand())
		{
			// Verificar comando
			if (sh.isCommand("start"))
			{
				comandStart();
			}
			else if (sh.isCommand("x"))
			{
				cout << "Good bye" << endl;
				break;
			}
		}
	} while (!sh.toExit());

	return 0;
}

void comandStart()
{
	// Um pipe
	HANDLE hPipe, lastPipe;
	BOOL connected = 1;
	BOOL success = 0;
	vector<ThreadCliente*> threads;
	Servidor server;
	SECURITY_ATTRIBUTES sa = security::Seguranca();

	// Registo
	server.LoadRegistry();

	ThreadSaveRegistry threadSaveRegistry(server);
	threadSaveRegistry.LancarThread();

	while (1) {
		// Criar instância
		hPipe = CreateNamedPipe(PIPE_SERVER,
			PIPE_ACCESS_DUPLEX, //OpenMode
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, //PipeMode
			PIPE_UNLIMITED_INSTANCES, //MaxInstances
			sizeof(chatbuffer_t), //Int
			sizeof(chatbuffer_t), //Out
			0, //Timeout
			&sa); //Security

		// Verificar se pipe foi instanciado
		if (hPipe == INVALID_HANDLE_VALUE)
		{
		    tcout << TEXT("CreateNamedPipe failed: ") << GetLastError() << endl;
			return;
		}

		lastPipe = hPipe;
		// Esperar pelo cliente
		connected = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
		// Verificar conectividade
		if (connected)
		{
			// Cliente conectado
			tcout << TEXT("New thread ") << threads.size() << TEXT(": client connected") << endl;
			threads.push_back(new ThreadCliente(hPipe, &server));
			threads.back()->LancarThread();
		}
		else{ //Não consegue conexão -> fechar handle.
			CloseHandle(hPipe);
		}
	}

	for (unsigned int i = 0; i < threads.size(); i++)
	{
		delete threads.at(i);
	}
}