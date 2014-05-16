#include <iostream>
#include <string>
#include <sstream>

#include "CoreShell.h"

const string CoreShell::noCommand = "No command";
TCHAR* CoreShell::noArgument = TEXT("No argument");

CoreShell::CoreShell()
{
	this->name = "Shell"; //Por defeito
	init();
}

CoreShell::CoreShell(string name)
{
	this->name = name;
	init();
}

CoreShell::~CoreShell()
{
	if (listCommands)
		delete listCommands;
}

void CoreShell::init()
{
	listCommands = NULL;
}

void CoreShell::open()
{
	closing = 0;
}

void CoreShell::close()
{
	closing = 1;
}

int CoreShell::toExit()
{
	// Se for igual ao comando j
	if (!closing)
		closing = args.size() > 0 && getCommand().compare("j") == 0;
	return closing;
}

const string& CoreShell::getCommand()
{
	if (args.size() > 0)
		return args[0];
	else
		return noCommand;
}

int CoreShell::isCommand(const string& c)
{
	return getCommand().compare(c) == 0;
}

TCHAR* CoreShell::getArgument(int index)
{
	// Sem contar com o nome do comando
	int realIdx = index+1;
	// Obter argumento
	if (realIdx > (int)args.size()-1) return noArgument;
	return TEXT("Ricardo"); //args[realIdx];
}

int CoreShell::getArgumentAsInt(int index)
{
	//return atoi(getArgument(index).c_str());
	return 0;
}

const string& CoreShell::getName()
{
	return name;
}

vector<string>& CoreShell::getArgs()
{
	return args;
}

void CoreShell::internalInterpret()
{
	if (isCommand("h") || isCommand("help"))
	{
		showCommands();
	}
}

void CoreShell::notFound(const string& c)
{
	// De sistema
	if (c.compare("j") == 0 || c.compare("h") == 0 || c.compare("help") == 0) return;
	// Comando nao existe
	showCommandNotFound(c);
}

int CoreShell::isValid()
{
	vector<CommandItem>::const_iterator item;
	//Validar comando
	for (item = getCommandsList().cbegin(); item != getCommandsList().end(); ++item)
		if (getCommand().compare(item->getName()) == 0)
			break;
	// Comando nao existe
	if (item == getCommandsList().end())
	{
		notFound(getCommand());
		return 0;
	}

	// Validar nr de argumentos
	if (item->getArgs().size() != (int)args.size()-1)
	{
		showCommandSintaxeFailed(*item);
		return 0;
	}
	else
	{
		//?
		return 1;
	}
}

string CoreShell::getAsString()
{
	ostringstream out;
	// Listar os comandos
	for (vector<CommandItem>::const_iterator item = getCommandsList().cbegin(); item != getCommandsList().end(); ++item)
		out << item->getAsString() << endl;
	// Retornar listagem de comandos
	return out.str();
}

const vector<CommandItem>& CoreShell::getCommandsList()
{
	// Apenas cria se for necessario
	if (!listCommands)
	{
		listCommands = new vector<CommandItem>;
		// Comandos possiveis
		createCommands(*listCommands);
	}
	return *listCommands;
}

// Command Item

CommandItem::CommandItem(string n, string argsInComma)
{
	name = n;
	// Parsing cada elemento da linha
	istringstream parse(argsInComma);
	string data;
	// Vector para os argumentos
	while (parse >> data)
	{
		// Adiciona ao vetor
		args.push_back(data);
	}
}

const string& CommandItem::getName() const
{ 
	return name;
};

const vector<string>& CommandItem::getArgs() const
{ 
	return args; 
};

string CommandItem::getAsString() const
{
	ostringstream out;
	if (getArgs().size() == 0)
	{
		// Mostra o nome do comando
		out << getName();
	}
	else
	{
		// Listar comando com argumentos
		out << getName() << ": ";
		int idx = 0;
		for (vector<string>::const_iterator arg = getArgs().cbegin(); arg != getArgs().end(); ++arg, idx++)
		{
			out << "<" << *arg << ">";
			if (idx != (int)getArgs().size()-1)
				out << " ";
		}
	}
	return out.str();
}