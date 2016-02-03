#ifndef TESTPACUICONSOLE_H
#define TESTPACUICONSOLE_H 
#include "pacUiConsole.h"

using namespace pac;

class ImplUiConsole : public UiConsole
{
public:

	virtual UiConsole& endl()
	{
		std::cout << std::endl;
		return *this;
	}

	virtual UiConsole& outputNoAutoWrap(const std::string& output, int type = 1)
	{
		std::cout << output; 
		return * this;
	}

	virtual UiConsole& complete(const std::string& s)
	{
		std::cout << "complete" << s;
		return *this;
	}

	virtual void updateCommandLine(const std::string& cmdLine = "")
	{
		std::cout << "updateCmdLine:" << cmdLine << std::endl;
	}

	virtual void updateCwd(const std::string& cwd)
	{
		std::cout << "updateCwd:" << cwd << std::endl;
	}

};

#endif //TESTPACUICONSOLE_H
