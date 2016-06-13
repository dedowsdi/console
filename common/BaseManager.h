#ifndef BASEMANAGER_H
#define BASEMANAGER_H

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
#include <Compositor/OgreCompositorWorkspace.h>
#include <SDL.h>

class BaseManager : public Ogre::FrameListener,
                    public Ogre::WindowEventListener {
public:
  bool mShutDown;
  Ogre::Root* mRoot;
  Ogre::Camera* mCamera;
  Ogre::SceneManager* mSceneManager;
  Ogre::RenderWindow* mRenderWindow;
  Ogre::String mResourcesCfg;
  Ogre::String mPluginsCfg;
  Ogre::ColourValue mBackgroundColour;

  SDL_Window *mSdlWindow;

public:
  BaseManager(void);
  virtual ~BaseManager(void);

  virtual void go(void);

  Ogre::SceneManager* getSceneMgr() const { return mSceneManager; }
  void setSceneMgr(Ogre::SceneManager* v) { mSceneManager = v; }

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
  virtual void registerHlms(const Ogre::String& folder);
  virtual void initGui(){}
  virtual void update(){}
  virtual void handleSdlEvent(const SDL_Event& evt);
  virtual void setupCompositor();
};

#endif /* BASEMANAGER_H */
