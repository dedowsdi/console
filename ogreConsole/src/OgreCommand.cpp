#include "OgreCommand.h"
#include "OgreConsole.h"
#include "pacArgHandler.h"
#include "pacEnumUtil.h"
#include "pacLogger.h"
#include "pacStringUtil.h"
#include "pacAbsDir.h"
#include "OgreArgHandler.h"
#include <OgreRoot.h>
#include <OgreHlmsManager.h>
#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreMeshManager2.h>
#include <OgreTextureManager.h>
#include <OgreParticleSystemManager.h>
#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>
#include <OgreParticleAffector.h>
#include <OgreEntity.h>
#include <Animation/OgreTagPoint.h>
#include <Animation/OgreBone.h>
#include <Animation/OgreSkeletonInstance.h>
#include <OgreCamera.h>
#include <Compositor/OgreCompositorWorkspace.h>
#include <Compositor/OgreCompositorManager2.h>
#include <Compositor/OgreCompositorNode.h>
#include <boost/regex.hpp>


//#include <OgreCompositorManager.h>

typedef Ogre::ResourceManager::ResourceMapIterator ResMgrIter;
typedef Ogre::SceneManager::MovableObjectIterator SceneMgrMoIter;

namespace pac {

template <typename T>
void outputMovable(
    T oi, const std::string& reExp = "", const std::string& moType = "") {
  boost::regex regex(reExp);
  while (oi.hasMoreElements()) {
    Ogre::MovableObject* mo = oi.getNext();
    const std::string&& nameid = OgreUtil::createNameid(mo);
    if ((reExp.empty() || boost::regex_match(nameid, regex)) &&
        (moType.empty() || moType == mo->getMovableType()))
      sgConsole.output(nameid);
  }
}

//------------------------------------------------------------------------------
Ogre::ResourceManager* getResourceManager(const std::string& resType) {
  if (resType == "material") return Ogre::MaterialManager::getSingletonPtr();
  if (resType == "mesh") return Ogre::MeshManager::getSingletonPtr();
  if (resType == "texture") return Ogre::TextureManager::getSingletonPtr();
  // if (resType == "compositor")
  // return Ogre::CompositorManager::getSingletonPtr();
  PAC_EXCEPT(Exception::ERR_INVALIDPARAMS, resType + " is not defined ");
}

//------------------------------------------------------------------------------
void printNodeTree(Ogre::Node* node, size_t level, size_t maxLevel) {
  static std::string prefix = "|-- ";

  std::string blank(level >= 1 ? (level - 1) * prefix.size() : 0, ' ');
  // output extra | if parent bone is not last child of grand bone, do this
  // recursively until level 0
  Ogre::Node* parentNode = node->getParent();
  int grandLevel = level - 2;
  while (grandLevel >= 0 && parentNode && parentNode->getParent()) {
    Ogre::Node* grandNode = parentNode->getParent();
    if (*grandNode->getChildIterator().end() != parentNode) {
      blank[grandLevel * prefix.size()] = '|';
    }
    --grandLevel;
    parentNode = grandNode;
  }

  sgConsole.outputLine(
      blank + (level ? prefix : "") + OgreUtil::createNameid(node));

  if (level >= maxLevel) return;

  Ogre::Node::NodeVecIterator oi = node->getChildIterator();

  while (oi.hasMoreElements()) {
    printNodeTree(oi.getNext(), level + 1, maxLevel);
  }
}

//------------------------------------------------------------------------------
void printBoneTree(Ogre::Bone* bone, size_t level, size_t maxLevel) {
  static std::string prefix = "|-- ";

  std::string blank(level >= 1 ? (level - 1) * prefix.size() : 0, ' ');
  // output extra | if parent bone is not last child of grand bone, do this
  // recursively until level 0
  Ogre::Bone* parentBone = bone->getParent();
  int grandLevel = level - 2;
  while (grandLevel >= 0 && parentBone && parentBone->getParent()) {
    Ogre::Bone* grandBone = parentBone->getParent();
    if (grandBone->getChild(grandBone->getNumChildren() - 1) != parentBone) {
      blank[grandLevel * prefix.size()] = '|';
    }
    --grandLevel;
    parentBone = grandBone;
  }

  sgConsole.outputLine(blank + (level ? prefix : "") + bone->getName());

  if (level >= maxLevel) return;

  for (size_t i = 0; i < bone->getNumChildren(); ++i) {
    printBoneTree(bone->getChild(i), level + 1, maxLevel);
  }
}

//------------------------------------------------------------------------------
LsresCmd::LsresCmd() : Command("lsres") {}

//------------------------------------------------------------------------------
bool LsresCmd::doExecute() {
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  const std::string& branch = handler->getMatchedBranch();
  if (branch != "0" && branch != "1")
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "invalidate branch:" + branch);

  // lsres resType ("0")
  // lsres resType ltl_regex regex ("1")
  const std::string& resType = handler->getMatchedNodeValue("resType");
  const std::string& reExp = handler->getMatchedNodeValue("regex", {"1"});
  boost::regex regex(reExp);
  RaiiConsoleBuffer rcb;

