#include "Thread.h"

Thread::Thread()
{
	this->threadID = 0;
	this->handleThread = NULL;
}

Thread::~Thread()
{
	//Esperar threads terminarem
	//if (this->handleThread)
	//	WaitForSingleObject(this->handleThread,INFINITE);
}

DWORD WINAPI Thread::stub(LPVOID param)
{
	Thread *pClasse = static_cast<Thread*>(param);
	if (!pClasse){
		return -1;
	}
	return pClasse->funcaoThread();
}

void Thread::LancarThread()
{
	this->handleThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) Thread::stub, (LPVOID)this->ptrClasse, 0, &this->threadID);
}

