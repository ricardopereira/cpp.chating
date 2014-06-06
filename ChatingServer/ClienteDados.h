#pragma once

#include "Common.h"

class ClienteDados
{
	sTchar_t username;
	sTchar_t password;
	int tipo;
	bool online;
	bool isAdmin;
	bool hasPrivateChat;
	HANDLE privatePipe;
public:
	ClienteDados(const sTchar_t username, const sTchar_t password, int tipo);
	~ClienteDados();
	void SetOnline();
	void SetOffline();
	sTchar_t GetUsername() const;
	sTchar_t GetPassword() const;
	int GetTipo() const;
	bool getIsOnline() const;
	bool getIsAdmin() const;
	void CreatePrivatePipe();
};