  if (resType != "pst" && resType != "datablock") {
    // normal resource managed by child class or Ogre::ResourceManager
    auto rm = getResourceManager(resType);
    auto oi = rm->getResourceIterator();
    while (oi.hasMoreElements()) {
      Ogre::ResourcePtr ptr = oi.getNext();
      if (reExp.empty() || boost::regex_match(ptr->getName(), regex))
        sgConsole.output(ptr->getName());
    }
    if (resType == "mesh" || resType == "texture") {
      // output fixed item for mesh and texture
      PassiveResourceAH* ah =
          static_cast<PassiveResourceAH*>(sgArgLib.createArgHandler(resType));
      std::for_each(ah->beginResourceIter(), ah->endResourceIter(),
          [&](const std::string& v) -> void {
            if (reExp.empty() || boost::regex_match(v, regex)) {
              // v will also in resource manager if it's loaded
              auto iter = std::find_if(oi.begin(), oi.end(),
                  [&](Ogre::ResourceManager::ResourceHandleMap::value_type p)
                      -> bool { return p.second->getName() == v; });
              if (iter == oi.end()) sgConsole.output(v);
            }
          });
    }
  } else if (resType == "pst") {
    // particle system template
    auto oi =
        Ogre::ParticleSystemManager::getSingletonPtr()->getTemplateIterator();
    while (oi.hasMoreElements()) {
      auto ps = oi.getNext();
      if (reExp.empty() || boost::regex_match(ps->getName(), regex))
        sgConsole.output(ps->getName());
    }
  } else if (resType == "datablock") {
    // datablock
    auto mgr = Ogre::Root::getSingleton().getHlmsManager();
    auto oi = mgr->getDatablocks();
    auto blockMap = mgr->getDatablocks();
    std::for_each(blockMap.begin(), blockMap.end(),
        [&](std::map<const Ogre::IdString, Ogre::HlmsDatablock*>::value_type& v)
            -> void {
              const std::string&& block = v.first.getFriendlyText();
              sgConsole.output(block);
            });
    sgConsole.output(mgr->getDefaultDatablock()->getName().getFriendlyText());
  }

  return true;
}

//------------------------------------------------------------------------------
bool LsresCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  Node* root = handler->getRoot();
  Node* node = root->acn("resType");
  // lsres resType
  node->eb("0");
  // lsres resType ltl_regex regex ("1")
  node->acn("ltl_regex")->acn("regex")->eb("1");
  mArgHandler = handler;
  return true;
}

//------------------------------------------------------------------------------
LsndCmd::LsndCmd() : Command("lsnd") {}

//------------------------------------------------------------------------------
bool LsndCmd::doExecute() {
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  const std::string& branch = handler->getMatchedBranch();
  Ogre::SceneManager* sceneMgr = sgOgreConsole.getSceneMgr();
  Ogre::SceneNode* dynamicRoot =
      sceneMgr->getRootSceneNode(Ogre::SCENE_DYNAMIC);
  Ogre::SceneNode* staticRoot = sceneMgr->getRootSceneNode(Ogre::SCENE_STATIC);

  const std::string& reExp =
      handler->getMatchedNodeValue("regex", {"r0", "r1"});
  const std::string& smmtStr =
      handler->getMatchedNodeValue("en_smmt", {"g1", "s1", "r1"});
  int smmt =
      smmtStr.empty() ? -1 : enumFromString<Ogre::SceneMemoryMgrTypes>(smmtStr);

  RaiiConsoleBuffer rcb;
  if (branch[0] == 'g') {
    // lsnd ("g0")
    // lsnd en_smmt ("g1")
    outputNode(dynamicRoot, smmt);
    outputNode(staticRoot, smmt);
  } else if (branch[0] == 's') {
    // lsnd [-r] sceneNode ("s0")      [-r] recursively list children
    // lsnd [-r] sceneNode en_smmt("s1")
    Ogre::SceneNode* node = OgreUtil::getSceneNodeById(
        sceneMgr, handler->getMatchedNodeUniformValue("sceneNode"));

    auto oi = node->getChildIterator();
    while (oi.hasMoreElements()) {
      auto n = static_cast<Ogre::SceneNode*>(oi.getNext());
      if (hasOption('r'))
        outputNode(n, smmt);
      else if (smmt < 0 || smmt == OgreUtil::getSceneType(n))
        sgConsole.output(OgreUtil::createNameid(n));
    }

  } else if (branch[0] == 'r') {
    // lsnd ltl_regex regex ("r0")
    // lsnd ltl_regex regex en_smmt("r1")
    boost::regex regex(reExp);
    outputNode(dynamicRoot, smmt, regex);
    outputNode(staticRoot, smmt, regex);
  } else if (branch == "ps0") {
    // lsnd ltl_parentOfNode sceneNode ("ps0")
    Ogre::SceneNode* node = OgreUtil::getSceneNodeById(
        sceneMgr, handler->getMatchedNodeUniformValue("sceneNode"));
    if (node->getParent())
      sgConsole.output(OgreUtil::createNameid(node->getParent()));

  } else if (branch == "pm0") {
    // lsnd ltl_parentOfMovable moType movable ("pm0")
    Ogre::MovableObject* mo = OgreUtil::getMovableByIdtype(
        sceneMgr, handler->getMatchedNodeUniformValue("movable"));
    if (mo->isAttached())
      //@TODO check bont attach
      sgConsole.output(OgreUtil::createNameid(mo->getParentNode()));

  } else {
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "invalid branch:" + branch);
  }
  return true;
}

