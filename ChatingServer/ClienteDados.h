#pragma once
#include "Common.h"
class ClienteDados
{
	sTchar_t username;
	sTchar_t password;
	int tipo;
	bool online;


public:
	ClienteDados(sTchar_t username, sTchar_t password, int tipo);
	~ClienteDados();
	void SetOnline();
	void SetOffline();
	sTchar_t GetUsername();
	sTchar_t GetPassword();


};

