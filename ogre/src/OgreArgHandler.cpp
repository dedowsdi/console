#include "pacStable.h"
#include "OgreArgHandler.h"
#include "OgreConsole.h"
#include "Ogre.h"

namespace pac {

//------------------------------------------------------------------------------
MovableAH::MovableAH(
    const std::string& name /*= "movable"*/, const std::string& moType /*= ""*/)
    : ArgHandler(name),
      mEntity(0),
      mSceneNode(0),
      mEntityNode(0),
      mBoneNode(0),
      mSnNode(0),
      mMoTypeNode(0),
      mMoType(moType) {
  mFixType = !mMoType.empty();
}

//------------------------------------------------------------------------------
void MovableAH::populatePromptBuffer(const std::string& s) {
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();

  if (mSceneNode) {
    // sceneNode moType movable
    auto oi = mSceneNode->getAttachedObjectIterator();
    populateFromOgreIterator(oi, s);
  } else if (mEntity) {
    PAC_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "no tagpoint in 2.0");
    // entity [bone] moType movable
    // auto oi = mEntity->getAttachedObjectIterator();
    // while (oi.hasMoreElements()) {
    // auto mo = oi.getNext();
    // if (mBone.empty() ||
    // mBone == mo->getParentNode()->getParent()->getName()) {
    // if ((s.empty() || StringUtil::startsWith(mo->getName(), s)) &&
    // mo->getMovableType() == mMoType)
    // appendPromptBuffer(mo->getName());
    //}
    //}
  } else {
    // moType movable
    if (mMoType != "Camera")
      populateFromOgreIterator(sm->getMovableObjectIterator(mMoType), s);
    else
      populateFromOgreIterator(sm->getCameraIterator(), s);
  }
}

//------------------------------------------------------------------------------
bool MovableAH::doValidate(const std::string& s) {
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();
  if (mSceneNode) {
    // sceneNode moType movable
    return OgreUtil::getMovableByIdtypeNoThrow(
        sm, OgreUtil::getIdFromNameid(s), mMoType);
  } else if (mEntity) {
    PAC_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "no tagpoint in 2.0");
    // entity [bone] moType movable
    // auto oi = mEntity->getAttachedObjectIterator();
    // while (oi.hasMoreElements()) {
    // auto mo = oi.getNext();
    // if (mBone.empty() ||
    // mBone == mo->getParentNode()->getParent()->getName()) {
    // if (mo->getName() == s && mo->getMovableType() == mMoType) return true;
    //}
    //}
    // return false;
  } else {
    return OgreUtil::getMovableByIdtypeNoThrow(
        sm, OgreUtil::getIdFromNameid(s), mMoType);
  }
}

//------------------------------------------------------------------------------
std::string MovableAH::getUniformValue() const {
  return Ogre::StringConverter::toString(OgreUtil::getIdFromNameid(mValue)) +
         "@" + mMoType;
}

//------------------------------------------------------------------------------
void MovableAH::runtimeInit() {
  mBone.clear();
  mEntity = 0;
  mSceneNode = 0;
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();
  if (!mFixType) mMoType = mMoTypeNode->getValue();

  if (mMoType.empty()) {
    PAC_EXCEPT(Exception::ERR_INVALID_STATE,
        "empty movable type is not allowed at movable arg handler");
  }
  if (mSnNode) {
    mSceneNode = OgreUtil::getSceneNodeById(
        sm, mSnNode->getArgHandler()->getUniformValue());
  } else if (mEntityNode) {
    // mEntity = sm->getEntity(mEntityNode->getValue());
    // if (mBoneNode) mBone = mBoneNode->getValue();
  }
}

//------------------------------------------------------------------------------
void MovableAH::onLinked(Node* grandNode) {
  mSnNode = mNode->getAncestorNode("parentSceneNode", 2);
  mEntityNode = mNode->getAncestorNode("parentEntity", 3);
  mBoneNode = mNode->getAncestorNode("parentBone", 2);
  if (!mFixType) {
    if (grandNode->isRoot() || grandNode->getAhName() != "moType")
      PAC_EXCEPT(
          Exception::ERR_INVALID_STATE, "plain movable must follow moType!!!");
    mMoTypeNode = grandNode;
  }
}

//------------------------------------------------------------------------------
MovableTH::MovableTH(const std::string& name, const std::string& moAhName)
    : TreeArgHandler(name), mMoAhName(moAhName) {
  bool fixMoType = moAhName != "movable";
  if (fixMoType) {
    mRoot->acn(moAhName)->eb("0");
    mRoot->acn("parentSceneNode", "t_sceneNode")->acn(moAhName)->eb("1");
  } else {
    // moType movable
    mRoot->acn("moType")->acn(moAhName)->eb("0");
    // t_sceneNode moType movable
    mRoot->acn("parentSceneNode", "t_sceneNode")
        ->acn("moType")
        ->acn(moAhName)
        ->eb("1");
    // entity moType movable
    // entity bone moType movable
  }
}

