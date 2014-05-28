#pragma once
#include <Windows.h>
class Semaforo
{
private:
	HANDLE handle_s;
public:
	Semaforo();
	~Semaforo();
	void Wait();
	void Release();
};

