#include "MyguiManager.h"
#include "SdlToMygui.h"
#include <SDL.h>

#include <Compositor/OgreCompositorManager2.h>
#include <Compositor/OgreCompositorNodeDef.h>
#include <Compositor/Pass/OgreCompositorPass.h>
#include <Compositor/Pass/OgreCompositorPassDef.h>
#include <Compositor/Pass/PassScene/OgreCompositorPassSceneDef.h>
#include <Compositor/Pass/PassClear/OgreCompositorPassClearDef.h>
#include <Compositor/OgreCompositorWorkspaceDef.h>
#include <Hlms/Unlit/OgreHlmsUnlit.h>
#include <Hlms/Pbs/OgreHlmsPbs.h>
#include <OgreHlmsManager.h>

//------------------------------------------------------------------------------
MyguiManager::MyguiManager()
    : mExecuting(false), mGUI(0), mPlatform(0), mConsoleUI(0), mConsole(0) {}

//------------------------------------------------------------------------------
MyguiManager::~MyguiManager() {
  SDL_StopTextInput();
  if (mGUI) {
    mGUI->shutdown();
    delete mGUI;
    mGUI = nullptr;
  }

  if (mPlatform) {
    mPlatform->shutdown();
    delete mPlatform;
    mPlatform = nullptr;
  }
}
//------------------------------------------------------------------------------
void MyguiManager::update() { mSdlToMygui.update(); }
//------------------------------------------------------------------------------
void MyguiManager::handleSdlEvent(const SDL_Event &evt) {
  if (evt.type == SDL_KEYDOWN) {
    if (mExecuting) return;
    if (evt.key.keysym.sym == SDLK_BACKQUOTE) {
      mConsole->toggleActive();
      return;
    }
    if (mConsole->isActive()) {
      switch (evt.key.keysym.sym) {
        case SDLK_RETURN:
          mExecuting = true;
          return;
        case SDLK_UP:
          mConsole->rollCommand(true);
          return;
        case SDLK_DOWN:
          mConsole->rollCommand(false);
          return;
        case SDLK_TAB:
          mConsole->prompt();
          return;
        default:
          break;
      }
    }
  }

  // igonre ` in textinput`
  if (evt.type == SDL_TEXTINPUT && *evt.text.text == '`') return;

  mSdlToMygui.handleSdlEvent(evt);
  BaseManager::handleSdlEvent(evt);
}

//------------------------------------------------------------------------------
void MyguiManager::initGui() {
  mPlatform = new MyGUI::Ogre2Platform();
  mPlatform->initialise(mRenderWindow, mSceneManager, "Mygui");

  mGUI = new MyGUI::Gui();
  mGUI->initialise("MyGUI_Core.xml");

  mConsoleUI = new pac::MyguiConsoleUI();
  mConsoleUI->initParams();
  mConsole = new pac::OgreConsole(mConsoleUI, mSceneManager);
  mConsole->init();

  SDL_StartTextInput();
}

//------------------------------------------------------------------------------
void MyguiManager::setupCompositor() {
  // setup workspace
  // NB this can also been done via a script using the following syntax
  /*
  pass custom MYGUI
  {
  }
  */
  const Ogre::String workspaceName = "scene workspace";
  const Ogre::IdString workspaceNameHash = workspaceName;

  Ogre::CompositorManager2 *pCompositorManager =
      Ogre::Root::getSingleton().getCompositorManager2();
  Ogre::CompositorNodeDef *nodeDef =
      pCompositorManager->addNodeDefinition("myworkspace");
  // Input texture
  nodeDef->addTextureSourceName(
      "WindowRT", 0, Ogre::TextureDefinitionBase::TEXTURE_INPUT);
  nodeDef->setNumTargetPass(1);
  {
    Ogre::CompositorTargetDef *targetDef = nodeDef->addTargetPass("WindowRT");
    targetDef->setNumPasses(3);
    {
      {
        Ogre::CompositorPassClearDef *passClear =
            static_cast<Ogre::CompositorPassClearDef *>(
                targetDef->addPass(Ogre::PASS_CLEAR));
        Ogre::CompositorPassSceneDef *passScene =
            static_cast<Ogre::CompositorPassSceneDef *>(
                targetDef->addPass(Ogre::PASS_SCENE));
        passScene->mShadowNode = Ogre::IdString();

        // For the MyGUI pass
        targetDef->addPass(
            Ogre::PASS_CUSTOM, MyGUI::OgreCompositorPassProvider::mPassId);
      }
    }
  }
  Ogre::CompositorWorkspaceDef *workDef =
      pCompositorManager->addWorkspaceDefinition(workspaceName);
  workDef->connectOutput(nodeDef->getName(), 0);

  pCompositorManager->addWorkspace(
      mSceneManager, mRenderWindow, mCamera, workspaceNameHash, true);
}

//------------------------------------------------------------------------------
bool MyguiManager::frameStarted(const Ogre::FrameEvent &evt) {
  if (mExecuting) {
    mConsole->execute();
    mExecuting = false;
  }
  return true;
}

int main(int argc, char *argv[]) {
  MyguiManager mgr;
  mgr.go();
  return 0;
}