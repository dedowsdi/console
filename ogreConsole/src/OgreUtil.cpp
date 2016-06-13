#include "pacStable.h"
#include "OgreUtil.h"
#include <limits>
#include <OgreSceneManager.h>
#include <OgreCamera.h>

namespace pac {
Ogre::IdType OgreUtil::nid = std::numeric_limits<Ogre::IdType>::max();

//------------------------------------------------------------------------------
Ogre::IdType OgreUtil::getIdFromNameid(const std::string& nameid) {
  size_t index = nameid.find_last_of('@');
  if (index == std::string::npos || index == nameid.size() - 1) return nid;
  return Ogre::StringConverter::parseUnsignedInt(nameid.substr(index + 1), nid);
}

//------------------------------------------------------------------------------
std::string OgreUtil::createNameid(const Ogre::Node* node) {
  return node->getName() + "@" + Ogre::StringConverter::toString(node->getId());
}

//------------------------------------------------------------------------------
std::string OgreUtil::createNameid(const Ogre::MovableObject* mo) {
  return mo->getName() + "@" + Ogre::StringConverter::toString(mo->getId());
}

//------------------------------------------------------------------------------
Ogre::SceneNode* OgreUtil::getSceneNodeById(
    Ogre::SceneManager* mgr, const std::string& id) {
  return getSceneNodeById(
      mgr, Ogre::StringConverter::parseUnsignedInt(id, nid));
}

//------------------------------------------------------------------------------
Ogre::SceneNode* OgreUtil::getSceneNodeById(
    Ogre::SceneManager* mgr, Ogre::IdType id) {
  auto n = getSceneNodeByIdNoThrow(mgr, id);
  if (!n)
    PAC_EXCEPT(Exception::ERR_INVALID_STATE,
        "invalid id " + Ogre::StringConverter::toString(id));
  return n;
}

//------------------------------------------------------------------------------
Ogre::SceneNode* OgreUtil::getSceneNodeByIdNoThrow(
    Ogre::SceneManager* mgr, Ogre::IdType id) {
  if (id != nid) {
    // normal node
    Ogre::SceneNode* node = mgr->getSceneNode(id);
    if (node) return node;
    // try dynamic root and static root node
    Ogre::SceneNode* dynRoot = mgr->getRootSceneNode(Ogre::SCENE_DYNAMIC);
    if (id == dynRoot->getId()) return dynRoot;
    Ogre::SceneNode* staticRoot = mgr->getRootSceneNode(Ogre::SCENE_STATIC);
    if (id == staticRoot->getId()) return staticRoot;
  }
  sgLogger.logMessage(
      "can not find scene node for id:" + Ogre::StringConverter::toString(id),
      SL_TRIVIAL);
  return 0;
}

//------------------------------------------------------------------------------
std::tuple<Ogre::IdType, std::string> OgreUtil::parseIdtype(
    const std::string& it) {
  size_t index = it.find_last_of("@");
  if (index == std::string::npos || index == it.size() - 1)
    PAC_EXCEPT(Exception::ERR_INVALIDPARAMS, "not a valid it :" + it);

  const std::string&& type = it.substr(index + 1);
  Ogre::IdType id =
      Ogre::StringConverter::parseUnsignedInt(it.substr(0, index), nid);
  return std::make_tuple(id, type);
}

//------------------------------------------------------------------------------
std::string OgreUtil::createIdtype(Ogre::MovableObject* mo) {
  std::stringstream ss;
  ss << mo->getId() << "@" << mo->getMovableType();
  return ss.str();
}

//------------------------------------------------------------------------------
Ogre::MovableObject* OgreUtil::getMovableByIdtypeNoThrow(
    Ogre::SceneManager* mgr, Ogre::IdType id, const std::string& type) {
  if (type == "Camera") {
    auto oi = mgr->getCameraIterator();
    while (oi.hasMoreElements()) {
      Ogre::MovableObject* mo = oi.getNext();
      if (mo->getId() == id) return mo;
    }
  } else {
    auto oi = mgr->getMovableObjectIterator(type);
    while (oi.hasMoreElements()) {
      Ogre::MovableObject* mo = oi.getNext();
      if (mo->getId() == id) return mo;
    }
  }
  return 0;
}

//------------------------------------------------------------------------------
Ogre::MovableObject* OgreUtil::getMovableByIdtype(
    Ogre::SceneManager* mgr, const std::string& it) {
  auto tit = parseIdtype(it);
  Ogre::IdType id = std::get<0>(tit);
  const std::string& type = std::get<1>(tit);
  return getMovableByIdtype(mgr, id, type);
}

//------------------------------------------------------------------------------
Ogre::MovableObject* OgreUtil::getMovableByIdtype(
    Ogre::SceneManager* mgr, Ogre::IdType id, const std::string& type) {
  auto mo = getMovableByIdtypeNoThrow(mgr, id, type);
  if (!mo) {
    PAC_EXCEPT(Exception::ERR_INVALIDPARAMS,
        "can not find MovableObject by id:" +
            Ogre::StringConverter::toString(id) + " ,type:" + type);
  }
  return mo;
}

//------------------------------------------------------------------------------
Ogre::SceneMemoryMgrTypes OgreUtil::getSceneType(const Ogre::Node* node) {
  return node->isStatic() ? Ogre::SCENE_STATIC : Ogre::SCENE_DYNAMIC;
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

//------------------------------------------------------------------------------
bool OgreUtil::hasSceneNode(Ogre::SceneManager* mgr, const std::string& name) {
  return hasNode(mgr->getRootSceneNode(Ogre::SCENE_DYNAMIC), name) ||
         hasNode(mgr->getRootSceneNode(Ogre::SCENE_STATIC), name);
}

//------------------------------------------------------------------------------
bool OgreUtil::hasNode(Ogre::Node* node, const std::string& name) {
  if (node->getName() == name) return true;

  auto oi = node->getChildIterator();
  while (oi.hasMoreElements()) {
    auto childNode = oi.getNext();
    if (hasNode(childNode, name)) return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool OgreUtil::hasMovableObject(
    Ogre::SceneManager* mgr, const std::string& name, const std::string& type) {
  if (type != "Camera") {
    auto oi = mgr->getMovableObjectIterator(type);
    while (oi.hasMoreElements()) {
      auto mo = oi.getNext();
      if (mo->getName() == name) return true;
    }
  } else {
    auto oi = mgr->getCameraIterator();
    while (oi.hasMoreElements()) {
      auto mo = oi.getNext();
      if (mo->getName() == name) return true;
    }
  }
  return false;
}
}
