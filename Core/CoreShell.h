#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

#ifndef __CORESHELL_H__
#define __CORESHELL_H__

using namespace std;

class CommandItem;

class CoreShell
{
	string name;
	vector<string> args;
	vector<CommandItem>* listCommands;
	static const string noCommand;
	static TCHAR* noArgument;
	int closing;

	void notFound(const string& c);
protected:
	const string& getName();
	vector<string>& getArgs();

	int isValid();
	string getArgsAsString(const CommandItem& item);
	// Lista de Comandos Válidos
	const vector<CommandItem>& getCommandsList();

	virtual void init();
	virtual void createCommands(vector<CommandItem>& list)=0;
	virtual void internalInterpret();
	virtual void showCommandNotFound(const string& c)=0;
	virtual void showCommandSintaxeFailed(const CommandItem& item)=0;
public:
	CoreShell();
	CoreShell(string name);
	virtual ~CoreShell();

	void open();
	void close();
	int toExit();
	virtual int readCommand()=0;
	virtual void showCommands()=0;

	// Comando recebido
	int isCommand(const string& c);
	const string& getCommand();
	TCHAR* getArgument(int index);
	int getArgumentAsInt(int index);

	string getAsString();
};

class CommandItem
{
	string name;
	vector<string> args;
public:
	CommandItem(string n, string argsInComma);
	const string& getName() const;
	const vector<string>& getArgs() const;
	string CommandItem::getAsString() const;
};

#endif