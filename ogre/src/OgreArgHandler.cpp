#include "pacStable.h"
#include "OgreArgHandler.h"
#include "OgreConsole.h"
#include "Ogre.h"

namespace pac {

//------------------------------------------------------------------------------
MovableAH::MovableAH()
    : ArgHandler("movable"),
      mEntity(0),
      mSceneNode(0),
      mEntityNode(0),
      mBoneNode(0),
      mSnNode(0),
      mMoTypeNode(0) {}

//------------------------------------------------------------------------------
void MovableAH::populatePromptBuffer(const std::string& s) {
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();

  if (mSceneNode) {
    // sceneNode moType movable
    auto oi = mSceneNode->getAttachedObjectIterator();
    while (oi.hasMoreElements()) {
      auto mo = oi.getNext();
      if ((s.empty() || StringUtil::startsWith(mo->getName(), s)) &&
          mo->getMovableType() == mMoType)
        appendPromptBuffer(mo->getName());
    }
  } else if (mEntity) {
    // entity [bone] moType movable
    auto oi = mEntity->getAttachedObjectIterator();
    while (oi.hasMoreElements()) {
      auto mo = oi.getNext();
      if (mBone.empty() ||
          mBone == mo->getParentNode()->getParent()->getName()) {
        if ((s.empty() || StringUtil::startsWith(mo->getName(), s)) &&
            mo->getMovableType() == mMoType)
          appendPromptBuffer(mo->getName());
      }
    }
  } else {
    // moType movable
    auto oi = sm->getMovableObjectIterator(mMoType);
    while (oi.hasMoreElements()) {
      auto mo = oi.getNext();
      if ((s.empty() || StringUtil::startsWith(mo->getName(), s)) &&
          mo->getMovableType() == mMoType)
        appendPromptBuffer(mo->getName());
    }
  }
}

//------------------------------------------------------------------------------
bool MovableAH::doValidate(const std::string& s) {
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();
  if (mSceneNode) {
    // sceneNode moType movable
    auto oi = mSceneNode->getAttachedObjectIterator();
    while (oi.hasMoreElements()) {
      auto mo = oi.getNext();
      if (mo->getName() == s && mo->getMovableType() == mMoType) return true;
    }
    return false;

  } else if (mEntity) {
    // entity [bone] moType movable
    auto oi = mEntity->getAttachedObjectIterator();
    while (oi.hasMoreElements()) {
      auto mo = oi.getNext();
      if (mBone.empty() ||
          mBone == mo->getParentNode()->getParent()->getName()) {
        if (mo->getName() == s && mo->getMovableType() == mMoType) return true;
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
std::string MovableAH::getUniformValue() const {
  //@TODO implement
  throw new std::runtime_error("unimplemented function called");
}

//------------------------------------------------------------------------------
void MovableAH::runtimeInit() {
  mMoType.clear();
  mBone.clear();
  mEntity = 0;
  mSceneNode = 0;
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();
  mMoType = mMoTypeNode->getValue();

  if (mMoType.empty()) {
    PAC_EXCEPT(Exception::ERR_INVALID_STATE,
        "empty movable type is not allowed at movable arg handler");
  }
  if (mSnNode) {
    mSceneNode = sm->getSceneNode(mSnNode->getValue());
  } else if (mEntityNode) {
    mEntity = sm->getEntity(mEntityNode->getValue());
    if (mBoneNode) mBone = mBoneNode->getValue();
  }
}

//------------------------------------------------------------------------------
void MovableAH::onLinked(Node* grandNode) {
  if (grandNode->isRoot() || grandNode->getAhName() != "moType")
    PAC_EXCEPT(
        Exception::ERR_INVALID_STATE, "plain movable must follow moType!!!");

  mMoTypeNode = grandNode;
  Node* node = mMoTypeNode->getParent();
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
    Ogre::SkeletonInstance* sk = mEntity->getSkeleton();
    auto oi = sk->getBoneIterator();
    while (oi.hasMoreElements()) {
      const std::string& name = oi.getNext()->getName();
      if (s.empty() || StringUtil::startsWith(name, s))
        appendPromptBuffer(name);
    }
  }
}

//------------------------------------------------------------------------------
bool BoneAH::doValidate(const std::string& s) {
  if (mEntity->hasSkeleton()) {
    Ogre::SkeletonInstance* sk = mEntity->getSkeleton();
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
    if ((s.empty() || StringUtil::startsWith(mo->getName(), s)) &&
        mo->getMovableType() == mMoType)
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
SceneNodeAH::SceneNodeAH()
    : ArgHandler("sceneNode"),
      mParentSnNode(0),
      mAncestorSnNode(0),
      mSmmtNode(0),
      mParentSceneNode(0),
      mAncestorSceneNode(0),
      mSmmt(-1) {}

//------------------------------------------------------------------------------
std::string SceneNodeAH::getUniformValue() const {
  return Ogre::StringConverter::toString(getIdFromNameid(mValue));
}

//------------------------------------------------------------------------------
Ogre::SceneNode* SceneNodeAH::getSceneNode() {
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();
  Ogre::IdType id = getIdFromNameid(mValue);
  if (id != nid) {
    // normal node
    Ogre::SceneNode* node = sm->getSceneNode(id);
    if (node) return node;
    // try dynamic root and static root node
    Ogre::SceneNode* dynRoot = sm->getRootSceneNode(Ogre::SCENE_DYNAMIC);
    if (id == dynRoot->getId()) return dynRoot;
    Ogre::SceneNode* staticRoot = sm->getRootSceneNode(Ogre::SCENE_STATIC);
    if (id == staticRoot->getId()) return staticRoot;
  }
  return 0;
}

//------------------------------------------------------------------------------
void SceneNodeAH::onLinked(Node* grandNode) {
  mSmmtNode = mNode->getAncestorNode("en_smmt", 2);
  mParentSnNode = mNode->getAncestorNode("parentSceneNode", 2);
  mAncestorSnNode = mNode->getAncestorNode("ancestorSceneNode", 2);
}

//------------------------------------------------------------------------------
void SceneNodeAH::runtimeInit() {
  mSmmt = -1;
  mParentSceneNode = mAncestorSceneNode = 0;
  if (mParentSnNode)
    mParentSceneNode = static_cast<SceneNodeAH*>(mParentSnNode->getArgHandler())
                           ->getSceneNode();
  if (mAncestorSnNode)
    mAncestorSnNode = static_cast<SceneNodeAH*>(
                          mAncestorSnNode->getArgHandler())->getSceneNode();
  if (mSmmtNode)
    mSmmt = enumFromString<Ogre::SceneMemoryMgrTypes>(mSmmtNode->getValue());
}

//------------------------------------------------------------------------------
SceneNodeTH::SceneNodeTH() : TreeArgHandler("t_sceneNode") {
  //  sceneNode ("0")
  mRoot->acn("sceneNode")->eb("0");
  //  en_smmt sceneNode ("1")
  mRoot->acn("en_smmt")->acn("sceneNode")->eb("1");
  //  ltl_parent parentSceneNode sceneNode ("2")
  Node* parentSnNode =
      mRoot->acn("ltl_parent")->("parentSceneNode", "sceneNode");
  parentSnNode->acn("sceneNode")->en("2");
  //  ltl_parent parentSceneNode en_smmt sceneNode ("3")
  parentSnNode->acn("en_smmt")->acn("sceneNode")->eb("3");
  //  ltl_ancestor ancestorSceneNode sceneNode ("4")
  Node* ancestorSnNode =
      mRoot->acn("ltl_ancestor")->("ancestorSceneNode", "sceneNode");
  ancestorSnNode->acn("sceneNode")->en("4");
  //  ltl_ancestor ancestorSceneNode en_smmt sceneNode ("5")
  ancestorSnNode->acn("en_smmt")->acn("sceneNode")->eb("5");
}

//------------------------------------------------------------------------------
Ogre::SceneNode* SceneNodeTH::getSceneNode() {
  return static_cast<SceneNodeAH*>(getMatchedNodeHandler("sceneNode"))
      ->getSceneNode();
}

//------------------------------------------------------------------------------
std::string SceneNodeTH::getUniformValue() const {
  return static_cast<SceneNodeAH*>(getMatchedNodeHandler("sceneNode"))
      ->getUniformValue();
}

//------------------------------------------------------------------------------
void SceneNodeAH::populatePromptBuffer(const std::string& s) {
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();
  if (mParentSceneNode) {
    // output direct child only
    RaiiConsoleBuffer b;
    auto oi = mParentSceneNode->getChildIterator();
    while (oi.hasMoreElements()) {
      Ogre::Node* child = oi->getNext();
      if ((mSmmt < 0 || mSmmt == child->getMemoryManagerType()) &&
          (s.empty() || StringUtil::startsWith(child->getName(), s)))
        appendPromptBuffer(s);
    }
  } else if (mAncestorSceneNode) {
    // output all children
    auto oi = mParentSceneNode->getChildIterator();
    while (oi.hasMoreElements()) loopNode(oi->getNext(), s);
  } else {
    loopNode(sm->getRootSceneNode(Ogre::SCENE_DYNAMIC), s);
    loopNode(sm->getRootSceneNode(Ogre::SCENE_STATIC), s);
  }
}
}

//------------------------------------------------------------------------------
bool SceneNodeAH::doValidate(const std::string& s) {
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();
  Ogre::IdType id = getIdFromNameid(s);
  if (id != nid) {
    Ogre::SceneNode* node = sm->getSceneNode(id);
    if (!node) {
      // try root
      if (id == sm->getRootSceneNode(Ogre::SCENE_DYNAMIC)->getId() ||
          id == sm->getRootSceneNode(Ogre::SCENE_STATIC)->getId())
        return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
void SceneNodeAH::loopNode(Ogre::Node* node, const std::string& s) {
  if ((mSmmt < 0 || mSmmt == child->getMemoryManagerType()) &&
      (s.empty() || StringUtil::startsWith(child->getName(), s)))
    appendPromptBuffer(createNameid(node->getName(), node->getId()));
  auto oi = node->getChildIterator();
  while (oi.hasMoreElements()) loopNode(oi.getNext(), s);
}

//------------------------------------------------------------------------------
ResourceAH::ResourceAH(const std::string& name, Ogre::ResourceManager* rm)
    : ArgHandler(name), mResourceMgr(rm) {}

//------------------------------------------------------------------------------
void ResourceAH::populatePromptBuffer(const std::string& s) {
  auto oi = mResourceMgr->getResourceIterator();
  while (oi.hasMoreElements()) {
    Ogre::ResourcePtr ptr = oi.getNext();
    if (s.empty() || StringUtil::startsWith(ptr->getName(), s)) {
      appendPromptBuffer(ptr->getName());
    }
  }
}

//------------------------------------------------------------------------------
bool ResourceAH::doValidate(const std::string& s) {
  Ogre::ResourcePtr ptr = mResourceMgr->getResourceByName(s);
  return !ptr.isNull();
}

//------------------------------------------------------------------------------
PassiveResourceAH::PassiveResourceAH(const std::string& name,
    Ogre::ResourceManager* rm, std::initializer_list<std::string> exts)
    : ArgHandler(name), mResourceMgr(rm) {
  // loop every archive of every resource group
  auto grps = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
  std::for_each(grps.begin(), grps.end(), [&](const std::string& grp) -> void {
    const Ogre::ResourceGroupManager::LocationList& resList =
        Ogre::ResourceGroupManager::getSingleton().getResourceLocationList(grp);
    std::for_each(resList.begin(), resList.end(),
        [&](const Ogre::ResourceGroupManager::ResourceLocation* rv) -> void {
          Ogre::StringVectorPtr reses = rv->archive->list(false);
          std::for_each(
              reses->begin(), reses->end(), [&](const std::string& r) -> void {
                // check extension
                size_t index = r.find_last_of(".");
                if (index != std::string::npos) {
                  const std::string&& ext = r.substr(index + 1);
                  if (std::find(exts.begin(), exts.end(), ext) != exts.end()) {
                    mResources.insert(r);
                  }
                }
              });
        });
  });
}

//------------------------------------------------------------------------------
StringSet::const_iterator PassiveResourceAH::beginResourceIter() const {
  return mResources.begin();
}

//------------------------------------------------------------------------------
StringSet::const_iterator PassiveResourceAH::endResourceIter() const {
  return mResources.end();
}

//------------------------------------------------------------------------------
void PassiveResourceAH::populatePromptBuffer(const std::string& s) {
  auto oi = mResourceMgr->getResourceIterator();
  // check items in resource manager
  while (oi.hasMoreElements()) {
    Ogre::ResourcePtr ptr = oi.getNext();
    if (s.empty() || StringUtil::startsWith(ptr->getName(), s)) {
      if (mResources.find(ptr->getName()) == mResources.end()) {
        appendPromptBuffer(ptr->getName());
      }
    }
  }
  // check items in steady resources
  std::for_each(
      mResources.begin(), mResources.end(), [&](const std::string& v) -> void {
        if (s.empty() || StringUtil::startsWith(v, s)) {
          appendPromptBuffer(v);
        }
      });
}

//------------------------------------------------------------------------------
bool PassiveResourceAH::doValidate(const std::string& s) {
  Ogre::ResourcePtr ptr = mResourceMgr->getResourceByName(s);
  return !ptr.isNull() || mResources.find(s) != mResources.end();
}

//------------------------------------------------------------------------------
ParticleSystemTemplateAH::ParticleSystemTemplateAH(
    Ogre::ParticleSystemManager* pm)
    : ArgHandler("pst"), mManager(pm) {}

//------------------------------------------------------------------------------
void ParticleSystemTemplateAH::populatePromptBuffer(const std::string& s) {
  auto oi = mManager->getTemplateIterator();
  while (oi.hasMoreElements()) {
    auto* ps = oi.getNext();

    if (s.empty() || StringUtil::startsWith(ps->getName(), s)) {
      appendPromptBuffer(ps->getName());
    }
  }
}

//------------------------------------------------------------------------------
bool ParticleSystemTemplateAH::doValidate(const std::string& s) {
  return mManager->getTemplate(s);
}
}
