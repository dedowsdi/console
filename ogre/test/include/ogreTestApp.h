/*
-----------------------------------------------------------------------------
Filename:    OgreTestApp.h
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
#ifndef OGRETESTAPP_H
#define OGRETESTAPP_H

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreRenderWindow.h>
#include <OgreFrameListener.h>
#include <OgreWindowEventUtilities.h>
#include <OgreTextureManager.h>

class OgreTestApp : public Ogre::FrameListener,
                        public Ogre::WindowEventListener {
public:
  OgreTestApp(void);
  virtual ~OgreTestApp(void);

  virtual void go(void);

  Ogre::SceneManager* getSceneMgr() const { return mSceneMgr; }
  void setSceneMgr( Ogre::SceneManager* v){mSceneMgr = v;}

  virtual bool setup();

protected:
  virtual bool configure(void);
  virtual void chooseSceneManager(void);
  virtual void createCamera(void);
  virtual void createFrameListener(void);
  virtual void createScene(void);
  virtual void destroyScene(void);
  virtual void setupResources(void);
  virtual void createResourceListener(void);
  virtual void loadResources(void);


  Ogre::Root* mRoot;
  Ogre::Camera* mCamera;
  Ogre::SceneManager* mSceneMgr;
  Ogre::RenderWindow* mWindow;
  Ogre::String mResourcesCfg;
  Ogre::String mPluginsCfg;

  // OgreBites
  bool mCursorWasVisible;  // was cursor visible before dialog appeared
  bool mShutDown;
};

#endif  // #ifndef OGRETESTAPP_H