//------------------------------------------------------------------------------
bool LsndCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  Node* root = handler->getRoot();
  // lsnd ("g0")
  root->eb("g0");
  // lsnd en_smmt ("g1")
  root->acn("en_smmt")->eb("g1");
  // lsnd [-r] sceneNode ("s0")      [-r] recursively list children
  Node* node = root->acn("sceneNode");
  node->eb("s0");
  // lsnd [-r] sceneNode en_smmt("s1")
  node->acn("en_smmt")->eb("s1");
  // lsnd ltl_regex regex ("r0")
  node = root->acn("ltl_regex")->acn("regex");
  node->eb("r0");
  // lsnd ltl_regex regex en_smmt("r1")
  node->acn("en_smmt")->eb("r1");
  // lsnd ltl_parentOfNode sceneNode ("ps0")
  root->acn("ltl_parentOfNode")->acn("sceneNode")->eb("ps0");
  // lsnd ltl_parentOfMovable moType movable ("pm0")
  root->acn("ltl_parentOfMovable")->acn("moType")->acn("movable")->eb("pm0");

  mArgHandler = handler;
  return true;
}

//------------------------------------------------------------------------------
void LsndCmd::outputNode(const Ogre::Node* node, int smmt) {
  if (smmt < 0 || OgreUtil::getSceneType(node) == smmt)
    sgConsole.output(OgreUtil::createNameid(node));
  auto oi = node->getChildIterator();
  while (oi.hasMoreElements()) outputNode(oi.getNext(), smmt);
}

//------------------------------------------------------------------------------
void LsndCmd::outputNode(
    const Ogre::Node* node, int smmt, boost::regex& regex) {
  const std::string&& nameid = OgreUtil::createNameid(node);
  if ((smmt < 0 || OgreUtil::getSceneType(node) == smmt) &&
      boost::regex_match(nameid, regex))
    sgConsole.output(nameid);

  auto oi = node->getChildIterator();
  while (oi.hasMoreElements()) outputNode(oi.getNext(), smmt, regex);
}

//------------------------------------------------------------------------------
AthCmd::AthCmd() : Command("ath") {}

//------------------------------------------------------------------------------
bool AthCmd::doExecute() {
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  Ogre::SceneManager* sceneMgr = sgOgreConsole.getSceneMgr();
  const std::string& branch = handler->getMatchedBranch();
  const std::string& id = handler->getMatchedNodeValue("id");
  Ogre::MovableObject* mo;
  // check if id already exists
  std::string moType;
  if (handler->hasMatchedNode("ltl_light"))
    moType = "Light";
  else if (handler->hasMatchedNode("ltl_item"))
    moType = "Item";
  else if (handler->hasMatchedNode("ltl_particle"))
    moType = "ParticleSystem";
  else if (handler->hasMatchedNode("ltl_camera"))
    moType = "Camera";
  else
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "blank motype");

  if (branch == "sn0" || branch == "tag0") {
    // ath ltl_sceneNode t_sceneNode ltl_light id ("sn0")
    // ath ltl_tagPoint entity bone ltl_light id ("tag0")
    mo = sceneMgr->createLight();
  } else if (branch == "sn1" || branch == "tag1") {
    // ath ltl_sceneNode t_sceneNode ltl_item id mesh ("sn1")
    // ath ltl_tagPoint entity bone ltl_item id mesh ("tag1")
    mo = sceneMgr->createItem(handler->getMatchedNodeValue("mesh"));
  } else if (branch == "sn2" || branch == "tag2") {
    // ath ltl_sceneNode t_sceneNode ltl_particle id pst ("sn2")
    // ath ltl_tagPoint entity bone ltl_particle id pst ("tag2")
    mo = sceneMgr->createParticleSystem(handler->getMatchedNodeValue("pst"));
  } else if (branch == "sn3" || branch == "tag3") {
    // ath ltl_sceneNode t_sceneNode ltl_camera id("sn3")
    // ath ltl_tagPoint entity bone ltl_camera id ("tag3")
    if (sceneMgr->findCameraNoThrow(id)) {
      sgConsole.outputLine(id + " is already used for another camera");
      return false;
    }
    mo = sceneMgr->createCamera(id);
    mo->detachFromParent();
  } else {
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "unknown branch:" + branch);
  }

  mo->setName(id);

  if (branch.size() > 0 && branch[0] == 's') {
    Ogre::SceneNode* sceneNode =
        handler->getMatchedNodeHandler<SceneNodeTH>("t_sceneNode")
            ->getSceneNode();
    sceneNode->attachObject(mo);
  } else if (branch.size() > 0 && branch[0] == 't') {
    Ogre::Item* item = static_cast<Ogre::Item*>(OgreUtil::getMovableByIdtype(
        sceneMgr, handler->getMatchedNodeUniformValue("item")));

    auto bone = item->getSkeletonInstance()->getBone(
        handler->getMatchedNodeValue("bone"));
    auto tp = sceneMgr->createTagPoint();
    tp->setName(item->getName() + "_" + bone->getName() + ":" + mo->getName());
    tp->attachObject(mo);
    bone->addTagPoint(tp);

  } else {
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "invalid branch");
  }
  return true;
}

