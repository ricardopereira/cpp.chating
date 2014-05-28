#include "Semaforo.h"


Semaforo::Semaforo()
{
	this->handle_s = CreateSemaphore(NULL, 2, 2, NULL);
}


Semaforo::~Semaforo()
{
	//eliminar semaforo
}

void Semaforo::Wait(){
	WaitForSingleObject(this->handle_s, INFINITE);
}
void Semaforo::Release(){
	ReleaseSemaphore(this->handle_s, 1, NULL);
}
