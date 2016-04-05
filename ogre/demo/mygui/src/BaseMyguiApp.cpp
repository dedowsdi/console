#include "BaseMyguiApp.h"
#include <OgreFrameStats.h>
#include <Compositor/OgreCompositorManager2.h>
#include <Compositor/OgreCompositorNodeDef.h>
#include <Compositor/Pass/OgreCompositorPassDef.h>
#include <Compositor/Pass/PassClear/OgreCompositorPassClearDef.h>
#include <Compositor/Pass/PassScene/OgreCompositorPassSceneDef.h>
#include <Compositor/OgreCompositorWorkspaceDef.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <macUtils.h>
#endif

//---------------------------------------------------------------------------
BaseMyguiApp::BaseMyguiApp(void)
    : mRoot(0),
      mCamera(0),
      mSceneMgr(0),
      mWindow(0),
      mResourcesCfg(""),
      mPluginsCfg(""),
      mConsole(0),
      mConsoleUI(0),
      mCursorWasVisible(false),
      mShutDown(false),
      mExecuteing(false),
      mInputManager(0),
      mMouse(0),
      mKeyboard(0),
      mInfo(nullptr)

{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
  m_ResourcePath = Ogre::macBundlePath() + "/Contents/Resources/";
#else
  m_ResourcePath = "";
#endif
}

//---------------------------------------------------------------------------
BaseMyguiApp::~BaseMyguiApp(void) {
  if (mInfo) delete mInfo;

  // Remove ourself as a Window listener
  Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
  windowClosed(mWindow);
  delete mRoot;
}

//---------------------------------------------------------------------------
bool BaseMyguiApp::configure(void) {
  // Show the configuration dialog and initialise the system.
  // You can skip this and use root.restoreConfig() to load configuration
  // settings if you were sure there are valid ones saved in ogre.cfg.
   if(mRoot->showConfigDialog()){
  //if (mRoot->restoreConfig()) {
    // If returned true, user clicked OK so initialise.
    // Here we choose to let the system create a default rendering window by
    // passing 'true'.
    mWindow = mRoot->initialise(true, "TutorialApplication Render Window");

    return true;
  } else {
    return false;
  }
}

