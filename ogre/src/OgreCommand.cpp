#include "OgreCommand.h"
#include "pacArgHandler.h"
#include "OgreConsole.h"
#include <OgreSI.h>
#include "pacEnumUtil.h"
#include "pacStringUtil.h"
#include "pacAbsDir.h"
#include "OgreArgHandler.h"
#include <Ogre.h>
#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreMeshManager.h>
#include <OgreTextureManager.h>
#include <OgreParticleSystemManager.h>
#include <OgreCompositorManager.h>

typedef Ogre::ResourceManager::ResourceMapIterator ResMgrIter;
typedef Ogre::SceneManager::MovableObjectIterator SceneMgrMoIter;
typedef Ogre::Node::ChildNodeIterator NodeIter;

namespace pac {

//------------------------------------------------------------------------------
Ogre::ResourceManager* getResourceManager(const std::string& resType) {
  if (resType == "material") return Ogre::MaterialManager::getSingletonPtr();
  if (resType == "mesh") return Ogre::MeshManager::getSingletonPtr();
  if (resType == "texture") return Ogre::TextureManager::getSingletonPtr();
  if (resType == "compositor")
    return Ogre::CompositorManager::getSingletonPtr();
  PAC_EXCEPT(Exception::ERR_INVALIDPARAMS, resType + " is not defined ");
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

  if (resType != "pst") {
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
  } else {
    // particle system template
    auto oi =
        Ogre::ParticleSystemManager::getSingletonPtr()->getTemplateIterator();
    while (oi.hasMoreElements()) {
      auto ps = oi.getNext();
      if (reExp.empty() || boost::regex_match(ps->getName(), regex))
        sgConsole.output(ps->getName());
    }
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
  Ogre::SceneNode* rootNode = sceneMgr->getRootSceneNode();

  const std::string& nodeName =
      handler->getMatchedNodeValue("sceneNode", {"1", "2"});
  const std::string& reExp = handler->getMatchedNodeValue("regex", {"3"});

  RaiiConsoleBuffer rcb;
  if (branch == "0") {
    // lsnd
    outputNode(rootNode);
  } else if (branch == "1") {
    // lsnd sceneNode
    Ogre::SceneNode* node = sceneMgr->getSceneNode(nodeName);
    outputNode(node);
  } else if (branch == "2") {
    // lsnd direct sceneNode
    Ogre::SceneNode* node = sceneMgr->getSceneNode(nodeName);
    auto ni = node->getChildIterator();
    while (ni.hasMoreElements()) {
      Ogre::Node* childNode = ni.getNext();
      sgConsole.output(childNode->getName());
    }
  } else if (branch == "3") {
    // lsnd ltl_regex regex ("3")
    boost::regex regex(reExp);
    outputNode(rootNode, regex);
  } else {
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "invalid branch:" + branch);
  }
  return true;
}

//------------------------------------------------------------------------------
bool LsndCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  Node* root = handler->getRoot();
  // lsnd ("0")
  root->eb("0");
  // lsnd sceneNode ("1")
  root->acn("sceneNode")->eb("1");
  // lsnd direct sceneNode ("2")
  root->acn("ltl_direct")->acn("sceneNode")->eb("2");
  // lsnd ltl_regex regex ("3")
  root->acn("ltl_regex")->acn("regex")->eb("3");
  mArgHandler = handler;
  return true;
}

//------------------------------------------------------------------------------
void LsndCmd::outputNode(const Ogre::Node* node) {
  sgConsole.output(node->getName());

  auto oi = node->getChildIterator();
  while (oi.hasMoreElements()) outputNode(oi.getNext());
}