//------------------------------------------------------------------------------
bool AthCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  Node* root = handler->getRoot();
  Node* snNode = root->acn("ltl_sceneNode")->acn("t_sceneNode");
  // ath ltl_sceneNode t_sceneNode ltl_light id ("sn0")
  snNode->acn("ltl_light")->acn("id")->eb("sn0");
  // ath ltl_sceneNode t_sceneNode ltl_item id mesh ("sn1")
  snNode->acn("ltl_item")->acn("id")->acn("mesh")->eb("sn1");
  // ath ltl_sceneNode t_sceneNode ltl_particle id pst ("sn2")
  snNode->acn("ltl_particle")->acn("id")->acn("pst")->eb("sn2");
  // ath ltl_sceneNode t_sceneNode ltl_camera id("sn3")
  snNode->acn("ltl_camera")->acn("id")->eb("sn3");

  Node* itemNode = root->acn("ltl_tagPoint")->acn("item")->acn("bone");
  // ath ltl_tagPoint entity bone ltl_light id ("tag0")
  itemNode->acn("ltl_light")->acn("id")->eb("tag0");
  // ath ltl_tagPoint entity bone ltl_item id mesh ("tag1")
  itemNode->acn("ltl_item")->acn("id")->acn("mesh")->eb("tag1");
  // ath ltl_tagPoint entity bone ltl_particle id pst ("tag2")
  itemNode->acn("ltl_particle")->acn("id")->acn("pst")->eb("tag2");
  // ath ltl_tagPoint entity bone ltl_camera id ("tag3")
  itemNode->acn("ltl_camera")->acn("id")->eb("tag3");

  mArgHandler = handler;
  return true;
}

//------------------------------------------------------------------------------
LsmoCmd::LsmoCmd() : Command("lsmo") {}

//------------------------------------------------------------------------------
bool LsmoCmd::doExecute() {
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  Ogre::SceneManager* sceneMgr = sgOgreConsole.getSceneMgr();
  const std::string& branch = handler->getMatchedBranch();

  RaiiConsoleBuffer rcb;

  if (branch == "g0" || branch == "g1") {
    // lsmo moType ("g0")
    // lsmo moType ltl_regex regex ("g1")
    const std::string& moType = handler->getMatchedNodeValue("moType");
    const std::string& reExp = handler->getMatchedNodeValue("regex", {"g1"});

    if (moType != "Camera") {
      auto oi = sceneMgr->getMovableObjectIterator(moType);
      outputMovable(oi, reExp);
    } else {
      auto oi = sceneMgr->getCameraIterator();
      outputMovable(oi, reExp);
    }
  } else if (branch == "sn0" || branch == "sn1") {
    // lsmo ltl_sceneNode sceneNode ("sn0")
    // lsmo ltl_sceneNode sceneNode moType ("sn1")
    Ogre::SceneNode* sceneNode = OgreUtil::getSceneNodeById(
        sceneMgr, handler->getMatchedNodeUniformValue("sceneNode"));
    const std::string& moType = handler->getMatchedNodeValue("moType", {"sn1"});
    auto oi = sceneNode->getAttachedObjectIterator();
    outputMovable(oi, "", moType);
  } else if (branch == "tag0" || branch == "tag1" || branch == "tag2" ||
             branch == "tag3") {
    // lsmo ltl_tagPoint entity ("tag0")
    // lsmo ltl_tagPoint entity moType ("tag1")
    // lsmo ltl_tagPoint entity bone ("tag2")
    // lsmo ltl_tagPoint entity bone moType ("tag3")
    // Ogre::Entity* ent =
    // sceneMgr->getEntity(handler->getMatchedNodeValue("entity"));
    // const std::string& bone =
    // handler->getMatchedNodeValue("bone", {"tag2", "tag3"});
    // const std::string& moType =
    // handler->getMatchedNodeValue("moType", {"tag1", "tag3"});
    // auto oi = ent->getAttachedObjectIterator();
    // RaiiConsoleBuffer rcb;
    // while (oi.hasMoreElements()) {
    // Ogre::MovableObject* mo = oi.getNext();
    // Ogre::Node* athNode = mo->getParentNode()->getParent();

    // if ((moType.empty() || moType == mo->getMovableType()) &&
    //(bone.empty() || athNode->getName() == bone))
    // sgConsole.output(mo->getName());
    //}
  } else {
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "invalid branch" + branch);
  }

  return true;
}

