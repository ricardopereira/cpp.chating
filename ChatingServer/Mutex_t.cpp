#include "Mutex_t.h"


Mutex_t::Mutex_t()
{
	this->handle_m = CreateMutex(NULL, FALSE, NULL);
}


Mutex_t::~Mutex_t()
{
	//eliminar mutex
}

void Mutex_t::Wait(){
	WaitForSingleObject(this->handle_m, INFINITE);
}

void Mutex_t::Release(){
	ReleaseMutex(this->handle_m);
}

