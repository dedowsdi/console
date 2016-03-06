#include "OgreCommand.h"
#include "pacArgHandler.h"
#include "OgreConsole.h"
#include <Ogre.h>
#include <OgreSceneManager.h>
#include <OgreSI.h>
#include "pacEnumUtil.h"
#include "pacStringUtil.h"

namespace pac {

typedef Ogre::ResourceManager::ResourceMapIterator ResMgrIter;
typedef Ogre::SceneManager::MovableObjectIterator SceneMgrMoIter;
typedef Ogre::Node::ChildNodeIterator NodeIter;

//------------------------------------------------------------------------------
LsmatCmd::LsmatCmd() : Command("lsmat") {}

//------------------------------------------------------------------------------
bool LsmatCmd::doExecute() {
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  const std::string& branch = handler->getMatchedBranch();
  if (branch != "0" && branch != "1")
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "invalidate branch:" + branch);

  auto oi = sgMatMgr.getResourceIterator();

  // lsmat ("0")
  // lsmat ltl_regex regex ("1")
  const std::string& reExp = handler->getMatchedNodeValue("regex");
  boost::regex regex(reExp);

  RaiiConsoleBuffer rcb;
  while (oi.hasMoreElements()) {
    Ogre::ResourcePtr ptr = oi.getNext();
    if (reExp.empty() || boost::regex_match(ptr->getName(), regex))
      sgConsole.output(ptr->getName());
  }

  return true;
}

//------------------------------------------------------------------------------
bool LsmatCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  Node* root = handler->getRoot();
  // lsmat
  root->eb("0");
  // lsmat ltl_regex regex ("1")
  root->acn("ltl_regex")->acn("regex")->eb("1");
  mArgHandler = handler;
  return true;
}

//------------------------------------------------------------------------------
Lsnd::Lsnd() : Command("lsnd") {}

//------------------------------------------------------------------------------
bool Lsnd::doExecute() {
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  const std::string& branch = handler->getMatchedBranch();
  Ogre::SceneManager* sceneMgr = sgOgreConsole.getSceneMgr();
  Ogre::SceneNode* rootNode = sceneMgr->getRootSceneNode();

  const std::string& nodeName =
      handler->getMatchedNodeValue("sceneNode", ({"1", "2"}));
  const std::string& reExp = handler->getMatchedNodeValue("regex", ({"3"}));

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
    NodeIter ni = node->getChildIterator();
    std::for_each(ni.begin(), ni.end(), [&](NodeIter::ValueType& v) -> void {
      sgConsole.output(v->getName());
    });
  } else if (branch == "3") {
    // lsnd ltl_regex regex ("3")
    outputNode(rootNode, boost::regex(reExp));
  } else {
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "invalid branch:" + branch);
  }
  return true;
}

//------------------------------------------------------------------------------
bool Lsnd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  Node* root = handler->getRoot();
  // lsnd ("0")
  root->eb("0");
  // lsnd sceneNode ("1")
  root->acn("sceneNode")->eb("1");
  // lsnd direct sceneNode ("2")
  root->acn("ltl_direct")->acn("sceneNode")->eb("2");
  // lsnd ltl_regex regex ("3")
  root->acn("regex")->eb("3");
  mArgHandler = handler;
  return true;
}

//------------------------------------------------------------------------------
void Lsnd::outputNode(const Ogre::Node* node) {
  sgConsole.output(node->getName());

  auto oi = node->getChildIterator();
  while (oi.hasMoreElements()) outputNode(oi.getNext());
}

