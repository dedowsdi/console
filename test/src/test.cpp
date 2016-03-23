#include <iostream>
#include "pacConsole.h"

#include "testAbsDir.hpp"
#include "testArgHandler.hpp"
#include "testCommand.hpp"
#include "testConsole.hpp"
#include "testConsolePattern.hpp"
#include "testCmdHistory.hpp"
#include "testStdUtil.hpp"
#include "testStringUtil.hpp"
using namespace pac;

int main(int argc, char **argv) 
{
	//init console
	ImplConsoleUI ui;
	Console console(&ui);
	console.init();
	
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

