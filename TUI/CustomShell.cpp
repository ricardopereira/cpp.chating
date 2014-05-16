#include <sstream>
#include <algorithm>

#include "CustomShell.h"

#include "CoreShell.h"

int CustomShell::readCommand()
{
	string line;
	cout << getName() << ">";
	// Obtem a linha
	getline(cin,line);
	// Parsing cada elemento da linha
	istringstream parse(line);
	string data;
	// Vector para os argumentos
	getArgs().clear();
	while (parse >> data)
	{
		// Sempre em lowercase
		transform(data.begin(),data.end(),data.begin(),::tolower);
		// Adiciona ao vetor
		getArgs().push_back(data);
	}
	internalInterpret();
	return !toExit() && isValid();
}

void CustomShell::showCommands()
{
	int line = 1;
	// Listar os comandos
	for (vector<CommandItem>::const_iterator item = getCommandsList().cbegin(); item != getCommandsList().end(); ++item, line++)
		cout << item->getAsString() << endl;
}

void CustomShell::showCommandNotFound(const string& c)
{
	cout << "Command '" << c << "' not found" << endl;
}

void CustomShell::showCommandSintaxeFailed(const CommandItem& item)
{
	ostringstream out;
	out << "Sintaxe '";
	out << item.getName() << "': ";
	// Listar os argumentos de cada comando
	for (vector<string>::const_iterator arg = item.getArgs().cbegin(); arg != item.getArgs().end(); ++arg)			
		out << "<" << *arg << "> ";
	cout << out.str() << endl;
}