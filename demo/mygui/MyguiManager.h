#ifndef MYGUIMANAGER_H
#define MYGUIMANAGER_H

#include "BaseManager.h"
#include "SdlToMygui.h"
#include "MyguiConsoleUI.h"
#include "OgreConsole.h"
#include <MyGUI.h>
#include <MyGUI_Ogre2Platform.h>

class MyguiManager : public BaseManager {
private:
  bool mExecuting;
  MyGUI::Gui* mGUI;
  MyGUI::Ogre2Platform* mPlatform;
  pac::MyguiConsoleUI* mConsoleUI;
  pac::OgreConsole* mConsole;
  input::SdlToMygui mSdlToMygui;

public:
  MyguiManager();
  ~MyguiManager();
  virtual void update();
  virtual void handleSdlEvent(const SDL_Event& evt);
  virtual void initGui();
  virtual void setupCompositor();
  virtual bool frameStarted(const Ogre::FrameEvent& evt);
};

#endif /* MYGUIMANAGER_H */
