#include "pacStable.h"
#include "OgreArgHandler.h"
#include "OgreConsole.h"
#include <OgreMovableObject.h>
#include <OgreParticleSystem.h>
#include <OgreItem.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreResourceManager.h>
#include <OgreParticleSystemManager.h>
#include <OgreMeshManager.h>
#include <OgreHlmsManager.h>
#include <OgreTextureManager.h>
#include <Animation/OgreSkeletonInstance.h>

namespace pac {

//------------------------------------------------------------------------------
MovableAH::MovableAH(const std::string& name /*= "movable"*/,
    const std::string& moType /*= ""*/, bool attachedOnly /*= false*/)
    : ArgHandler(name),
      mAttachedOnly(attachedOnly),
      mItem(0),
      mSceneNode(0),
      mItemNode(0),
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
  } else if (mItem) {
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
    auto mo = OgreUtil::getMovableByIdtypeNoThrow(
        sm, OgreUtil::getIdFromNameid(s), mMoType);
    return mo && (!mAttachedOnly || mo->isAttached());
  } else if (mItem) {
    PAC_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "no tagpoint in 2.0");
    // entity [bone] moType movable
    // auto oi = mItem->getAttachedObjectIterator();
    // while (oi.hasMoreElements()) {
    // auto mo = oi.getNext();
    // if (mBone.empty() ||
    // mBone == mo->getParentNode()->getParent()->getName()) {
    // if (mo->getName() == s && mo->getMovableType() == mMoType) return true;
    //}
    //}
    // return false;
  } else {
    auto mo = OgreUtil::getMovableByIdtypeNoThrow(
        sm, OgreUtil::getIdFromNameid(s), mMoType);
    return mo && (!mAttachedOnly || mo->isAttached());
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
  mItem = 0;
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
  } else if (mItemNode) {
    // mItem = sm->getEntity(mItemNode->getValue());
    // if (mBoneNode) mBone = mBoneNode->getValue();
  }
}

//------------------------------------------------------------------------------
void MovableAH::onLinked(Node* grandNode) {
  mSnNode = mNode->getAncestorNode("parentSceneNode", 2);
  mItemNode = mNode->getAncestorNode("parentItem", 3);
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
    mRoot->acn("parentSceneNode", "sceneNode")->acn(moAhName)->eb("1");
  } else {
    // moType movable
    mRoot->acn("moType")->acn(moAhName)->eb("0");
    // t_sceneNode moType movable
    mRoot->acn("parentSceneNode", "sceneNode")
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
BoneAH::BoneAH() : ArgHandler("bone"), mItem(0), mItemNode(0) {}

//------------------------------------------------------------------------------
void BoneAH::populatePromptBuffer(const std::string& s) {
  if (mItem->hasSkeleton()) {
    Ogre::SkeletonInstance* sk = mItem->getSkeletonInstance();
    for (size_t i = 0; i < sk->getNumBones(); ++i) {
      const std::string& name = sk->getBone(i)->getName();
      if (s.empty() || StringUtil::startsWith(name, s)) {
        appendPromptBuffer(name);
      }
    }
  }
}

//------------------------------------------------------------------------------
bool BoneAH::doValidate(const std::string& s) {
  if (mItem->hasSkeleton()) {
    return mItem->getSkeletonInstance()->getBone(s);
  }
  return false;
}

//------------------------------------------------------------------------------
void BoneAH::runtimeInit() {
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();
  mItem = static_cast<Ogre::Item*>(OgreUtil::getMovableByIdtype(
      sm, mItemNode->getArgHandler()->getUniformValue()));
}

//------------------------------------------------------------------------------
void BoneAH::onLinked(Node* grandNode) {
  mItemNode = mNode->getAncestorNode("item", 1);
  if (!mItemNode) {
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "bone must follow item");
  }
}

//------------------------------------------------------------------------------
SceneNodeAH::SceneNodeAH(
    const std::string& name /*= "sceneNode"*/, bool includeRoot /*= true*/)
    : ArgHandler(name),
      mIncludeRoot(includeRoot),
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
    auto oi = mAncestorSceneNode->getChildIterator();
    while (oi.hasMoreElements()) loopNode(oi.getNext(), s);
  } else {
    Ogre::SceneNode* dynamicRoot = sm->getRootSceneNode(Ogre::SCENE_DYNAMIC);
    Ogre::SceneNode* staticRoot = sm->getRootSceneNode(Ogre::SCENE_STATIC);
    if (mIncludeRoot) {
      loopNode(dynamicRoot, s);
      loopNode(staticRoot, s);
    } else {
      auto doi = dynamicRoot->getChildIterator();
      while (doi.hasMoreElements()) loopNode(doi.getNext(), s);
      auto soi = staticRoot->getChildIterator();
      while (soi.hasMoreElements()) loopNode(soi.getNext(), s);
    }
  }
}

