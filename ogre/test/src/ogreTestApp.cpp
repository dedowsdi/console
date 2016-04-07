/*
-----------------------------------------------------------------------------
Filename:    OgreTestApp.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#include "ogreTestApp.h"
#include <OgreArchiveManager.h>
#include <OgreHlms.h>
#include <OgreHlmsPbs.h>
#include <OgreHlmsManager.h>

//------------------------------------------------------------------------------
OgreTestApp::OgreTestApp(void)
    : mRoot(0),
      mCamera(0),
      mSceneMgr(0),
      mWindow(0),
      mResourcesCfg(""),
      mPluginsCfg(""),
      mCursorWasVisible(false),
      mShutDown(false) {}

//------------------------------------------------------------------------------
OgreTestApp::~OgreTestApp(void) {
  // Remove ourself as a Window listener
  Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
  windowClosed(mWindow);
  delete mRoot;
}

//------------------------------------------------------------------------------
bool OgreTestApp::configure(void) {
  // Show the configuration dialog and initialise the system
  // You can skip this and use root.restoreConfig() to load configuration
  // settings if you were sure there are valid ones saved in ogre.cfg
  // if (mRoot->showConfigDialog()) {
  if (mRoot->restoreConfig()) {
    // If returned true, user clicked OK so initialise
    // Here we choose to let the system create a default rendering window by
    // passing 'true'
    mWindow = mRoot->initialise(true, "TutorialApplication Render Window");

    return true;
  } else {
    return false;
  }
}
//------------------------------------------------------------------------------
void OgreTestApp::chooseSceneManager(void) {
  // Get the SceneManager, in this case a generic one
  mSceneMgr = mRoot->createSceneManager(
      Ogre::ST_GENERIC, 1, Ogre::INSTANCING_CULLING_SINGLETHREAD);
}
//------------------------------------------------------------------------------
void OgreTestApp::createCamera(void) {
  // Create the camera
  mCamera = mSceneMgr->createCamera("PlayerCam");
  // Position it at 500 in Z direction
  mCamera->setPosition(Ogre::Vector3(0, 0, 80));
  // Look back along -Z
  mCamera->lookAt(Ogre::Vector3(0, 0, -300));
  mCamera->setNearClipDistance(5);
}
//------------------------------------------------------------------------------
void OgreTestApp::createFrameListener(void) {
  size_t windowHnd = 0;
  std::ostringstream windowHndStr;
  mWindow->getCustomAttribute("WINDOW", &windowHnd);
  windowHndStr << windowHnd;
  // Register as a Window listener
  Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
  mRoot->addFrameListener(this);
}

//------------------------------------------------------------------------------
void OgreTestApp::createScene(void) {}
//------------------------------------------------------------------------------
void OgreTestApp::destroyScene(void) {}
//------------------------------------------------------------------------------
void OgreTestApp::setupResources(void) {
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
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
          archName, typeName, secName);
    }
  }
}
//------------------------------------------------------------------------------
void OgreTestApp::createResourceListener(void) {}
//------------------------------------------------------------------------------
void OgreTestApp::loadResources(void) {
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

//------------------------------------------------------------------------------
void OgreTestApp::initHlms() {
  Ogre::Archive *archiveLibrary = Ogre::ArchiveManager::getSingletonPtr()->load(
      "/usr/local/source/ogre/ogre2.1/Samples/Media/Hlms/Common/GLSL",
      "FileSystem", true);
  Ogre::ArchiveVec library;
  library.push_back(archiveLibrary);

  Ogre::Archive *archivePbs = Ogre::ArchiveManager::getSingletonPtr()->load(
      "/usr/local/source/ogre/ogre2.1/Samples/Media/Hlms/Pbs/GLSL",
      "FileSystem", true);
  Ogre::HlmsPbs *hlmsPbs = OGRE_NEW Ogre::HlmsPbs(archivePbs, &library);
  Ogre::Root::getSingleton().getHlmsManager()->registerHlms(hlmsPbs);
}
//------------------------------------------------------------------------------
void OgreTestApp::go(void) {
  if (!setup()) return;

  mRoot->startRendering();

  // clean up
  destroyScene();
}
//------------------------------------------------------------------------------
bool OgreTestApp::setup(void) {
#ifdef _DEBUG
  mResourcesCfg = "resources_d.cfg";
  mPluginsCfg = "plugins_d.cfg";
#else
  mResourcesCfg = "resources.cfg";
  mPluginsCfg = "plugins.cfg";
#endif

  mRoot = new Ogre::Root(mPluginsCfg);

  setupResources();

  bool carryOn = configure();
  if (!carryOn) return false;

  initHlms();

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

  return true;
};
