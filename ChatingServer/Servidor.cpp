#include "Servidor.h"


Servidor::Servidor()
{
}
///////////SEMMMMMMMMMMMMMMMMAAAAAAAAAAAAAAAAFFFFFFFFOOOORRROOOSSSS PREEEECISAMMMMMMM_SE

Servidor::~Servidor()
{
//apagar memoria alocada dinamicamente
}

void Servidor::NovaMensagem(DATA data, int user1, int user2, sTchar_t msg){
	this->msgs.push_back(new Mensagens(data, user1, user2, msg));
}

Servidor::rMsg Servidor::Login(sTchar_t username, sTchar_t password, ClienteDados* cliente){
	for (unsigned int i = 0; i < clientes.size(); i++){
		if (clientes.at(i)->GetUsername() == username){
			if (clientes.at(i)->GetPassword() == password){
				cliente = clientes.at(i);
				return Servidor::SUCCESS;
			}
			else{
				cliente = nullptr;
				return Servidor::INCORRECT_PASSWORD;
			}
		}
	}
	cliente = nullptr;
	return Servidor::USER_NOT_FOUND;
}

Servidor::rMsg Servidor::RegisterUser(sTchar_t username, sTchar_t password){
	clientes.push_back(new ClienteDados(username, password));

	//Limitar a adição de novos utilizadores?
	return Servidor::SUCCESS;
}

