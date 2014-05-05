#pragma once
#include <Windows.h>

class Thread
{
private:
	
protected:
	//typedef DWORD(Thread::*pfuncao)(LPVOID);
	Thread();
	HANDLE handleThread;
	Thread* ptrClasse;///pfuncao fThread;
	DWORD threadID;
	static DWORD WINAPI stub(LPVOID param);
public:

	~Thread();
	virtual DWORD WINAPI funcaoThread(LPVOID param)=0;
	virtual void LancarThread();
};

