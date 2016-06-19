#include "BaseManager.h"

#include "OgreRoot.h"
#include "OgreException.h"
#include "OgreConfigFile.h"

#include "OgreRenderWindow.h"
#include "OgreCamera.h"
#include "OgreItem.h"

#include "OgreHlmsUnlit.h"
#include "OgreHlmsPbs.h"
#include "OgreHlmsManager.h"
#include "OgreArchiveManager.h"

#include "Compositor/OgreCompositorManager2.h"

#include "OgreWindowEventUtilities.h"

#include <SDL_syswm.h>

//------------------------------------------------------------------------------
BaseManager::BaseManager(void)
    : mRoot(0),
      mCamera(0),
      mSceneManager(0),
      mRenderWindow(0),
      mResourcesCfg(""),
      mPluginsCfg(""),
      mShutDown(false),
      mBackgroundColour(0, 0.4, 0.8, 1) {}

//------------------------------------------------------------------------------
BaseManager::~BaseManager(void) {
  // Remove ourself as a Window listener
  OGRE_DELETE mRoot;
  mRoot = 0;
  if (mSdlWindow) {
    // Restore desktop resolution on exit
    SDL_SetWindowFullscreen(mSdlWindow, 0);
    SDL_DestroyWindow(mSdlWindow);
    mSdlWindow = 0;
  }
  SDL_Quit();
}

//------------------------------------------------------------------------------
bool BaseManager::configure(void) {
  Ogre::String windowTitle("ogre");

  //if (!mRoot->showConfigDialog()) return false;
  if(!mRoot->restoreConfig()) return false;

  mRoot->getRenderSystem()->setConfigOption("sRGB Gamma Conversion", "Yes");
  mRoot->initialise(false);

  Ogre::ConfigOptionMap &cfgOpts = mRoot->getRenderSystem()->getConfigOptions();

  int width = 800;
  int height = 600;

  Ogre::ConfigOptionMap::iterator opt = cfgOpts.find("Video Mode");
  if (opt != cfgOpts.end()) {
    // Ignore leading space
    const Ogre::String::size_type start =
        opt->second.currentValue.find_first_of("012356789");
    // Get the width and height
    Ogre::String::size_type widthEnd =
        opt->second.currentValue.find(' ', start);
    // we know that the height starts 3 characters after the width and goes
    // until the next space
    Ogre::String::size_type heightEnd =
        opt->second.currentValue.find(' ', widthEnd + 3);
    // Now we can parse out the values
    width = Ogre::StringConverter::parseInt(
        opt->second.currentValue.substr(0, widthEnd));
    height = Ogre::StringConverter::parseInt(
        opt->second.currentValue.substr(widthEnd + 3, heightEnd));
  }

  int screen = 0;
  int posX = SDL_WINDOWPOS_CENTERED_DISPLAY(screen);
  int posY = SDL_WINDOWPOS_CENTERED_DISPLAY(screen);

  bool fullscreen =
      Ogre::StringConverter::parseBool(cfgOpts["Full Screen"].currentValue);
  if (fullscreen) {
    posX = SDL_WINDOWPOS_UNDEFINED_DISPLAY(screen);
    posY = SDL_WINDOWPOS_UNDEFINED_DISPLAY(screen);
  }

  mSdlWindow = SDL_CreateWindow(windowTitle.c_str(),  // window title
      posX,                                           // initial x position
      posY,                                           // initial y position
      width,                                          // width, in pixels
      height,                                         // height, in pixels
      SDL_WINDOW_SHOWN | (fullscreen ? SDL_WINDOW_FULLSCREEN : 0) |
          SDL_WINDOW_RESIZABLE);

  // Get the native whnd
  SDL_SysWMinfo wmInfo;
  SDL_VERSION(&wmInfo.version);

  if (SDL_GetWindowWMInfo(mSdlWindow, &wmInfo) == SDL_FALSE) {
    OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
        "Couldn't get WM Info! (SDL2)", "GraphicsSystem::initialize");
  }

  Ogre::String winHandle;
  Ogre::NameValuePairList params;

  switch (wmInfo.subsystem) {
#ifdef WIN32
    case SDL_SYSWM_WINDOWS:
      // Windows code
      winHandle =
          Ogre::StringConverter::toString((uintptr_t)wmInfo.info.win.window);
      break;
#elif __MACOSX__
    case SDL_SYSWM_COCOA:
      // required to make OGRE play nice with our window
      params.insert(std::make_pair("macAPI", "cocoa"));
      params.insert(std::make_pair("macAPICocoaUseNSView", "true"));

      winHandle =
          Ogre::StringConverter::toString(WindowContentViewHandle(wmInfo));
      break;
#else
    case SDL_SYSWM_X11:
      winHandle =
          Ogre::StringConverter::toString((uintptr_t)wmInfo.info.x11.window);
      break;
#endif
    default:
      OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "Unexpected WM! (SDL2)",
          "GraphicsSystem::initialize");
      break;
  }

  params.insert(std::make_pair("title", windowTitle));
  params.insert(std::make_pair("gamma", "true"));
  params.insert(std::make_pair("FSAA", cfgOpts["FSAA"].currentValue));
  params.insert(std::make_pair("vsync", cfgOpts["VSync"].currentValue));

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
  params.insert(std::make_pair("externalWindowHandle", winHandle));