//------------------------------------------------------------------------------
void LsndCmd::outputNode(const Ogre::Node* node, boost::regex& regex) {
  if (boost::regex_match(node->getName(), regex))
    sgConsole.output(node->getName());

  auto oi = node->getChildIterator();
  while (oi.hasMoreElements()) outputNode(oi.getNext(), regex);
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
  else if (handler->hasMatchedNode("ltl_entity"))
    moType = "Entity";
  else if (handler->hasMatchedNode("ltl_particle"))
    moType = "ParticleSystem";
  else if (handler->hasMatchedNode("ltl_camera"))
    moType = "Camera";
  else
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "blank motype");
  if (sceneMgr->hasMovableObject(id, moType)) {
    sgConsole.outputLine(moType + " : " + id + " already exists");
    return false;
  }

  if (branch == "sn0" || branch == "tag0") {
    // ath ltl_sceneNode sceneNode ltl_light id ("sn0")
    // ath ltl_tagPoint entity bone ltl_light id ("tag0")
    mo = sceneMgr->createLight(id);
  } else if (branch == "sn1" || branch == "tag1") {
    // ath ltl_sceneNode sceneNode ltl_entity id mesh ("sn1")
    // ath ltl_tagPoint entity bone ltl_entity id mesh ("tag1")
    mo = sceneMgr->createEntity(id, handler->getMatchedNodeValue("mesh"));
  } else if (branch == "sn2" || branch == "tag2") {
    // ath ltl_sceneNode sceneNode ltl_particle id pst ("sn2")
    // ath ltl_tagPoint entity bone ltl_particle id pst ("tag2")
    mo =
        sceneMgr->createParticleSystem(id, handler->getMatchedNodeValue("pst"));
  } else if (branch == "sn3" || branch == "tag3") {
    // ath ltl_sceneNode sceneNode ltl_camera id("sn3")
    // ath ltl_tagPoint entity bone ltl_camera id ("tag3")
    mo = sceneMgr->createCamera(id);
  }

  if (branch.size() > 0 && branch[0] == 's') {
    const std::string& snName = handler->getMatchedNodeValue("sceneNode");
    Ogre::SceneNode* sceneNode = sceneMgr->getSceneNode(snName);
    sceneNode->attachObject(mo);
  } else if (branch.size() > 0 && branch[0] == 't') {
    Ogre::Entity* ent =
        sceneMgr->getEntity(handler->getMatchedNodeValue("entity"));
    const std::string& bone = handler->getMatchedNodeValue("bone");
    ent->attachObjectToBone(bone, mo);
  } else {
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "invalid branch");
  }
  return true;
}

