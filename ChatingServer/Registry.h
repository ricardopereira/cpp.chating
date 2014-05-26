#pragma once
#include "Servidor.h"

namespace Registry
{
	Servidor::rMsg LoadData(
		std::vector<ClienteDados*>& clientdata,
		std::vector<Mensagens*>& msgdata
		);
	Servidor::rMsg SaveData(
		std::vector<ClienteDados*>& clientdata,
		std::vector<Mensagens*>& msgdata
		);
	
	
};