//------------------------------------------------------------------------------
Ogre::MovableObject* MovableTH::getMovableObject() const {
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();
  return OgreUtil::getMovableByIdtype(sm, getUniformValue());
}

//------------------------------------------------------------------------------
std::string MovableTH::getUniformValue() const {
  return getMatchedNodeHandler(mMoAhName)->getUniformValue();
}

//------------------------------------------------------------------------------
BoneAH::BoneAH() : ArgHandler("bone"), mEntity(0), mEntityNode(0) {}

//------------------------------------------------------------------------------
void BoneAH::populatePromptBuffer(const std::string& s) {
  // if (mEntity->hasSkeleton()) {
  // Ogre::SkeletonInstance* sk = mEntity->getSkeleton();
  // auto oi = sk->getBoneIterator();
  // while (oi.hasMoreElements()) {
  // const std::string& name = oi.getNext()->getName();
  // if (s.empty() || StringUtil::startsWith(name, s))
  // appendPromptBuffer(name);
  //}
  //}
}

//------------------------------------------------------------------------------
bool BoneAH::doValidate(const std::string& s) {
  // if (mEntity->hasSkeleton()) {
  // Ogre::SkeletonInstance* sk = mEntity->getSkeleton();
  // return sk->hasBone(s);
  //}

  return false;
}

//------------------------------------------------------------------------------
void BoneAH::runtimeInit() {
  // Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();
  // mEntity = sm->getEntity(mEntityNode->getValue());
}

//------------------------------------------------------------------------------
void BoneAH::onLinked(Node* grandNode) {
  if (grandNode->isRoot() || grandNode->getAhName() != "entity")
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "bone must follow entity");
  mEntityNode = grandNode;
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
  return Ogre::StringConverter::toString(OgreUtil::getIdFromNameid(mValue));
}

//------------------------------------------------------------------------------
Ogre::SceneNode* SceneNodeAH::getSceneNode() {
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();
  return OgreUtil::getSceneNodeById(sm, getUniformValue());
}

//------------------------------------------------------------------------------
void SceneNodeAH::populatePromptBuffer(const std::string& s) {
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();
  if (mParentSceneNode) {
    // output direct child only
    RaiiConsoleBuffer b;
    auto oi = mParentSceneNode->getChildIterator();
    while (oi.hasMoreElements()) {
      Ogre::Node* child = oi.getNext();
      if ((mSmmt < 0 || mSmmt == OgreUtil::getSceneType(child)) &&
          (s.empty() || StringUtil::startsWith(child->getName(), s)))
        appendPromptBuffer(s);
    }
  } else if (mAncestorSceneNode) {
    // output all children
    auto oi = mParentSceneNode->getChildIterator();
    while (oi.hasMoreElements()) loopNode(oi.getNext(), s);
  } else {
    loopNode(sm->getRootSceneNode(Ogre::SCENE_DYNAMIC), s);
    loopNode(sm->getRootSceneNode(Ogre::SCENE_STATIC), s);
  }
}

//------------------------------------------------------------------------------
bool SceneNodeAH::doValidate(const std::string& s) {
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();
  return OgreUtil::getSceneNodeByIdNoThrow(sm, OgreUtil::getIdFromNameid(s));
}

//------------------------------------------------------------------------------
void SceneNodeAH::loopNode(Ogre::Node* node, const std::string& s) {
  const std::string&& nameid = OgreUtil::createNameid(node);
  if ((mSmmt < 0 || mSmmt == OgreUtil::getSceneType(node)) &&
      (s.empty() || StringUtil::startsWith(nameid, s)))
    appendPromptBuffer(nameid);
  auto oi = node->getChildIterator();
  while (oi.hasMoreElements()) loopNode(oi.getNext(), s);
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
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();

  if (mParentSnNode)
    mParentSceneNode = OgreUtil::getSceneNodeById(
        sm, mParentSnNode->getArgHandler()->getUniformValue());

  if (mAncestorSnNode)
    mAncestorSceneNode = OgreUtil::getSceneNodeById(
        sm, mAncestorSnNode->getArgHandler()->getUniformValue());

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
      mRoot->acn("ltl_parent")->acn("parentSceneNode", "sceneNode");
  parentSnNode->acn("sceneNode")->eb("2");
  //  ltl_parent parentSceneNode en_smmt sceneNode ("3")
  parentSnNode->acn("en_smmt")->acn("sceneNode")->eb("3");
  //  ltl_ancestor ancestorSceneNode sceneNode ("4")
  Node* ancestorSnNode =
      mRoot->acn("ltl_ancestor")->acn("ancestorSceneNode", "sceneNode");
  ancestorSnNode->acn("sceneNode")->eb("4");
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
  return getMatchedNodeHandler("sceneNode")->getUniformValue();
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
