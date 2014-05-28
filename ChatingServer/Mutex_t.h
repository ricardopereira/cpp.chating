#pragma once
#include <Windows.h>

class Mutex_t
{
private:
	HANDLE handle_m;
public:
	Mutex_t();
	~Mutex_t();
	void Wait();
};

