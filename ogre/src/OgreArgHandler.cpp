#include "pacStable.h"
#include "OgreArgHandler.h"
#include "OgreConsole.h"
#include "Ogre.h"

namespace pac {

//------------------------------------------------------------------------------
void MovableAH::runtimeInit() {
  Ogre::SceneManager* sceneMgr = sgOgreConsole.getSceneMgr();
  Ogre::SceneManager::MovableObjectIterator moi =
      sceneMgr->getMovableObjectIterator(mMovableType);
  mStrings.clear();
  std::for_each(moi.begin(), moi.end(),
      [&](Ogre::SceneManager::MovableObjectIterator::ValueType& v)
          -> void { mStrings.push_back(v.first); });
}
}