//------------------------------------------------------------------------------
void Lsnd::outputNode(const Ogre::Node* node, boost::regex& regex) {
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

  if (branch == "sn0" || branch == "sn1" || branch == "sn2") {
    const std::string& snName = handler->getMatchedNodeValue("sceneNode");
    Ogre::SceneNode* sceneNode = sceneMgr->getSceneNode(snName);
    if (branch == "sn0") {
      // ath ltl_sceneNode sceneNode ltl_light id ("tag0")
      mo = sceneMgr->createLight(id);
    } else if (branch == "sn1") {
      // ath ltl_sceneNode sceneNode ltl_entity id mesh ("sn1")
      mo = sceneMgr->createEntity(id, handler->getMatchedNodeValue("mesh"));
    } else {
      // ath ltl_sceneNode sceneNode ltl_particle id pst ("sn2")
      mo = sceneMgr->createParticleSystem(
          id, handler->getMatchedNodeValue("pst"));
    }
    sceneNode->attachObject(mo);
  } else if (branch == "tag0" || branch == "tag1" || branch == "tag2") {
    Ogre::Entity* ent =
        sceneMgr->getEntity(handler->getMatchedNodeValue("entity"));
    const std::string& bone = handler->getMatchedNodeValue("bone");
    if (branch == "tag0") {
      // ath ltl_tagPoint entity bone ltl_light id ("tag0")
      mo = sceneMgr->createLight(id);
    } else if (branch == "tag1") {
      // ath ltl_tagPoint entity bone ltl_entity id mesh ("tag1")
      mo = sceneMgr->createEntity(id, handler->getMatchedNodeValue("mesh"));
    } else {
      // ath ltl_tagPoint entity bone ltl_particle id pst ("tag2")
      mo = sceneMgr->createParticleSystem(
          id, handler->getMatchedNodeValue("pst"));
    }
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
  root->eb("0");  // lsmo
  Node* idNode = root->acn("ltl_sceneNode")->acn("sceneNode");
  // ath ltl_sceneNode sceneNode ltl_light id ("sn0")
  idNode->acn("ltl_light")->acn("id")->eb("sn0");
  // ath ltl_sceneNode sceneNode ltl_entity id mesh ("sn1")
  idNode->acn("ltl_entity")->acn("id")->acn("mesh")->eb("sn1");
  // ath ltl_sceneNode sceneNode ltl_particle id pst ("sn2")
  idNode->acn("ltl_particle")->acn("id")->acn("pst")->eb("sn2");

  Node* entNode = root->acn("ltl_tagPoint")->acn("entity")->acn("bone");
  // ath ltl_tagPoint entity bone ltl_light id ("tag0")
  entNode->acn("ltl_light")->acn("id")->eb("tag0");
  // ath ltl_tagPoint entity bone ltl_entity id mesh ("tag1")
  entNode->acn("ltl_entity")->acn("id")->acn("mesh")->eb("tag1");
  // ath ltl_tagPoint entity bone ltl_particle id pst ("tag2")
  entNode->acn("ltl_particle")->acn("id")->acn("pst")->eb("sn3");

  mArgHandler = handler;
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
    const std::string& moType = getMatchedNodeValue("moType");
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
    const std::string& moType = getMatchedNodeValue("moType", ({"sn1"}));
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
        handler->getMatchedNodeValue("bone", ({"tag2", "tag3"}));
    const std::string& moType =
        handler->getMatchedNodeValue("moType", ({"tag1", "tag3"}));
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
}

//------------------------------------------------------------------------------
bool LsmoCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  Node* root = handler->getRoot();
  mArgHandler = handler;

  // lsmo moType ("g0")
  root->eb("g0");
  // lsmo moType ltl_regex regex ("g1")
  root->acn("ltl_regex")->acn("regex")->eb("1");

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

  if (branch == "mo0") {
    // dth moType movable... ("m0")
    Node* moNode = handler->getMatchedNode("movable");
    StringVector nodes(
        moNode->beginLoopValueIter(), moNode->endLoopValueIter());
    // remove duplicate item
    std::sort(nodes.begin(), nodes.end());
    auto iter = std::unique(nodes.begin(), nodes.end());
    std::erase(iter, nodes.end());

    std::for_each(
        nodes.begin(), nodes.end(), [&](const std::string& v) -> void {
          Ogre::MovableObject* mo = sceneMgr->getMovableObject(
              handelr->getMatchedNodeValue("moType"));
          //  destroy it
          sceneMgr->destroyManualObject(mo);
        });
  } else if (branch == "sn0" || branch == "sn1") {
    // dth ltl_sceneNode sceneNode ("sn0")
    // dth ltl_sceneNode sceneNode moType ("sn1")
    std::vector<Ogre::MovableObject*> movables;
    auto sceneNode =
        sceneMgr->getSceneNode(handler->getMatchedNodeValue("sceneNode"));
    const std::string& moType =
        handler->getMatchedNodeValue("moType", ({"sn1"}));
    auto oi = sceneNode->getAttachedObjectIterator();
    while (oi->hasMoreElements()) {
      Ogre::MovableObject* mo = oi->getNext();
      if (moType.empty() || moType == mo->getMovableType())
        movables.push_back(mo);
    }
    destroyMovableObjects(movables);

  } else if (branch == "tag0" || branch == "tag1" || branch == "tag2" ||
             branch == "tag3") {
    // dth ltl_entity entity ("tag0")
    // dth ltl_entity entity moType  ("tag1")
    // dth ltl_entity entity bone ("tag2")
    // dth ltl_entity entity bone moType ("tag3")
    std::vector<Ogre::MovableObject*> movables;
    Ogre::Entity* ent =
        sceneMgr->getEntity(handler->getMatchedNodeValue("entity"));
    const std::string& moType =
        handler->getMatchedNodeValue("moType", ({"tag1", "tag3"}));
    const std::string& bone =
        handler->getMatchedNodeValue("bone", ({"tag2", "tag3"}));

    auto oi = ent->getAttachedObjectIterator();
    while (oi->hasMoreElements()) {
      Ogre::MovableObject* mo = oi->getNext();
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
  // dth movable... ("m0")
  root->acn("movable", "movable", Node::NT_LOOP)->eb("m0");
  Node* snNode = root->acn("ltl_sceneNode")->acn("sceneNode");
  // dth ltl_sceneNode sceneNode ("sn0")
  snNode->eb("sn0");
  // dth ltl_sceneNode sceneNode moType ("sn1")
  snNode->acn("moType")->eb("sn1");

  Node* entNode = root->acn("ltl_entity")->acn("entity");
  // dth ltl_entity entity ("tag0")
  entNode->eb("tag0");
  // dth ltl_entity entity moType  ("tag1")
  engNode->acn("moType")->eb("tag1");

  Node* boneNode = entNode->acn("bone");
  // dth ltl_entity entity bone ("tag2")
  boneNode->eb("tag2");
  // dth ltl_entity entity bone moType ("tag3")
  boneNode->acn("moType")->eb("tag3");
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
  sgOgreConsole.freeze();
  Ogre::MovableObject* mo = sceneMgr->getMovableObject(movable, moType);
  MovableSI* si = OgreSiUtil::createMovableSI(mo);
  AbsDir* dir = new AbsDir(si->getName(), si);
  sgOgreConsole.getMovableDir()->addChild(dir);
  sgConsole.setCwd(dir) : return true;
}

//------------------------------------------------------------------------------
bool EdmoCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  mArgHandler = handler;
  Node* root = handler->getRoot();

  // edmo moTyp movable ("0")
  root->acn("edmo")->eb("0");
  // edmo ltl_sceneNode sceneNode moType movable ("1")
  root->acn("ltl_sceneNode")->acn("sceneNode")->acn("moType")->acn("movable");
  // edmo ltl_tagPoint entity bone moType movable ("2")
  root->acn("ltl_tagPoint")
      ->acn("entity")
      ->acn("bone")
      ->acn("moType")
      ->acn("movable");
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
  Ogre::SceneNode* sceneNode =
      sceneMgr->getSceneNode(handler->getMatchedNodeValue("handler"));
  sgOgreConsole.freeze();
  SceneNodeSI* si = new SceneNodeSI(sceneNode);
  AbsDir* dir = new AbsDir(si->getName(), si);
  sgOgreConsole.getNodeDir()->addChild(dir);
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
}
}
