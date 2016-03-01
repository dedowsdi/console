#ifndef OGRECONOSLE_H
#define OGRECONOSLE_H 

#include "OgreConsolePrerequisites.h"
#include "pacConsole.h"

namespace pac
{


class _PacExport OgreConsole : public Console
{
public:
	OgreConsole(Ogre::SceneManager* sceneMgr);

  /**
   * register following arghandler
   *
   * mot
   * movable
   * removable
   * particle
   * reparticle
   * light
   * relight
   * camara
   * recamara
   * entity
   * reentity
   * node
   * renode
   */
  virtual void initArghandler();
  virtual void initCommand();
  virtual void initDir();

  Ogre::SceneManager* getSceneMgr() const { return mSceneMgr; }
  void setSceneMgr( Ogre::SceneManager* v){mSceneMgr = v;}

protected:
  Ogre::SceneManager* mSceneMgr;
};


}


#endif /* OGRECONOSLE_H */