#else
  params.insert(std::make_pair("parentWindowHandle", winHandle));
#endif

  mRenderWindow = Ogre::Root::getSingleton().createRenderWindow(
      windowTitle, width, height, fullscreen, &params);

  return true;
}
//------------------------------------------------------------------------------
void BaseManager::chooseSceneManager(void) {
  Ogre::InstancingThreadedCullingMethod threadedCullingMethod =
      Ogre::INSTANCING_CULLING_SINGLETHREAD;
#if OGRE_DEBUG_MODE
  // Debugging multithreaded code is a PITA, disable it.
  const size_t numThreads = 1;
#else
  // getNumLogicalCores() may return 0 if couldn't detect
  const size_t numThreads =
      std::max<size_t>(1, Ogre::PlatformInformation::getNumLogicalCores());
// See doxygen documentation regarding culling methods.
// In some cases you may still want to use single thread.
// if( numThreads > 1 )
//	threadedCullingMethod = Ogre::INSTANCING_CULLING_THREADED;
#endif
  // Create the SceneManager, in this case a generic one
  mSceneManager = mRoot->createSceneManager(
      Ogre::ST_GENERIC, numThreads, threadedCullingMethod, "ExampleSMInstance");

  // mSceneManager->addRenderQueueListener(mOverlaySystem);

  // Set sane defaults for proper shadow mapping
  mSceneManager->setShadowDirectionalLightExtrusionDistance(500.0f);
  mSceneManager->setShadowFarDistance(500.0f);
}
//------------------------------------------------------------------------------
void BaseManager::createCamera(void) {
  mCamera = mSceneManager->createCamera("Main_Camera");
  mCamera->setPosition(Ogre::Vector3(0, 5, 15));
  mCamera->lookAt(Ogre::Vector3(0, 0, 0));
  mCamera->setNearClipDistance(0.2f);
  mCamera->setFarClipDistance(1000.0f);
  mCamera->setAutoAspectRatio(true);
}
//------------------------------------------------------------------------------
void BaseManager::createFrameListener(void) {
  size_t windowHnd = 0;
  std::ostringstream windowHndStr;
  mRenderWindow->getCustomAttribute("WINDOW", &windowHnd);
  windowHndStr << windowHnd;
  // Register as a Window listener
  Ogre::WindowEventUtilities::addWindowEventListener(mRenderWindow, this);
  mRoot->addFrameListener(this);
}

