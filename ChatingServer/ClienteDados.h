#pragma once

#include "Common.h"

class ClienteDados
{
	sTchar_t username;
	sTchar_t password;
	int tipo;
	bool online;
	HANDLE privatePipe;
	int id;
public:
	ClienteDados(const sTchar_t username, const sTchar_t password, int tipo, int id);
	~ClienteDados();
	void SetOnline();
	void SetOffline();
	sTchar_t GetUsername()const;
	sTchar_t GetPassword()const;
	int GetTipo()const;
	void CreatePrivatePipe();
	int GetId()const;
	HANDLE GetPipe()const;
	bool GetIsOnline()const;
};