//------------------------------------------------------------------------------
bool AthCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  Node* root = handler->getRoot();
  Node* idNode = root->acn("ltl_sceneNode")->acn("sceneNode");
  // ath ltl_sceneNode sceneNode ltl_light id ("sn0")
  idNode->acn("ltl_light")->acn("id")->eb("sn0");
  // ath ltl_sceneNode sceneNode ltl_entity id mesh ("sn1")
  idNode->acn("ltl_entity")->acn("id")->acn("mesh")->eb("sn1");
  // ath ltl_sceneNode sceneNode ltl_particle id pst ("sn2")
  idNode->acn("ltl_particle")->acn("id")->acn("pst")->eb("sn2");
  // ath ltl_sceneNode sceneNode ltl_camera id("sn3")
  idNode->acn("ltl_camera")->acn("id")->eb("sn3");

  Node* entNode = root->acn("ltl_tagPoint")->acn("entity")->acn("bone");
  // ath ltl_tagPoint entity bone ltl_light id ("tag0")
  entNode->acn("ltl_light")->acn("id")->eb("tag0");
  // ath ltl_tagPoint entity bone ltl_entity id mesh ("tag1")
  entNode->acn("ltl_entity")->acn("id")->acn("mesh")->eb("tag1");
  // ath ltl_tagPoint entity bone ltl_particle id pst ("tag2")
  entNode->acn("ltl_particle")->acn("id")->acn("pst")->eb("tag2");
  // ath ltl_tagPoint entity bone ltl_camera id ("tag3")
  entNode->acn("ltl_camera")->acn("id")->eb("tag3");

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

  if (branch == "g0" || branch == "g1") {
    // lsmo moType ("g0")
    // lsmo moType ltl_regex regex ("g1")
    const std::string& moType = handler->getMatchedNodeValue("moType");
    const std::string& reExp = handler->getMatchedNodeValue("regex", {"g1"});

    boost::regex regex(reExp);
    SceneMgrMoIter oi = sceneMgr->getMovableObjectIterator(moType);
    while (oi.hasMoreElements()) {
      Ogre::MovableObject* mo = oi.getNext();
      if (reExp.empty() || boost::regex_match(mo->getName(), regex))
        sgConsole.output(mo->getName());
    }
  } else if (branch == "sn0" || branch == "sn1") {
    // lsmo ltl_sceneNode sceneNode ("sn0")
    // lsmo ltl_sceneNode sceneNode moType ("sn1")
    Ogre::SceneNode* sceneNode =
        sceneMgr->getSceneNode(handler->getMatchedNodeValue("sceneNode"));
    const std::string& moType = handler->getMatchedNodeValue("moType", {"sn1"});
    auto oi = sceneNode->getAttachedObjectIterator();
    RaiiConsoleBuffer rcb;
    while (oi.hasMoreElements()) {
      Ogre::MovableObject* mo = oi.getNext();
      if (moType.empty() || mo->getMovableType() == moType)
        sgConsole.output(mo->getName());
    }
  } else if (branch == "tag0" || branch == "tag1" || branch == "tag2" ||
             branch == "tag3") {
    // lsmo ltl_tagPoint entity ("tag0")
    // lsmo ltl_tagPoint entity moType ("tag1")
    // lsmo ltl_tagPoint entity bone ("tag2")
    // lsmo ltl_tagPoint entity bone moType ("tag3")
    Ogre::Entity* ent =
        sceneMgr->getEntity(handler->getMatchedNodeValue("entity"));
    const std::string& bone =
        handler->getMatchedNodeValue("bone", {"tag2", "tag3"});
    const std::string& moType =
        handler->getMatchedNodeValue("moType", {"tag1", "tag3"});
    auto oi = ent->getAttachedObjectIterator();
    RaiiConsoleBuffer rcb;
    while (oi.hasMoreElements()) {
      Ogre::MovableObject* mo = oi.getNext();
      Ogre::Node* athNode = mo->getParentNode()->getParent();

      if ((moType.empty() || moType == mo->getMovableType()) &&
          (bone.empty() || athNode->getName() == bone))
        sgConsole.output(mo->getName());
    }
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

  Node* entNode = root->acn("ltl_tagPoint")->acn("entity");
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
    const std::string& moType = handler->getMatchedNodeValue("moType");

    std::for_each(
        nodes.begin(), nodes.end(), [&](const std::string& v) -> void {
          Ogre::MovableObject* mo = sceneMgr->getMovableObject(v, moType);
          sceneMgr->destroyMovableObject(mo);
        });
  } else if (branch == "sn2" || branch == "tag4") {
    // dth ltl_sceneNode sceneNode moType movable("sn2")
    // dth ltl_tagPoint entity bone moType moType("tag4")
    sceneMgr->destroyMovableObject(movable, moType);
  } else if (branch == "sn0" || branch == "sn1") {
    // dth ltl_sceneNode sceneNode ("sn0")
    // dth ltl_sceneNode sceneNode moType ("sn1")
    std::vector<Ogre::MovableObject*> movables;
    auto sceneNode =
        sceneMgr->getSceneNode(handler->getMatchedNodeValue("sceneNode"));
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
    std::vector<Ogre::MovableObject*> movables;
    Ogre::Entity* ent =
        sceneMgr->getEntity(handler->getMatchedNodeValue("entity"));
    const std::string& bone =
        handler->getMatchedNodeValue("bone", {"tag2", "tag3"});

    auto oi = ent->getAttachedObjectIterator();
    while (oi.hasMoreElements()) {
      Ogre::MovableObject* mo = oi.getNext();
      if ((moType.empty() || moType == mo->getMovableType()) &&
          (bone.empty() || mo->getParentNode()->getParent()->getName() == bone))
        movables.push_back(mo);
    }
    destroyMovableObjects(movables);

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
  Node* snNode = root->acn("ltl_sceneNode")->acn("sceneNode");
  // dth ltl_sceneNode sceneNode ("sn0")
  snNode->eb("sn0");
  // dth ltl_sceneNode sceneNode moType ("sn1")
  Node* motNode = snNode->acn("moType");
  motNode->eb("sn1");
  //  dth ltl_sceneNode sceneNode moType movable("sn2")
  motNode->acn("movable")->eb("sn2");

  Node* entNode = root->acn("ltl_tagPoint")->acn("entity");
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
EdmoCmd::EdmoCmd() : Command("edmo") {}

//------------------------------------------------------------------------------
bool EdmoCmd::doExecute() {
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  const std::string& branch = handler->getMatchedBranch();
  if (branch != "0" && branch != "1" && branch != "2")
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "invalid branch:" + branch);

  Ogre::SceneManager* sceneMgr = sgOgreConsole.getSceneMgr();

  const std::string& moType = handler->getMatchedNodeValue("moType");
  const std::string& movable = handler->getMatchedNodeValue("movable");
  Ogre::MovableObject* mo = sceneMgr->getMovableObject(movable, moType);
  AbsDir* dir = sgOgreConsole.getMovableDir()->addUniqueChild(
      mo->getName(), OgreSiUtil::createMovableSI(mo));
  sgConsole.setCwd(dir);
  return true;
}