//------------------------------------------------------------------------------
bool LsmoCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  Node* root = handler->getRoot();
  mArgHandler = handler;

  // lsmo moType ("g0")
  Node* motNode = root->acn("moType");
  motNode->eb("g0");
  // lsmo moType ltl_regex regex ("g1")
  motNode->acn("ltl_regex")->acn("regex")->eb("g1");

  Node* snNode = root->acn("ltl_sceneNode")->acn("sceneNode");
  // lsmo ltl_sceneNode sceneNode ("sn0")
  snNode->eb("sn0");
  // lsmo ltl_sceneNode sceneNode moType ("sn1")
  snNode->acn("moType")->eb("sn1");

  Node* entNode = root->acn("ltl_tagPoint")->acn("item");
  // lsmo ltl_tagPoint entity ("tag0")
  entNode->eb("tag0");
  // lsmo ltl_tagPoint entity moType ("tag1")
  entNode->acn("moType")->eb("tag1");

  Node* boneNode = entNode->acn("bone");
  // lsmo ltl_tagPoint entity bone ("tag2")
  boneNode->eb("tag2");
  // lsmo ltl_tagPoint entity bone moType ("tag3")
  boneNode->acn("moType")->eb("tag3");

  return true;
}

//------------------------------------------------------------------------------
DthCmd::DthCmd() : Command("dth") {}

//------------------------------------------------------------------------------
bool DthCmd::doExecute() {
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  const std::string& branch = handler->getMatchedBranch();
  Ogre::SceneManager* sceneMgr = sgOgreConsole.getSceneMgr();
  const std::string& movable =
      handler->getMatchedNodeValue("movable", {"sn2", "tag4"});
  const std::string& moType = handler->getMatchedNodeValue(
      "moType", {"m0", "sn1", "sn2", "tag1", "tag3", "tag4"});

  if (branch == "m0") {
    // dth moType movable... ("m0")
    Node* moNode = handler->getMatchedNode("movable");
    StringVector nodes(
        moNode->beginLoopValueIter(), moNode->endLoopValueIter());
    // remove duplicate item
    std::sort(nodes.begin(), nodes.end());
    auto iter = std::unique(nodes.begin(), nodes.end());
    nodes.erase(iter, nodes.end());
    std::for_each(
        nodes.begin(), nodes.end(), [&](const std::string& v) -> void {
          Ogre::MovableObject* mo = OgreUtil::getMovableByIdtype(
              sceneMgr, OgreUtil::getIdFromNameid(v), moType);
          sceneMgr->destroyMovableObject(mo);
        });
  } else if (branch == "sn2" || branch == "tag4") {
    // dth ltl_sceneNode t_sceneNode moType movable("sn2")
    // dth ltl_tagPoint entity bone moType moType("tag4")
    sceneMgr->destroyMovableObject(OgreUtil::getMovableByIdtype(
        sceneMgr, OgreUtil::getIdFromNameid(movable), moType));
  } else if (branch == "sn0" || branch == "sn1") {
    // dth ltl_sceneNode t_sceneNode ("sn0")
    // dth ltl_sceneNode t_sceneNode moType ("sn1")
    std::vector<Ogre::MovableObject*> movables;
    auto sceneNode = OgreUtil::getSceneNodeById(
        sceneMgr, handler->getMatchedNodeUniformValue("t_sceneNode"));
    auto oi = sceneNode->getAttachedObjectIterator();
    while (oi.hasMoreElements()) {
      Ogre::MovableObject* mo = oi.getNext();
      if (moType.empty() || moType == mo->getMovableType())
        movables.push_back(mo);
    }
    destroyMovableObjects(movables);

  } else if (branch == "tag0" || branch == "tag1" || branch == "tag2" ||
             branch == "tag3") {
    // dth ltl_tagPoint entity ("tag0")
    // dth ltl_tagPoint entity moType  ("tag1")
    // dth ltl_tagPoint entity bone ("tag2")
    // dth ltl_tagPoint entity bone moType ("tag3")
    // std::vector<Ogre::MovableObject*> movables;
    // Ogre::Entity* ent =
    // sceneMgr->getEntity(handler->getMatchedNodeValue("entity"));
    // const std::string& bone =
    // handler->getMatchedNodeValue("bone", {"tag2", "tag3"});

    // auto oi = ent->getAttachedObjectIterator();
    // while (oi.hasMoreElements()) {
    // Ogre::MovableObject* mo = oi.getNext();
    // if ((moType.empty() || moType == mo->getMovableType()) &&
    //(bone.empty() || mo->getParentNode()->getParent()->getName() == bone))
    // movables.push_back(mo);
    //}
    // destroyMovableObjects(movables);

  } else {
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "invalid branch" + branch);
  }
  return true;
}

