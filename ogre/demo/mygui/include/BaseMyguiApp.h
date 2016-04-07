#ifndef BASEMYGUIAPP_H
#define BASEMYGUIAPP_H

#include <OgreCamera.h>
#include <OgreItem.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <OIS/OISEvents.h>
#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>

#else
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#endif

#ifdef OGRE_STATIC_LIB
#define OGRE_STATIC_GL
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define OGRE_STATIC_Direct3D9
// D3D10 will only work on vista, so be careful about statically linking
#if OGRE_USE_D3D10
#define OGRE_STATIC_Direct3D10
#endif
#endif
#define OGRE_STATIC_BSPSceneManager
#define OGRE_STATIC_ParticleFX
#define OGRE_STATIC_CgProgramManager
#ifdef OGRE_USE_PCZ
#define OGRE_STATIC_PCZSceneManager
#define OGRE_STATIC_OctreeZone
#else
#define OGRE_STATIC_OctreeSceneManager
#endif
#include "OgreStaticPluginLoader.h"
#endif

#include <MyGUI.h>
#include <MyGUI_Ogre2Platform.h>
#include <StatisticInfo.h>
#include "OgreConsole.h"
#include "MyguiConsoleUI.h"

#define sgmLayoutMgr MyGUI::LayoutManager::getInstance()
#define sgmRenderMgr MyGUI::RenderManager::getInstance()

//---------------------------------------------------------------------------
class BaseMyguiApp : public Ogre::FrameListener,
                     public Ogre::WindowEventListener,
                     public OIS::KeyListener,
                     public OIS::MouseListener {
public:
  BaseMyguiApp(void);
  virtual ~BaseMyguiApp(void);

  virtual void go(void);

protected:
  virtual bool setup();

  virtual bool configure(void);
  virtual void chooseSceneManager(void);
  virtual void createCamera(void);
  virtual void createFrameListener(void);
  virtual void createScene(void);
  virtual void destroyScene(void);
  virtual void createWorkspace(void);
  virtual void setupResources(void);
  virtual void createResourceListener(void);
  virtual void loadResources(void);
  virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
  virtual bool frameStarted(const Ogre::FrameEvent& evt);
  virtual void initGui();
  virtual void initConsole();
  void initHlms();

  virtual bool keyPressed(const OIS::KeyEvent& arg);
  virtual bool keyReleased(const OIS::KeyEvent& arg);
  virtual bool mouseMoved(const OIS::MouseEvent& arg);
  virtual bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
  virtual bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);

  // Adjust mouse clipping area
  virtual void windowResized(Ogre::RenderWindow* rw);
  // Unattach OIS before window shutdown (very important under Linux)
  virtual void windowClosed(Ogre::RenderWindow* rw);

  MyGUI::MapString getStatistic();
  void notifyFrameStartUpdateStatistic(Ogre::Real dt);

  Ogre::Root* mRoot;
  Ogre::Camera* mCamera;
  Ogre::SceneManager* mSceneMgr;
  Ogre::RenderWindow* mWindow;
  Ogre::String mResourcesCfg;
  Ogre::String mPluginsCfg;
  pac::OgreConsole* mConsole;
  pac::MyguiConsoleUI* mConsoleUI;

  bool mCursorWasVisible;  // Was cursor visible before dialog appeared?
  bool mShutDown;
  bool mExecuteing;

  // OIS Input devices
  OIS::InputManager* mInputManager;
  OIS::Mouse* mMouse;
  OIS::Keyboard* mKeyboard;

  // Added for Mac compatibility
  Ogre::String m_ResourcePath;
  // mygui
  MyGUI::Gui* mGUI;
  MyGUI::Ogre2Platform* mPlatform;
  diagnostic::StatisticInfo* mInfo;

#ifdef OGRE_STATIC_LIB
  Ogre::StaticPluginLoader m_StaticPluginLoader;
#endif
};

#endif /* BASEMYGUIAPP_H */
