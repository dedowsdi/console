#include "pacStable.h"
#include "OgreUtil.h"
#include <limits>

namespace pac {

Ogre::IdType OgreUtil::nid = std::numeric_limits<Ogre::IdType>::max();

//------------------------------------------------------------------------------
Ogre::IdType OgreUtil::getIdFromNameid(const std::string& nameid) {
  size_t index = nameid.find_last_of('@');
  if (index == std::string::npos || index == nameid.size() - 1)
    return nid;
  else
    return Ogre::StringConverter::parseUnsignedInt(nameid.substr(index + 1));
}

//------------------------------------------------------------------------------
std::string OgreUtil::createNameid(const std::string& name, Ogre::IdType id) {
  return name + "@" + Ogre::StringConverter::toString(id);
}

//------------------------------------------------------------------------------
void OgreUtil::destroySceneNodeTotally(Ogre::SceneNode* node) {
  Ogre::SceneManager* sceneMgr = node->getCreator();
  auto oi = node->getChildIterator();
  // destroy children, 2.0 use vec instead of map, must do this on a copy
  Ogre::Node::NodeVec nv(oi.begin(), oi.end());
  std::for_each(nv.begin(), nv.end(), [&](Ogre::Node* childNode) -> void {
    destroySceneNodeTotally(static_cast<Ogre::SceneNode*>(childNode));
  });
  // destroy attaced movabled objects
  auto moi = node->getAttachedObjectIterator();
  Ogre::SceneNode::ObjectVec ov(moi.begin(), moi.end());
  std::for_each(ov.begin(), ov.end(), [&](Ogre::MovableObject* mo) -> void {
    if (mo->getMovableType() == "Camera")
      // camera is special
      sceneMgr->destroyCamera(static_cast<Ogre::Camera*>(mo));
    else
      sceneMgr->destroyMovableObject(mo);

  });
  sceneMgr->destroySceneNode(node);
}
}
