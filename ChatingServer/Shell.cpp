#include "Shell.h"

void Shell::createCommands(vector<CommandItem>& list)
{
	list.push_back(CommandItem("start","")); //Iniciar pipe
	list.push_back(CommandItem("exit",""));
}