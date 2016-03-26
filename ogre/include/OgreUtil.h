#ifndef OGREUTIL_H
#define OGREUTIL_H

#include "pacConsolePreRequisite.h"
#include "Ogre.h"
namespace pac {

class OgreUtil {
public:
  static Ogre::IdType nid;
  static Ogre::IdType getIdFromNameid(const std::string& nameid);
  static std::string createNameid(const std::string& name, Ogre::IdType id);
  /**
   * destroy scenenode, all it's children, and all the movable object attached
   * to them
   * @param node : target scenenode
   */
  static void destroySceneNodeTotally(Ogre::SceneNode* node);

private:
  OgreUtil(){};
};
}

#endif /* OGREUTIL_H */
