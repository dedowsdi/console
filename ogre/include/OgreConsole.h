#ifndef OGRECONOSLE_H
#define OGRECONOSLE_H

#include "OgreConsolePreRequisite.h"
#include "pacConsole.h"

namespace pac {

class _PacExport OgreConsole : public Console {
public:
  OgreConsole(Ogre::SceneManager* sceneMgr);

  /**
   * register following arghandler
   *
   * moType
   * movable
   * particle
   * light
   * camara
   * entity
   * node
   * ltl_sceneNode
   * ltl_tagPoint
   * ltl_light
   * ltl_entity
   * ltl_particle
   * ltl_direct
   * ltl_all
   * en_lightType
   */
  virtual void initArghandler();
  virtual void initCommand();
  virtual void initDir();

  /**
   * The scene need better be freezed when you want to change properties of some
   * movable object  or scenenode, otherwise if the editting object got removed
   * from scene, your next set type command will crash.
   */
  virtual void freeze() { this->outputLine("freezing....."); }
  /**
   * restore back scene
   */
  virtual void melt() { this->outputLine("melting....."); }
  virtual bool isFrozen() { return false; }

  Ogre::SceneManager* getSceneMgr() const { return mSceneMgr; }
  void setSceneMgr(Ogre::SceneManager* v) { mSceneMgr = v; }

  AbsDir* getMovableDir() const { return mMovableDir; }
  AbsDir* getNodeDir() const { return mNodeDir; }

protected:
  Ogre::SceneManager* mSceneMgr;
  AbsDir* mMovableDir; 
  AbsDir* mNodeDir;
};
}

#endif /* OGRECONOSLE_H */
