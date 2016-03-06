#include "pacStable.h"
#include "OgreArgHandler.h"
#include "OgreConsole.h"
#include "Ogre.h"

namespace pac {

//------------------------------------------------------------------------------
MovableAH::MovableAH() : ArgHandler("movable") {}

//------------------------------------------------------------------------------
void MovableAH::populatePromptBuffer(const std::string& s) {
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();

  if (mSceneNode) {
    // sceneNode moType movable
    auto oi = moType->getMovableObjectIterator();
    while (oi.hasMoreElements()) {
      auto mo = oi.getNext();
      if (StringUtil::startsWith(mo->getName(), s) &&
          mo->getMoveType() == mMoType)
        appendPromptBuffer(s);
    }
  } else if (mEntity) {
    // entity [bone] moType movable
    auto oi = mEntity->getAttachedObjectIterator();
    while (oi.hasMoreElements()) {
      auto mo = oi.getNext();
      if (mBone.empty() ||
          mBone == mo->GetParentNode()->getParent()->getName()) {
        if (StringUtil::startsWith(mo->getName(), s) &&
            mo->getMoveType() == mMoType)
          appendPromptBuffer(s);
      }
    }
  } else {
    // moType movable
    auto oi = sm->getMovableObjectIterator();
    while (oi.hasMoreElements()) {
      auto mo = oi.getNext();
      if (StringUtil::startsWith(mo->getName(), s) &&
          mo->getMoveType() == mMoType)
        appendPromptBuffer(s);
    }
  }
}

//------------------------------------------------------------------------------
bool MovableAH::doValidate(const std::string& s) {
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();
  if (mSceneNode) {
    // sceneNode moType movable
    auto oi = moType->getMovableObjectIterator();
    while (oi.hasMoreElements()) {
      auto mo = oi.getNext();
      if (mo->getName() && mo->getMoveType() == mMoType) return true;
    }
    return false;

  } else if (mEntity) {
    // entity [bone] moType movable
    auto oi = mEntity->getAttachedObjectIterator();
    while (oi.hasMoreElements()) {
      auto mo = oi.getNext();
      if (mBone.empty() ||
          mBone == mo->GetParentNode()->getParent()->getName()) {
        if (mo->getName() == s && mo->getMoveType() == mMoType) return true;
      }
    }
    return false;
  } else {
    // moType movable
    try {
      sm->getMovableObject(s, mMoType);
      return true;
    } catch (Ogre::ItemIdentityException e) {
      return false;
    }
  }
}

//------------------------------------------------------------------------------
void MovableAH::runtimeInit() {
  mMoType.clear();
  mBone.clear();
  mEntity = 0;
  mSceneNode = 0;
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();
  mMoType = mMoTypeNode->getValue();
  if (mSnNode) {
    mSceneNode = sm->getSceneNode(mSnNode->getValue());
  } else if (mEntityNode) {
    mEntity = sm->getEntity(mEntityNode->getName());
    if (mBoneNode) mBone = mBoneNode->getValue();
  }
}

//------------------------------------------------------------------------------
void MovableAH::onLinked(Node* grandNode) {
  if (grandNode->isRoot() || grandNode()->getAhName() != "moType")
    PAC_EXCEPT(
        Exception::ERR_INVALID_STATE, "plain movable must follow moType!!!");

  mMoTypeNode = grandNode;
  Node* node = mMoTypeNode->getParentNode();
  if (!node->isRoot()) {
    const std::string& ahName = node->getAhName();
    if (ahName == "bone") {
      // entity bone moType
      mBoneNode = node;
      mEntityNode = mBoneNode->getParent();
      if (mEntityNode->isRoot() || mEntityNode->getAhName() != "entity") {
        PAC_EXCEPT(Exception::ERR_INVALID_STATE, "bone must follow entity");
      }
    } else if (ahName == "entity") {
      // entity moType
      mEntityNode = node;
    } else if (ahName == "sceneNode") {
      // sceneNode moType
      mSnNode = node;
    }
  }
}

//------------------------------------------------------------------------------
BoneAH::BoneAH() : ArgHandler("bone"), mEntity(0), mEntityNode(0) {}

//------------------------------------------------------------------------------
void BoneAH::populatePromptBuffer(const std::string& s) {
  if (mEntity->hasSkeleton()) {
    Ogre::SkeletonInstance* sk = getEntity->getSkeleton();
    auto oi = sk->getBoneIterator();
    while (oi->hasMoreElements()) {
      const std::string& name = oi->getNext()->getName();
      if (StringUtil::startsWith(name, s)) appendPromptBuffer(name);
    }
  }
}

//------------------------------------------------------------------------------
bool BoneAH::doValidate(const std::string& s) {
  if (mEntity->hasSkeleton()) {
    Ogre::SkeletonInstance* sk = getEntity->getSkeleton();
    return sk->hasBone(s);
  }

  return false;
}

//------------------------------------------------------------------------------
void BoneAH::runtimeInit() {
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();
  mEntity = sm->getEntity(mEntityNode->getValue());
}

//------------------------------------------------------------------------------
void BoneAH::onLinked(Node* grandNode) {
  if (grandNode->isRoot() || grandNode->getAhName() != "entity")
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "bone must follow entity");
  mEntityNode = grandNode;
}