//---------------------------------------------------------------------------
void BaseMyguiApp::chooseSceneManager(void) {
  // Get the SceneManager, in this case a generic one
  mSceneMgr = mRoot->createSceneManager(
      Ogre::ST_GENERIC, 1, Ogre::INSTANCING_CULLING_SINGLETHREAD);
}
//---------------------------------------------------------------------------
void BaseMyguiApp::createCamera(void) {
  // Create the camera
  mCamera = mSceneMgr->createCamera("PlayerCam");

  // Look back along -Z
  mCamera->lookAt(Ogre::Vector3(0, 0, -1));
  mCamera->setNearClipDistance(5);
  mCamera->detachFromParent();

  auto node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
  node->setName("cn0");
  node->attachObject(mCamera);
  node->setPosition(0, 0, 80);

  // mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // Create a default
  // camera controller
}
//---------------------------------------------------------------------------
void BaseMyguiApp::createFrameListener(void) {
  Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
  OIS::ParamList pl;
  size_t windowHnd = 0;
  std::ostringstream windowHndStr;

  mWindow->getCustomAttribute("WINDOW", &windowHnd);
  windowHndStr << windowHnd;
  pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

#if defined OIS_WIN32_PLATFORM
  pl.insert(std::make_pair(
      std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
  pl.insert(std::make_pair(
      std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
  pl.insert(std::make_pair(
      std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
  pl.insert(std::make_pair(
      std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
  pl.insert(
      std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
  pl.insert(
      std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
  pl.insert(
      std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
  pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif

  mInputManager = OIS::InputManager::createInputSystem(pl);

  mKeyboard = static_cast<OIS::Keyboard *>(
      mInputManager->createInputObject(OIS::OISKeyboard, true));
  mMouse = static_cast<OIS::Mouse *>(
      mInputManager->createInputObject(OIS::OISMouse, true));

  mMouse->setEventCallback(this);
  mKeyboard->setEventCallback(this);

  // Set initial mouse clipping size
  windowResized(mWindow);

  // Register as a Window listener
  Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

  mRoot->addFrameListener(this);
}

//------------------------------------------------------------------------------
void BaseMyguiApp::createScene(void) {
  Ogre::Entity *ent = mSceneMgr->createEntity("ogrehead.mesh");
  ent->setName("ogrehead");
  auto node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
  node->setName("en0");
  node->attachObject(ent);
  auto l = mSceneMgr->createLight();
  l->setName("light");
  node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
  node->setName("ln0");
  node->setPosition(0, 50, 0);
  node->attachObject(l);
}
//---------------------------------------------------------------------------
void BaseMyguiApp::destroyScene(void) {}
//---------------------------------------------------------------------------
void BaseMyguiApp::createWorkspace(void) {
  const Ogre::IdString workspaceName("MyOwnWorkspace");
  // Ogre::CompositorManager2 *compositorManager =
  // mRoot->getCompositorManager2();
  // if (!compositorManager->hasWorkspaceDefinition(workspaceName))
  // compositorManager->createBasicWorkspaceDef(
  // workspaceName, Ogre::ColourValue(0.0f, 0.0f, 0.0f));
  // compositorManager->addWorkspace(
  // mSceneMgr, mWindow, mCamera, workspaceName, true);

  Ogre::CompositorManager2 *pCompositorManager =
      Ogre::Root::getSingleton().getCompositorManager2();
  Ogre::CompositorNodeDef *nodeDef =
      pCompositorManager->addNodeDefinition("myworkspace");
  // Input texture
  nodeDef->addTextureSourceName(
      "WindowRT", 0, Ogre::TextureDefinitionBase::TEXTURE_INPUT);
  // add target pass
  nodeDef->setNumTargetPass(1);
  Ogre::CompositorTargetDef *targetDef = nodeDef->addTargetPass("WindowRT");
  // add clear, scene, mygui passes
  targetDef->setNumPasses(3);
  auto passClear = static_cast<Ogre::CompositorPassClearDef *>(
      targetDef->addPass(Ogre::PASS_CLEAR));
  passClear->mColourValue = Ogre::ColourValue(0, 0, 0);
  targetDef->addPass(Ogre::PASS_SCENE);
  targetDef->addPass(
      Ogre::PASS_CUSTOM, MyGUI::OgreCompositorPassProvider::mPassId);
  Ogre::CompositorWorkspaceDef *workDef =
      pCompositorManager->addWorkspaceDefinition(workspaceName);
  workDef->connectOutput(nodeDef->getName(), 0);

  pCompositorManager->addWorkspace(
      mSceneMgr, mWindow, mCamera, workspaceName, true);
}
//---------------------------------------------------------------------------
void BaseMyguiApp::setupResources(void) {
  // Load resource paths from config file
  Ogre::ConfigFile cf;
  cf.load(mResourcesCfg);

  // Go through all sections & settings in the file
  Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

  Ogre::String secName, typeName, archName;
  while (seci.hasMoreElements()) {
    secName = seci.peekNextKey();
    Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
    Ogre::ConfigFile::SettingsMultiMap::iterator i;
    for (i = settings->begin(); i != settings->end(); ++i) {
      typeName = i->first;
      archName = i->second;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
      // OS X does not set the working directory relative to the app.
      // In order to make things portable on OS X we need to provide
      // the loading with it's own bundle path location.
      if (!Ogre::StringUtil::startsWith(
              archName, "/", false))  // only adjust relative directories
        archName = Ogre::String(Ogre::macBundlePath() + "/" + archName);
#endif

      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
          archName, typeName, secName);
    }
  }
}
//---------------------------------------------------------------------------
void BaseMyguiApp::createResourceListener(void) {}
//---------------------------------------------------------------------------
void BaseMyguiApp::loadResources(void) {
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//---------------------------------------------------------------------------
void BaseMyguiApp::go(void) {
#ifdef _DEBUG
#ifndef OGRE_STATIC_LIB
  mResourcesCfg = m_ResourcePath + "resources_d.cfg";
  mPluginsCfg = m_ResourcePath + "plugins_d.cfg";
#else
  mResourcesCfg = "resources_d.cfg";
  mPluginsCfg = "plugins_d.cfg";
#endif
#else
#ifndef OGRE_STATIC_LIB
  mResourcesCfg = m_ResourcePath + "resources.cfg";
  mPluginsCfg = m_ResourcePath + "plugins.cfg";
#else
  mResourcesCfg = "resources.cfg";
  mPluginsCfg = "plugins.cfg";
#endif
#endif

  if (!setup()) return;

  mRoot->startRendering();

  // Clean up
  destroyScene();
}

//---------------------------------------------------------------------------
bool BaseMyguiApp::setup(void) {
  mRoot = new Ogre::Root(mPluginsCfg);

  setupResources();

  bool carryOn = configure();
  if (!carryOn) return false;

  chooseSceneManager();
  createCamera();

  // Set default mipmap level (NB some APIs ignore this)
  Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

  // Create any resource listeners (for loading screens)
  createResourceListener();
  // Load resources
  loadResources();

  // Create the scene
  createScene();

  createFrameListener();

  initGui();
  initConsole();
  createWorkspace();

  return true;
};
//---------------------------------------------------------------------------
bool BaseMyguiApp::frameRenderingQueued(const Ogre::FrameEvent &evt) {
  if (mWindow->isClosed()) return false;

  if (mShutDown) return false;

  // Need to capture/update each device
  mKeyboard->capture();
  mMouse->capture();

  // mCameraMan->frameRenderingQueued(evt);

  return true;
}

//------------------------------------------------------------------------------
bool BaseMyguiApp::frameStarted(const Ogre::FrameEvent &evt) {
  if (mExecuteing) {
    mConsole->execute();
    mExecuteing = false;
  }
  return true;
}

//------------------------------------------------------------------
void BaseMyguiApp::initGui() {
  mPlatform = new MyGUI::OgrePlatform();
  Ogre::WindowEventUtilities::addWindowEventListener(
      mWindow, MyGUI::OgreRenderManager::getInstancePtr());
  // mWindow is Ogre::RenderWindow*, mSceneMgr is Ogre::SceneManager*
  mPlatform->initialise(mWindow, "Mygui");

  mGUI = new MyGUI::Gui();
  mGUI->initialise();
  mInfo = new diagnostic::StatisticInfo();
  mGUI->eventFrameStart +=
      MyGUI::newDelegate(this, &BaseMyguiApp::notifyFrameStartUpdateStatistic);
  mConsoleUI = new pac::MyguiConsoleUI();
}

//------------------------------------------------------------------------------
void BaseMyguiApp::initConsole() {
  // mConsole = new OgreConsole("mSceneMgr")
  mConsole = new pac::OgreConsole(mConsoleUI, mSceneMgr);
  mConsole->init();
}

//---------------------------------------------------------------------------
bool BaseMyguiApp::keyPressed(const OIS::KeyEvent &arg) {
  if (mExecuteing) {
    return true;
  }
  if (arg.key == OIS::KC_GRAVE) {
    mConsole->toggleActive();
    return true;
  }
  MyGUI::InputManager::getInstance().injectKeyPress(
      MyGUI::KeyCode::Enum(arg.key), arg.text);

  if (mConsole->isActive()) {
    switch (arg.key) {
      case OIS::KC_RETURN:
        // you can't execute it directly, you need to do it at frameStarted
        mExecuteing = true;
        // mConsole->execute();
        break;
      case OIS::KC_TAB:
        mConsole->prompt();
        break;
      case OIS::KC_UP:
        mConsole->rollCommand(true);
        break;
      case OIS::KC_DOWN:
        mConsole->rollCommand(false);
        break;
      default:
        break;
    }
    return true;
  }

  return true;
}

//---------------------------------------------------------------------------
bool BaseMyguiApp::keyReleased(const OIS::KeyEvent &arg) {
  MyGUI::InputManager::getInstance().injectKeyRelease(
      MyGUI::KeyCode::Enum(arg.key));
  return true;
}

//---------------------------------------------------------------------------
bool BaseMyguiApp::mouseMoved(const OIS::MouseEvent &arg) {
  MyGUI::InputManager::getInstance().injectMouseMove(
      arg.state.X.abs, arg.state.Y.abs, arg.state.Z.abs);
  return true;
}
//---------------------------------------------------------------------------
bool BaseMyguiApp::mousePressed(
    const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
  MyGUI::InputManager::getInstance().injectMousePress(
      arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
  return true;
}
//---------------------------------------------------------------------------
bool BaseMyguiApp::mouseReleased(
    const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
  MyGUI::InputManager::getInstance().injectMouseRelease(
      arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
  return true;
}
//---------------------------------------------------------------------------
// Adjust mouse clipping area
void BaseMyguiApp::windowResized(Ogre::RenderWindow *rw) {
  unsigned int width, height, depth;
  int left, top;
  rw->getMetrics(width, height, depth, left, top);

  const OIS::MouseState &ms = mMouse->getMouseState();
  ms.width = width;
  ms.height = height;
  //if (mConsole) mConsole->resize();
}
//---------------------------------------------------------------------------
// Unattach OIS before window shutdown (very important under Linux)
void BaseMyguiApp::windowClosed(Ogre::RenderWindow *rw) {
  // Only close for window that created OIS (the main window in these demos)
  if (rw == mWindow) {
    if (mInputManager) {
      mInputManager->destroyInputObject(mMouse);
      mInputManager->destroyInputObject(mKeyboard);

      OIS::InputManager::destroyInputSystem(mInputManager);
      mInputManager = 0;
    }
  }
}

//------------------------------------------------------------------
MyGUI::MapString BaseMyguiApp::getStatistic() {
  MyGUI::MapString result;

  try {
    const Ogre::FrameStats *fs = Ogre::Root::getSingleton().getFrameStats();
    const Ogre::RenderWindow::FrameStats &stats = mWindow->getStatistics();
    result["FPS"] = MyGUI::utility::toString(fs->getAvgFps());
    result["triangle"] = MyGUI::utility::toString(stats.triangleCount);
    result["batch"] = MyGUI::utility::toString(stats.batchCount);
    result["batch gui"] = MyGUI::utility::toString(
        MyGUI::OgreRenderManager::getInstance().getBatchCount());
  } catch (...) {
    MYGUI_LOG(Warning, "Error get statistics");
  }

  return result;
}

//------------------------------------------------------------------
void BaseMyguiApp::notifyFrameStartUpdateStatistic(Ogre::Real dt) {
  if (mInfo != nullptr) {
    static float time = 0;
    time += dt;
    if (time > 1) {
      time -= 1;

      MyGUI::MapString statistic = getStatistic();
      for (MyGUI::MapString::const_iterator info = statistic.begin();
           info != statistic.end(); info++)
        mInfo->change((*info).first, (*info).second);
      mInfo->update();
    }
  }
}

//---------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char *argv[])
#endif
{
  // Create application object
  BaseMyguiApp app;

  try {
    app.go();
  } catch (Ogre::Exception &e) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    MessageBox(NULL, e.getFullDescription().c_str(),
        "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
    std::cerr << "An exception has occurred: " << e.getFullDescription().c_str()
              << std::endl;
#endif
  }

  return 0;
}

#ifdef __cplusplus
}
#endif
