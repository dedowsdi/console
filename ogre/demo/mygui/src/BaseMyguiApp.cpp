#include "BaseMyguiApp.h"

namespace Demo {

//------------------------------------------------------------------------------
MyguiConsoleGameState::MyguiConsoleGameState() {}

//------------------------------------------------------------------------------
void MyguiConsoleGameState::keyPressed(const SDL_KeyboardEvent& arg) {
    ::input::InputManager::keyPressed(arg.keysym.sym, nullptr);
}

//------------------------------------------------------------------------------
//void MyguiConsoleGameState::textInput( const SDL_TextInputEvent& arg )
//{
    //::input::InputManager::keyPressed(arg.keysym.sym, arg.text );
//}

//------------------------------------------------------------------------------
void MyguiConsoleGameState::_notifyGraphicsSystem(GraphicsSystem* graphicsSystem) {
  mGraphicsSystem = graphicsSystem;
}

//------------------------------------------------------------------------------
MyguiConsole::MyguiConsole(GameState* gs) : GraphicsSystem(gs) {}

//------------------------------------------------------------------------------
void MyguiConsole::initGui() {
  //@TODO implement
  throw new std::runtime_error("unimplemented function called");
  
}

//------------------------------------------------------------------------------
void MyguiConsole::go() {
  this->initialize("a single point");
  this->initGui();

  if (this->getQuit()) {
    this->deinitialize();
    return;  // User cancelled config
  }

  Ogre::RenderWindow* renderWindow = this->getRenderWindow();

  this->createScene01();
  this->createScene02();

  // Do this after creating the scene for easier the debugging (the mouse
  // doesn't hide itself)
  SdlInputHandler* inputHandler = this->getInputHandler();
  inputHandler->setGrabMousePointer(true);
  inputHandler->setMouseVisible(false);

  Ogre::Timer timer;
  unsigned long startTime = timer.getMicroseconds();

  double timeSinceLast = 1.0 / 60.0;

  while (!this->getQuit()) {
    this->beginFrameParallel();
    this->update(static_cast<float>(timeSinceLast));
    this->finishFrameParallel();
    this->finishFrame();

    if (!renderWindow->isVisible()) {
      // Don't burn CPU cycles unnecessary when we're minimized.
      Ogre::Threads::Sleep(500);
    }

    unsigned long endTime = timer.getMicroseconds();
    timeSinceLast = (endTime - startTime) / 1000000.0;
    timeSinceLast = std::min(1.0, timeSinceLast);  // Prevent from going
                                                   // haywire.
    startTime = endTime;
  }

  this->destroyScene();
  this->deinitialize();

  return;
}
}

DEFINE_APP(MyguiConsole)
