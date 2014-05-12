#include "Thread.h"

Thread::Thread()
{
	this->threadID = 0;
}

Thread::~Thread()
{
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