//------------------------------------------------------------------------------
bool DthCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  mArgHandler = handler;

  Node* root = handler->getRoot();
  // dth moType movable... ("m0")
  root->acn("moType")->acn("movable", "movable", Node::NT_LOOP)->eb("m0");
  Node* snNode = root->acn("ltl_sceneNode")->acn("t_sceneNode");
  // dth ltl_sceneNode t_sceneNode ("sn0")
  snNode->eb("sn0");
  // dth ltl_sceneNode t_sceneNode moType ("sn1")
  Node* motNode = snNode->acn("moType");
  motNode->eb("sn1");
  //  dth ltl_sceneNode t_sceneNode moType movable("sn2")
  motNode->acn("movable")->eb("sn2");

  Node* entNode = root->acn("ltl_tagPoint")->acn("item");
  // dth ltl_tagPoint entity ("tag0")
  entNode->eb("tag0");
  // dth ltl_tagPoint entity moType  ("tag1")
  entNode->acn("moType")->eb("tag1");

  Node* boneNode = entNode->acn("bone");
  // dth ltl_tagPoint entity bone ("tag2")
  boneNode->eb("tag2");
  // dth ltl_tagPoint entity bone moType ("tag3")
  motNode = boneNode->acn("moType");
  motNode->eb("tag3");
  // dth ltl_tagPoint entity bone moType movable("tag4")
  motNode->acn("movable")->eb("tag4");
  return true;
}

//------------------------------------------------------------------------------
void DthCmd::destroyMovableObjects(Movables& movables) {
  Ogre::SceneManager* sceneMgr = sgOgreConsole.getSceneMgr();
  std::for_each(
      movables.begin(), movables.end(), [&](Ogre::MovableObject* v) -> void {
        sgConsole.outputLine(
            "destroy " + v->getMovableType() + " : " + v->getName());
        sceneMgr->destroyMovableObject(v);
      });
}

//------------------------------------------------------------------------------
EdmoCmd::EdmoCmd() : Command("edmo", "t_movable") {}

//------------------------------------------------------------------------------
bool EdmoCmd::doExecute() {
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  // const std::string& branch = handler->getMatchedBranch();
  Ogre::SceneManager* sceneMgr = sgOgreConsole.getSceneMgr();

  Ogre::MovableObject* mo =
      OgreUtil::getMovableByIdtype(sceneMgr, handler->getUniformValue());
  AbsDir* dir = sgOgreConsole.getMovableDir()->addUniqueChild(
      OgreUtil::createNameid(mo), OgreSiUtil::createMovableSI(mo));
  sgConsole.setCwd(dir);
  return true;
}

//------------------------------------------------------------------------------
EdndCmd::EdndCmd() : Command("ednd", "t_sceneNode") {}

//------------------------------------------------------------------------------
bool EdndCmd::doExecute() {
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  // const std::string& branch = handler->getMatchedBranch();
  Ogre::SceneManager* sceneMgr = sgOgreConsole.getSceneMgr();

  Ogre::SceneNode* sceneNode =
      OgreUtil::getSceneNodeById(sceneMgr, handler->getUniformValue());
  auto si = new SceneNodeSI(sceneNode);
  si->initParams();
  AbsDir* dir = sgOgreConsole.getNodeDir()->addUniqueChild(
      OgreUtil::createNameid(sceneNode), si);
  sgConsole.setCwd(dir);
  return true;
}

//------------------------------------------------------------------------------
AdndCmd::AdndCmd() : Command("adnd") {}

//------------------------------------------------------------------------------
bool AdndCmd::doExecute() {
  Ogre::SceneManager* sceneMgr = sgOgreConsole.getSceneMgr();
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  // const std::string& branch = handler->getMatchedBranch();
  const std::string& id = handler->getMatchedNodeValue("id");
  const std::string&& parentId =
      handler->getMatchedNodeUniformValue("t_sceneNode");
  Ogre::SceneNode* parentNode = OgreUtil::getSceneNodeById(sceneMgr, parentId);
  if (!parentNode) PAC_EXCEPT(Exception::ERR_INVALID_STATE, "0 parent node");
  auto smmt = enumFromString<Ogre::SceneMemoryMgrTypes>(
      handler->getMatchedNodeValue("en_smmt"));

  Ogre::SceneNode* node = parentNode->createChildSceneNode(smmt);
  node->setName(id);
  return true;
}

//------------------------------------------------------------------------------
bool AdndCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  mArgHandler = handler;
  Node* root = handler->getRoot();
  // adnd t_sceneNode en_smmt id
  root->acn("t_sceneNode")->acn("en_smmt")->acn("id")->eb("0");
  return true;
}

//------------------------------------------------------------------------------
RmndCmd::RmndCmd() : Command("rmnd") {}

//------------------------------------------------------------------------------
bool RmndCmd::doExecute() {
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  const std::string& branch = handler->getMatchedBranch();
  Ogre::SceneManager* sceneMgr = sgOgreConsole.getSceneMgr();
  Ogre::SceneNode* sceneNode = OgreUtil::getSceneNodeById(
      sceneMgr, handler->getMatchedNodeUniformValue("t_sceneNode"));
  if (branch == "0") {
    // rmnd t_sncneNode ("0")
    OgreUtil::destroySceneNodeTotally(sceneNode);
  } else if (branch == "1") {
    // rmnd t_sncneNode ltl_childonly ("1")
    auto oi = sceneNode->getChildIterator();
    while (oi.hasMoreElements()) {
      OgreUtil::destroySceneNodeTotally(
          static_cast<Ogre::SceneNode*>(oi.getNext()));
    }
  } else {
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "unknown branch" + branch);
  }

  return true;
}