//------------------------------------------------------------------------------
MovableBaseAH::MovableBaseAH(const std::string& name, const std::string& moType)
    : ArgHandler(name), mMoType(moType) {}

//------------------------------------------------------------------------------
void MovableBaseAH::populatePromptBuffer(const std::string& s) {
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();
  auto oi = sm->getMovableObjectIterator(mMoType);
  while (oi.hasMoreElements()) {
    auto mo = oi.getNext();
    if (StringUtil::startsWith(mo->getName(), s) &&
        mo->getMoveType() == mMoType)
      appendPromptBuffer(s);
  }
}

//------------------------------------------------------------------------------
bool MovableBaseAH::doValidate(const std::string& s) {
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();
  try {
    sm->getMovableObject(s, mMoType);
    return true;
  } catch (Ogre::ItemIdentityException e) {
    return false;
  }
}

//------------------------------------------------------------------------------
SceneNodeAH::SceneNodeAH() : ArgHandler("sceneNode") {}

//------------------------------------------------------------------------------
void SceneNodeAH::populatePromptBuffer(const std::string& s) {
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();
  loopNode(sm->getRootSceneNode());
}

//------------------------------------------------------------------------------
bool SceneNodeAH::doValidate(const std::string& s) {
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();
  try {
    sm->getSceneNode(s);
    return true;
  } catch (Ogre::ItemIdentityException e) {
    return false;
  }
}

//------------------------------------------------------------------------------
void SceneNodeAH::loopNode(Ogre::SceneNode* sceneNode, const std::string& s) {
  if (StringUtil::startsWith(sceneNode->getName(), s))
    appendPromptBuffer(sceneNode->getName());
  auto oi = sceneNode->getChildIterator();
  while (oi->hasMoreElements()) loopNode(oi->getNext(), s);
}

//------------------------------------------------------------------------------
ResourceAh::ResourceAh(const std::string& name, Ogre::ResourceManager* rm)
    : ArgHandler(name), mResourceMgr(rm) {}

//------------------------------------------------------------------------------
void ResourceAh::populatePromptBuffer(const std::string& s) {
  auto oi = mResourceMgr->getResourceIterator();
  while (oi->hasMoreElements()) {
    Ogre::ResourcePtr ptr = oi->getNext();
    if (StringUtil::startsWith(ptr->getName(), s)) {
      appendPromptBuffer(ptr->getName());
    }
  }
}

//------------------------------------------------------------------------------
bool ResourceAh::doValidate(const std::string& s) {
  Ogre::ResourcePtr ptr = mResourceMgr->getResourceByName(s);
  return ptr.isEmpty();
}
}
