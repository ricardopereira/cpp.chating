#pragma once
#include "../ChatingDll/Dll.h"
#include "Common.h"

class Mensagens
{
private:
	DATA data;
	sTchar_t msg;
	sTchar_t owner;
	int user1;
	int user2;
public:
	Mensagens(DATA data, int user1, int user2, sTchar_t msg);
	sTchar_t GetMensagem()const;
	const DATA& GetDataMensagem();
	int GetReceiver()const;
	int GetSender()const;
	~Mensagens();
};