//------------------------------------------------------------------------------
bool RmndCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  mArgHandler = handler;
  Node* root = handler->getRoot();
  // rmnd t_sncneNode ("0")
  Node* node = root->acn("t_sceneNode");
  node->eb("0");
  // rmnd t_sncneNode ltl_childonly ("1")
  node->acn("ltl_childOnly")->eb("1");
  return true;
}

//------------------------------------------------------------------------------
AdemitCmd::AdemitCmd() : Command("ademit") {}

//------------------------------------------------------------------------------
bool AdemitCmd::doExecute() {
  Ogre::SceneManager* mgr = sgOgreConsole.getSceneMgr();
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  Ogre::ParticleSystem* ps =
      static_cast<Ogre::ParticleSystem*>(OgreUtil::getMovableByIdtype(
          mgr, handler->getMatchedNodeUniformValue("particle")));
  const std::string& emitterType = handler->getMatchedNodeValue("emitterType");
  ps->addEmitter(emitterType);
  AbsDir* psDir = sgOgreConsole.getMovableDir()->addUniqueChild(
      OgreUtil::createNameid(ps), OgreSiUtil::createMovableSI(ps));
  AbsDir* dir = psDir->getChildByName(
      emitterType + +"_" +
      Ogre::StringConverter::toString(ps->getNumEmitters() - 1));
  sgConsole.setCwd(dir);
  return true;
}

//------------------------------------------------------------------------------
bool AdemitCmd::buildArgHandler() {
  // ademit particle emitterType
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  mArgHandler = handler;
  Node* root = handler->getRoot();
  root->acn("particle")->acn("emitterType")->eb("0");
  return true;
}

//------------------------------------------------------------------------------
RmemitCmd::RmemitCmd() : Command("rmemit") {}

//------------------------------------------------------------------------------
bool RmemitCmd::doExecute() {
  Ogre::SceneManager* mgr = sgOgreConsole.getSceneMgr();
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  Ogre::ParticleSystem* ps =
      static_cast<Ogre::ParticleSystem*>(OgreUtil::getMovableByIdtype(
          mgr, handler->getMatchedNodeUniformValue("particle")));
  const std::string& emitterType = handler->getMatchedNodeValue("emitterType");
  unsigned int index = Ogre::StringConverter::parseUnsignedInt(
      handler->getMatchedNodeValue("index"));
  std::vector<Ogre::ParticleEmitter*> emitters;
  for (size_t i = 0; i < ps->getNumEmitters(); ++i) {
    auto e = ps->getEmitter(i);
    if (e->getType() == emitterType) emitters.push_back(e);
  }

  if (emitters.size() <= index) {
    sgOgreConsole.outputLine(Ogre::StringConverter::toString(index) +
                             " out of " +
                             Ogre::StringConverter::toString(emitters.size()));
    return false;
  }

  ps->removeEmitter(index);
  AbsDir* psDir = sgOgreConsole.getMovableDir()->addUniqueChild(
      OgreUtil::createNameid(ps), OgreSiUtil::createMovableSI(ps));
  sgConsole.setCwd(psDir);

  return true;
}

//------------------------------------------------------------------------------
bool RmemitCmd::buildArgHandler() {
  // ademit particle emitterType index
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  mArgHandler = handler;
  Node* root = handler->getRoot();
  root->acn("particle")->acn("emitterType")->acn("index", "ushort")->eb("0");
  return true;
}

//------------------------------------------------------------------------------
AdafctCmd::AdafctCmd() : Command("adafct") {}

//------------------------------------------------------------------------------
bool AdafctCmd::doExecute() {
  Ogre::SceneManager* mgr = sgOgreConsole.getSceneMgr();
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  Ogre::ParticleSystem* ps =
      static_cast<Ogre::ParticleSystem*>(OgreUtil::getMovableByIdtype(
          mgr, handler->getMatchedNodeUniformValue("particle")));
  const std::string& affectorType =
      handler->getMatchedNodeValue("affectorType");
  ps->addAffector(affectorType);
  sgOgreConsole.getMovableDir()->addUniqueChild(
      OgreUtil::createNameid(ps), OgreSiUtil::createMovableSI(ps));
  AbsDir* psDir = sgOgreConsole.getMovableDir()->addUniqueChild(
      OgreUtil::createNameid(ps), OgreSiUtil::createMovableSI(ps));
  AbsDir* dir = psDir->getChildByName(
      affectorType + "_" +
      Ogre::StringConverter::toString(ps->getNumAffectors() - 1));
  sgConsole.setCwd(dir);

  return true;
}

//------------------------------------------------------------------------------
bool AdafctCmd::buildArgHandler() {
  // adafct particle emitterType
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  mArgHandler = handler;
  Node* root = handler->getRoot();
  root->acn("particle")->acn("affectorType")->eb("0");
  return true;
}

