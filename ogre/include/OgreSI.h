#ifndef OGRESI_H
#define OGRESI_H

#include "pacConsolePreRequisite.h"
#include "pacStringInterface.h"
#include "OgreMovableObject.h"

namespace pac {

class _PacExport MovableSI : public StringInterface {
public:
  MovableSI(Ogre::MovableObject* obj);

protected:
  void initParams();
  Ogre::MovableObject* mMo;
};

class _PacExport LightSI : public MovableSI {
public:
  LightSI(Ogre::Light* light);

protected:
  void initParams();
};

class _PacExport CameraSI : public MovableSI {
public:
  CameraSI(Ogre::Camera* camera);

protected:
  void initParams();
};

class _PacExport EntitySI : public MovableSI {
public:
  EntitySI(Ogre::Entity* ent);

protected:
  void initParams();
};

class _PacExport SceneSI : public MovableSI {
public:
  SceneSI(Ogre::SceneManager* sceneMgr);

  Ogre::SceneManager* getSceneMgr() const { return mSceneMgr; }
  void setSceneMgr(Ogre::SceneManager* v) { mSceneMgr = v; }

private:
  Ogre::SceneManager* mSceneMgr;
};
}

#endif /* OGRESI_H */
