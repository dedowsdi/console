#ifndef BASEMYGUIAPP_H
#define BASEMYGUIAPP_H

#include "GraphicsSystem.h"
#include "GameState.h"
#include "CameraController.h"
#include "OgreConfigFile.h"
#include "OgreLogManager.h"
#include "OgreRenderWindow.h"
#include "OgreRoot.h"
#include "OgreTimer.h"
#include "SdlInputHandler.h"
#include "SDL_InputManager.h"
#include "SDL_PointerManager.h"
#include <Compositor/OgreCompositorManager2.h>

using namespace Ogre;

namespace Demo {

class MyguiConsoleGameState : public GameState, public ::input::InputManager {
protected:
  GraphicsSystem* mGraphicsSystem;
  CameraController* mCameraController;

public:
  MyguiConsoleGameState();
  virtual void keyPressed(const SDL_KeyboardEvent& arg);
  //virtual void textInput(const SDL_TextInputEvent& arg);
  void _notifyGraphicsSystem(GraphicsSystem* graphicsSystem);
};

/*
 *for simplicity, demoapp will derive from both tutorial game state and graphics
 *system
 */
class MyguiConsole : public GraphicsSystem {
public:
  MyguiConsole(GameState* gs);

  virtual void initGui();
  void go();
};
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define DEFINE_APP(App)                                                      \
  using namespace Demo;                                                      \
  INT WINAPI WinMainApp(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT) { \
    App##GameState state;                                                    \
    App app(&state);                                                         \
    state._notifyGraphicsSystem(&app);                                       \
    app.go();                                                                \
  }
#else
#define DEFINE_APP(App)                \
  using namespace Demo;                \
  int main(int argc, char* argv[]) {   \
    App##GameState state;              \
    App app(&state);                   \
    state._notifyGraphicsSystem(&app); \
    app.go();                          \
  }
#endif
#endif /* BASEMYGUIAPP_H */
