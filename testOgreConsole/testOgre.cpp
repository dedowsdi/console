#include <iostream>
#include "testOgreArgHandler.h"
#include "testOgreConsole.h"
#include "testOgreSI.h"
#include "../testConsole/testConsoleUI.h"
//#include "testOgreParticleSystemSI.h"
#include "../common/BaseManager.h"

using namespace pac;

int main(int argc, char **argv) {
  // init ogre
  BaseManager app;
  app.setup();
  // app.go();

  // init console
  ImplConsoleUI ui;
  ui.initParams();
  OgreConsole oc(&ui, app.getSceneMgr());
  oc.init();

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