//------------------------------------------------------------------------------
bool EdmoCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  mArgHandler = handler;
  Node* root = handler->getRoot();

  // edmo moTyp movable ("0")
  root->acn("moType")->acn("movable")->eb("0");
  // edmo ltl_sceneNode sceneNode moType movable ("1")
  root->acn("ltl_sceneNode")
      ->acn("sceneNode")
      ->acn("moType")
      ->acn("movable")
      ->eb("1");
  // edmo ltl_tagPoint entity bone moType movable ("2")
  root->acn("ltl_tagPoint")
      ->acn("entity")
      ->acn("bone")
      ->acn("moType")
      ->acn("movable")
      ->eb("2");
  return true;
}

//------------------------------------------------------------------------------
EdndCmd::EdndCmd() : Command("ednd") {}

//------------------------------------------------------------------------------
bool EdndCmd::doExecute() {
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  const std::string& branch = handler->getMatchedBranch();
  if (branch != "0")
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "invalid branch:" + branch);

  Ogre::SceneManager* sceneMgr = sgOgreConsole.getSceneMgr();
  sgOgreConsole.freeze();
  Ogre::SceneNode* sceneNode =
      sceneMgr->getSceneNode(handler->getMatchedNodeValue("sceneNode"));
  AbsDir* dir = sgOgreConsole.getNodeDir()->addUniqueChild(
      sceneNode->getName(), new SceneNodeSI(sceneNode));
  sgConsole.setCwd(dir);
  return true;
}

//------------------------------------------------------------------------------
bool EdndCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  mArgHandler = handler;
  Node* root = handler->getRoot();
  // ednd sceneNode
  root->acn("sceneNode")->eb("0");
  return true;
}

//------------------------------------------------------------------------------
AdndCmd::AdndCmd() : Command("adnd", "id") {}

//------------------------------------------------------------------------------
bool AdndCmd::doExecute() {
  Ogre::SceneManager* sceneMgr = sgOgreConsole.getSceneMgr();
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  const std::string& branch = handler->getMatchedBranch();
  const std::string& id = handler->getMatchedNodeValue("id");
  if (sceneMgr->hasSceneNode(id)) {
    sgConsole.outputLine(id + " is already taken, pls use another one");
    return false;
  }

  Ogre::SceneNode* parentNode;
  if (branch == "0") {
    // adnd id ("0")
    parentNode = sceneMgr->getRootSceneNode();
  } else if (branch == "1") {
    // adnd sceneNode id ("1")
    const std::string& nodeName = handler->getMatchedNodeValue("sceneNode");
    parentNode = sceneMgr->getSceneNode(nodeName);
  } else
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "unknow branch:" + branch);

  parentNode->createChildSceneNode(id);
  return true;
}

//------------------------------------------------------------------------------
bool AdndCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  mArgHandler = handler;
  Node* root = handler->getRoot();
  // adnd id
  root->acn("id")->eb("0");
  // adnd sceneNode id
  root->acn("sceneNode")->acn("id")->eb("1");
  return true;
}

//------------------------------------------------------------------------------
RmndCmd::RmndCmd() : Command("rmnd", "sceneNode") {}

//------------------------------------------------------------------------------
bool RmndCmd::doExecute() {
  const std::string& ndName = mArgHandler->getValue();
  Ogre::SceneManager* sceneMgr = sgOgreConsole.getSceneMgr();
  Ogre::SceneNode* node = sceneMgr->getSceneNode(ndName);
  sceneMgr->destroySceneNode(node);
  return true;
}
}
