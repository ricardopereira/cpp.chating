#pragma once
#include "Common.h"
class ClienteDados
{
	sTchar_t username;
	sTchar_t password;
	bool online;


public:
	ClienteDados(sTchar_t username, sTchar_t password);
	~ClienteDados();
	void SetOnline();
	void SetOffline();
	sTchar_t GetUsername();
	sTchar_t GetPassword();


};

