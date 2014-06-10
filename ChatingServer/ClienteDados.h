#pragma once

#include "Common.h"

class ClienteDados
{
	sTchar_t username;
	sTchar_t password;
	int tipo;
	bool online;
	bool isAdmin;
	bool isBusy;
	HANDLE privatePipe;
	int id;
	SECURITY_ATTRIBUTES sa;
public:
	ClienteDados(const sTchar_t username, const sTchar_t password, int tipo, int id);
	~ClienteDados();
	void SetOnline();
	void SetOffline();
	void SetIsBusy(const bool isbusy);
	sTchar_t GetUsername() const;
	sTchar_t GetPassword() const;
	int GetTipo() const;
	bool GetIsOnline() const;
	bool getIsAdmin() const;
	bool GetIsBusy() const;
	SECURITY_ATTRIBUTES* getSecurity();
	void CreatePrivatePipe();
	int GetId() const;
	HANDLE GetPipe() const;
};