#include <iostream>
#include "pacConsole.h"

#include "testAbsDir.h"
#include "testArgHandler.h"
#include "testCommand.h"
#include "testConsole.h"
#include "testConsolePattern.h"
#include "testCmdHistory.h"
#include "testStdUtil.h"
#include "testStringUtil.h"
using namespace pac;

int main(int argc, char **argv) {
  // init console
  ImplConsoleUI ui;
  ui.initParams();
  Console console(&ui);
  console.init();

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
