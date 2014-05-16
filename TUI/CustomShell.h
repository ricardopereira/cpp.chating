#include <iostream>

#include "CoreShell.h"

#ifndef __CUSTOMSHELL_H__
#define __CUSTOMSHELL_H__

class CustomShell: public CoreShell
{
protected:
	virtual void showCommandNotFound(const string& c);
	virtual void showCommandSintaxeFailed(const CommandItem& item);
public:
	CustomShell(string name) : CoreShell(name) {};

	virtual int readCommand();
	virtual void showCommands();
};

#endif