//------------------------------------------------------------------------------
void BaseManager::createScene(void) {}
//------------------------------------------------------------------------------
void BaseManager::destroyScene(void) {}
//------------------------------------------------------------------------------
void BaseManager::setupResources(void) {
  // Load resource paths from config file
  Ogre::ConfigFile cf;
  cf.load(mResourcesCfg);

  // Go through all sections & settings in the file
  Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

  Ogre::String secName, typeName, archName;
  while (seci.hasMoreElements()) {
    secName = seci.peekNextKey();
    if (secName == "Hlms") {
      registerHlms(cf.getSetting("DoNotUseAsResource", "Hlms", ""));
      seci.moveNext();
      continue;
    }

    Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
    Ogre::ConfigFile::SettingsMultiMap::iterator i;
    for (i = settings->begin(); i != settings->end(); ++i) {
      typeName = i->first;
      archName = i->second;
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
          archName, typeName, secName);
    }
  }
}
//------------------------------------------------------------------------------
void BaseManager::createResourceListener(void) {}
//------------------------------------------------------------------------------
void BaseManager::loadResources(void) {
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

//------------------------------------------------------------------------------
void BaseManager::registerHlms(const Ogre::String &folder) {
  Ogre::String dataFolder(folder);
  if (dataFolder.empty())
    dataFolder = "./";
  else if (*(dataFolder.end() - 1) != '/')
    dataFolder += "/";

  Ogre::RenderSystem *renderSystem = mRoot->getRenderSystem();

  Ogre::String shaderSyntax = "GLSL";
  if (renderSystem->getName() == "Direct3D11 Rendering Subsystem")
    shaderSyntax = "HLSL";

  Ogre::Archive *archiveLibrary = Ogre::ArchiveManager::getSingletonPtr()->load(
      dataFolder + "Hlms/Common/" + shaderSyntax, "FileSystem", true);

  Ogre::ArchiveVec library;
  library.push_back(archiveLibrary);

  Ogre::Archive *archiveUnlit = Ogre::ArchiveManager::getSingletonPtr()->load(
      dataFolder + "Hlms/Unlit/" + shaderSyntax, "FileSystem", true);

  Ogre::HlmsUnlit *hlmsUnlit = OGRE_NEW Ogre::HlmsUnlit(archiveUnlit, &library);
  Ogre::Root::getSingleton().getHlmsManager()->registerHlms(hlmsUnlit);

  Ogre::Archive *archivePbs = Ogre::ArchiveManager::getSingletonPtr()->load(
      dataFolder + "Hlms/Pbs/" + shaderSyntax, "FileSystem", true);
  Ogre::HlmsPbs *hlmsPbs = OGRE_NEW Ogre::HlmsPbs(archivePbs, &library);
  Ogre::Root::getSingleton().getHlmsManager()->registerHlms(hlmsPbs);

  if (renderSystem->getName() == "Direct3D11 Rendering Subsystem") {
    // Set lower limits 512kb instead of the default 4MB per Hlms in D3D 11.0
    // and below to avoid saturating AMD's discard limit (8MB) or
    // saturate the PCIE bus in some low end machines.
    bool supportsNoOverwriteOnTextureBuffers;
    renderSystem->getCustomAttribute("MapNoOverwriteOnDynamicBufferSRV",
        &supportsNoOverwriteOnTextureBuffers);

    if (!supportsNoOverwriteOnTextureBuffers) {
      hlmsPbs->setTextureBufferDefaultSize(512 * 1024);
      hlmsUnlit->setTextureBufferDefaultSize(512 * 1024);
    }
  }
}

//------------------------------------------------------------------------------
void BaseManager::handleSdlEvent(const SDL_Event &evt) {
  switch (evt.type) {
    // drop file events
    case SDL_DROPFILE:
      break;
    // windows events
    case SDL_WINDOWEVENT:
      switch (evt.window.event) {
        case SDL_WINDOWEVENT_CLOSE:
          mShutDown = true;
          break;
        case SDL_WINDOWEVENT_RESIZED:
          //_windowResized(evt.window.data1, evt.window.data2);
          break;
        case SDL_WINDOWEVENT_SHOWN:
        case SDL_WINDOWEVENT_RESTORED:
        case SDL_WINDOWEVENT_EXPOSED:
        case SDL_WINDOWEVENT_MAXIMIZED:
          // mWindowOn = true;
          break;
        case SDL_WINDOWEVENT_MINIMIZED:
        case SDL_WINDOWEVENT_HIDDEN:
        // mWindowOn = false;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

//------------------------------------------------------------------------------
void BaseManager::setupCompositor() {
  Ogre::CompositorManager2 *compositorManager = mRoot->getCompositorManager2();

  const Ogre::IdString workspaceName("Demo Workspace");
  if (!compositorManager->hasWorkspaceDefinition(workspaceName)) {
    compositorManager->createBasicWorkspaceDef(
        workspaceName, mBackgroundColour, Ogre::IdString());
  }

  compositorManager->addWorkspace(
      mSceneManager, mRenderWindow, mCamera, workspaceName, true);
}
//------------------------------------------------------------------------------
void BaseManager::go(void) {
  if (!setup()) return;

  while (!mShutDown) {
    Ogre::WindowEventUtilities::messagePump();
    SDL_Event evt;
    while (SDL_PollEvent(&evt)) {
      handleSdlEvent(evt);
    }

    update();

    if (mRenderWindow->isVisible())
      mShutDown |= !mRoot->renderOneFrame();
    else
      Ogre::Threads::Sleep(500);
  }

  // clean up
  destroyScene();
}
//------------------------------------------------------------------------------
bool BaseManager::setup(void) {
#ifdef _DEBUG
  mResourcesCfg = "resources_d.cfg";
  mPluginsCfg = "plugins_d.cfg";
#else
  mResourcesCfg = "resources.cfg";
  mPluginsCfg = "plugins.cfg";
#endif

  mRoot = new Ogre::Root(mPluginsCfg);

  bool carryOn = configure();
  if (!carryOn) return false;

  setupResources();

  // Create any resource listeners (for loading screens)
  createResourceListener();
  // Load resources
  loadResources();

  chooseSceneManager();
  createCamera();

  initGui();
  setupCompositor();

  // Create the scene
  createScene();

  mRoot->addFrameListener(this);

  // createFrameListener();
  return true;
}