//------------------------------------------------------------------------------
RmafctCmd::RmafctCmd() : Command("rmafct") {}

//------------------------------------------------------------------------------
bool RmafctCmd::doExecute() {
  Ogre::SceneManager* mgr = sgOgreConsole.getSceneMgr();
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  Ogre::ParticleSystem* ps =
      static_cast<Ogre::ParticleSystem*>(OgreUtil::getMovableByIdtype(
          mgr, handler->getMatchedNodeUniformValue("particle")));
  const std::string& affectorType =
      handler->getMatchedNodeValue("affectorType");
  unsigned int index = Ogre::StringConverter::parseUnsignedInt(
      handler->getMatchedNodeValue("index"));
  std::vector<Ogre::ParticleAffector*> afctters;
  for (size_t i = 0; i < ps->getNumAffectors(); ++i) {
    auto e = ps->getAffector(i);
    if (e->getType() == affectorType) afctters.push_back(e);
  }

  if (afctters.size() <= index) {
    sgOgreConsole.outputLine(Ogre::StringConverter::toString(index) +
                             " out of " +
                             Ogre::StringConverter::toString(afctters.size()));
    return false;
  }

  ps->removeAffector(index);
  AbsDir* psDir = sgOgreConsole.getMovableDir()->addUniqueChild(
      OgreUtil::createNameid(ps), OgreSiUtil::createMovableSI(ps));
  sgOgreConsole.setCwd(psDir);

  return true;
}

//------------------------------------------------------------------------------
bool RmafctCmd::buildArgHandler() {
  // adafct particle affectorType index
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  mArgHandler = handler;
  Node* root = handler->getRoot();
  root->acn("particle")->acn("affectorType")->acn("index", "ushort")->eb("0");
  return true;
}

//------------------------------------------------------------------------------
TsnCmd::TsnCmd() : Command("tsn") {}

//------------------------------------------------------------------------------
bool TsnCmd::doExecute() {
  Ogre::SceneManager* mgr = sgOgreConsole.getSceneMgr();
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);

  Ogre::SceneNode* node = OgreUtil::getSceneNodeById(
      mgr, handler->getMatchedNodeUniformValue("t_sceneNode"));
  size_t maxLevel = Ogre::StringConverter::parseUnsignedInt(
      handler->getMatchedNodeValue("maxLevel", {"1"}), -1);
  printNodeTree(node, 0, maxLevel);
  return true;
}

//------------------------------------------------------------------------------
bool TsnCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  mArgHandler = handler;
  Node* root = handler->getRoot();
  Node* node = root->acn("t_sceneNode");
  // tsn t_sceneNode ("0")
  node->eb("0");
  // tsn t_sceneNode maxLevel ("1")
  node->acn("maxLevel", "uint")->eb("1");

  return true;
}

//------------------------------------------------------------------------------
TbCmd::TbCmd() : Command("tb") {}

//------------------------------------------------------------------------------
bool TbCmd::doExecute() {
  Ogre::SceneManager* mgr = sgOgreConsole.getSceneMgr();
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);

  Ogre::Item* item = static_cast<Ogre::Item*>(OgreUtil::getMovableByIdtype(
      mgr, handler->getMatchedNodeUniformValue("item")));

  auto bone = item->getSkeletonInstance()->getBone(
      handler->getMatchedNodeValue("bone"));

  size_t maxLevel = Ogre::StringConverter::parseUnsignedInt(
      handler->getMatchedNodeValue("maxLevel", {"1"}), -1);
  printBoneTree(bone, 0, maxLevel);
  return true;
}

//------------------------------------------------------------------------------
bool TbCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  mArgHandler = handler;
  Node* root = handler->getRoot();
  Node* node = root->acn("item", "t_item")->acn("bone");
  // tb t_item bone ("0")
  node->eb("0");
  // tb t_item bone maxLevel ("1")
  node->acn("maxLevel", "uint")->eb("1");

  return true;
}

//------------------------------------------------------------------------------
LswnCmd::LswnCmd():
    Command("lswn")
{
}

//------------------------------------------------------------------------------
bool LswnCmd::doExecute()
{
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  Ogre::CompositorManager2* cm2 =
      Ogre::Root::getSingleton().getCompositorManager2();
  Ogre::CompositorWorkspace* workspace =
      cm2->getWorkspaceAt(Ogre::StringConverter::parseUnsignedInt(
          handler->getMatchedNodeUniformValue("compWorkspace")));
  const Ogre::CompositorNodeVec& vec = workspace->getNodeSequence();
  RaiiConsoleBuffer rcb;
      std::for_each(vec.begin(),
      vec.end(),[&](decltype(*vec.begin()) v) {
          sgConsole.output(v->getName().getFriendlyText());
      });
  return true;
}

//------------------------------------------------------------------------------
bool LswnCmd::buildArgHandler()
{
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  mArgHandler = handler;
  Node* root = handler->getRoot();
  root->acn("compWorkspace")->eb("0");

  return true;
}
}