//------------------------------------------------------------------------------
bool SceneNodeAH::doValidate(const std::string& s) {
  Ogre::SceneManager* sm = sgOgreConsole.getSceneMgr();
  auto node =
      OgreUtil::getSceneNodeByIdNoThrow(sm, OgreUtil::getIdFromNameid(s));
  if (mSmmt >= 0 && OgreUtil::getSceneType(node) != mSmmt) return false;
  if (mIncludeRoot)
    return node;
  else
    return node && node->getId() != 0 && node->getId() != 1;
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
  //  ltl_childOfNode parentSceneNode sceneNode ("2")
  Node* parentSnNode =
      mRoot->acn("ltl_childOfNode")->acn("parentSceneNode", "sceneNode");
  parentSnNode->acn("sceneNode")->eb("2");
  //  ltl_childOfNode parentSceneNode en_smmt sceneNode ("3")
  parentSnNode->acn("en_smmt")->acn("sceneNode")->eb("3");
  //  ltl_descendantOfNode ancestorSceneNode sceneNode ("4")
  Node* ancestorSnNode =
      mRoot->acn("ltl_descendantOfNode")->acn("ancestorSceneNode", "sceneNode");
  ancestorSnNode->acn("sceneNode")->eb("4");
  //  ltl_descendantOfNode ancestorSceneNode en_smmt sceneNode ("5")
  ancestorSnNode->acn("en_smmt")->acn("sceneNode")->eb("5");
  //  ltl_parentOfNode childNode ("6")
  mRoot->acn("ltl_parentOfNode")->acn("childNode")->eb("ps0");
  //  ltl_parentOfMovalbe childMovable ("7")
  mRoot->acn("ltl_parentOfMovable")
      ->acn("moType")
      ->acn("childMovable")
      ->eb("pm0");
}

//------------------------------------------------------------------------------
Ogre::SceneNode* SceneNodeTH::getSceneNode() {
  return OgreUtil::getSceneNodeById(
      sgOgreConsole.getSceneMgr(), getUniformValue());
}

//------------------------------------------------------------------------------
std::string SceneNodeTH::getUniformValue() const {
  Ogre::SceneManager* sceneMgr = sgOgreConsole.getSceneMgr();
  const std::string& branch = getMatchedBranch();
  if (branch == "ps0") {
    Ogre::SceneNode* node = OgreUtil::getSceneNodeById(
        sceneMgr, getMatchedNodeUniformValue("childNode"));
    if (node->getParent())
      return Ogre::StringConverter::toString(node->getParent()->getId());
    else
      PAC_EXCEPT(Exception::ERR_INVALID_STATE, "0 parent sceneNode");
  } else if (branch == "pm0") {
    Ogre::MovableObject* mo = OgreUtil::getMovableByIdtype(sceneMgr,
        OgreUtil::getIdFromNameid(getMatchedNodeValue("childMovable")),
        getMatchedNodeValue("moType"));
    return Ogre::StringConverter::toString(mo->getParentNode()->getId());
  } else
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

//------------------------------------------------------------------------------
DatablockAH::DatablockAH(Ogre::HlmsManager* mgr) : ArgHandler("datablock") {}

//------------------------------------------------------------------------------
void DatablockAH::populatePromptBuffer(const std::string& s) {
#ifndef NDEBUG
  auto blockMap = mManager->getDatablocks();
  std::for_each(blockMap.begin(), blockMap.end(),
      [&](std::map<const Ogre::IdString, Ogre::HlmsDatablock*>::value_type& v)
          -> void {
            const std::string&& block = v.first.getFriendlyText();
            if (s.empty() || StringUtil::startsWith(block, s))
              appendPromptBuffer(block);
          });
  const std::string&& block =
      mManager->getDefaultDatablock()->getName().getFriendlyText();
  if (s.empty() || StringUtil::startsWith(block, s)) appendPromptBuffer(block);
#endif
}

//------------------------------------------------------------------------------
bool DatablockAH::doValidate(const std::string& s) {
#ifndef NDEBUG
  auto datablock = mManager->getDatablock(s);
  return datablock->getName().getFriendlyText() == s;
#endif
  return false;
}

//------------------------------------------------------------------------------
PositionTH::PositionTH() : TreeArgHandler("position") {
  // real3
  mRoot->acn("real3")->eb("0");
  // ltl_posOfNode t_sceneNode ("1")
  mRoot->acn("ltl_posOfNode")->acn("t_sceneNode")->eb("1");
}

//------------------------------------------------------------------------------
std::string PositionTH::getUniformValue() const {
  const std::string& branch = getMatchedBranch();
  if (branch == "0") {
    // real3
    return getMatchedNodeValue("real3");
  } else {
    // posOfNode t_sceneNode ("1")
    Ogre::SceneManager* sceneMgr = sgOgreConsole.getSceneMgr();
    Ogre::SceneNode* node = OgreUtil::getSceneNodeById(
        sceneMgr, getMatchedNodeUniformValue("t_sceneNode"));
    return Ogre::StringConverter::toString(node->_getDerivedPositionUpdated());
  }
}

//------------------------------------------------------------------------------
DirectionTH::DirectionTH() : TreeArgHandler("direction") {
  // real3
  mRoot->acn("real3")->eb("0");
  // ltl_posOfNode t_sceneNode ("1")
  mRoot->acn("ltl_dirOfNode")->acn("t_sceneNode")->eb("1");
}

//------------------------------------------------------------------------------
std::string DirectionTH::getUniformValue() const {
  const std::string& branch = getMatchedBranch();
  if (branch == "0") {
    // real3
    return getMatchedNodeValue("real3");
  } else {
    // ltl_dirOfNode t_sceneNode ("1")
    Ogre::SceneManager* sceneMgr = sgOgreConsole.getSceneMgr();
    Ogre::SceneNode* node = OgreUtil::getSceneNodeById(
        sceneMgr, getMatchedNodeUniformValue("t_sceneNode"));
    Ogre::Vector3 v[3];
    node->_getDerivedOrientationUpdated().ToAxes(v);
    return Ogre::StringConverter::toString(-v[2]);
  }
}
}
