#include <iostream>

// Text User Interface
#include "CustomShell.h"

/* tcout for streaming wide/nonwide char */
#if defined(UNICODE) || defined(_UNICODE)
#define tcout std::wcout
#else
#define tcout std::cout
#endif

#ifndef __SHELL_H__
#define __SHELL_H__

class Shell: public CustomShell
{
protected:
	virtual void createCommands(vector<CommandItem>& list);
public:
	Shell() : CustomShell("Server") {};
};

#endif