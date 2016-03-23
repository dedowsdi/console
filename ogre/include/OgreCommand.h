#ifndef OGRECOMMAND_H
#define OGRECOMMAND_H

#include "OgreConsolePreRequisite.h"
#include "pacCommand.h"
#include "OgreSI.h"
#include <boost/regex.hpp>

namespace pac {

Ogre::ResourceManager* getResourceManager(const std::string& resType);

/**
 * lsres resType ("0")
 * lsres resType ltl_regex regex ("1")
 *
 * list resource
 */
class _PacExport LsresCmd : public Command {
public:
  LsresCmd();
  virtual Command* clone() { return new LsresCmd(*this); }

protected:
  virtual bool doExecute();
  virtual bool buildArgHandler();
};

/**
 * lsnd ("0")
 * lsnd sceneNode ("1")
 * lsnd ltl_direct sceneNode ("2")
 * lsnd ltl_regex regex ("3")
 */
class _PacExport LsndCmd : public Command {
public:
  LsndCmd();
  virtual Command* clone() { return new LsndCmd(*this); }

protected:
  virtual bool doExecute();
  virtual bool buildArgHandler();

private:
  /**
   * recursively output node name
   * @node : target node
   */
  void outputNode(const Ogre::Node* node);
  /**
   * recursive output node name if it matches regex
   * @param node : target node
   * @param regex : regex expression
   */
  void outputNode(const Ogre::Node* node, boost::regex& regex);
};

/**
 * ath ltl_sceneNode sceneNode ltl_light id ("sn0")
 * ath ltl_sceneNode sceneNode ltl_entity id mesh ("sn1")
 * ath ltl_sceneNode sceneNode ltl_particle id pst ("sn2")
 * ath ltl_sceneNode sceneNode ltl_camera id("sn3")
 * ath ltl_tagPoint entity bone ltl_light id ("tag0")
 * ath ltl_tagPoint entity bone ltl_entity id mesh ("tag1")
 * ath ltl_tagPoint entity bone ltl_particle id pst ("tag2")
 * ath ltl_tagPoint entity bone ltl_camera id ("tag3")
 *
 * attach movable to scenenode or entity bone
 */
class _PacExport AthCmd : public Command {
public:
  AthCmd();
  virtual Command* clone() { return new AthCmd(*this); }

protected:
  virtual bool doExecute();
  virtual bool buildArgHandler();
};

/**
 * lsmo moType ("g0")
 * lsmo moType ltl_regex regex ("g1")
 * lsmo ltl_sceneNode sceneNode ("sn0")
 * lsmo ltl_sceneNode sceneNode moType ("sn1")
 * lsmo ltl_tagPoint entity ("tag0")
 * lsmo ltl_tagPoint entity moType ("tag1")
 * lsmo ltl_tagPoint entity bone ("tag2")
 * lsmo ltl_tagPoint entity bone moType ("tag3")
 */
class _PacExport LsmoCmd : public Command {
public:
  LsmoCmd();
  virtual Command* clone() { return new LsmoCmd(*this); }

protected:
  virtual bool doExecute();
   virtual bool buildArgHandler();
};

/**
 * dth moType movable... ("m0")
 * dth ltl_sceneNode sceneNode ("sn0")
 * dth ltl_sceneNode sceneNode moType ("sn1")
 * dth ltl_sceneNode sceneNode moType movable("sn2")
 * dth ltl_tagPoint entity ("tag0")
 * dth ltl_tagPoint entity moType  ("tag1")
 * dth ltl_tagPoint entity bone ("tag2")
 * dth ltl_tagPoint entity bone moType ("tag3")
 * dth ltl_tagPoint entity bone moType movable("tag4")
 *
 * detach movable
 */
class _PacExport DthCmd : public Command {
public:
  DthCmd();
  virtual Command* clone() { return new DthCmd(*this); }

protected:
  virtual bool doExecute();
  virtual bool buildArgHandler();

private:
  typedef std::vector<Ogre::MovableObject*> Movables;
  void destroyMovableObjects(Movables& movables);
};

/**
 * edmo moTyp movable ("0")
 * edmo ltl_sceneNode sceneNode moType movable ("1")
 * edmo ltl_tagPoint entity bone moType movable ("2")
 *
 * Edit movable object, you need to make sure this movable remains in scene when
 * you edit it, and clean it after you have done your work
 */
class _PacExport EdmoCmd : public Command {
public:
  EdmoCmd();
  virtual Command* clone() { return new EdmoCmd(*this); }

protected:
  virtual bool doExecute();
  virtual bool buildArgHandler();
};

/**
 * ednd sceneNode ("0")
 *
 * Edit scenenode
 */
class _PacExport EdndCmd : public Command {
public:
  EdndCmd();
  virtual Command* clone() { return new EdndCmd(*this); }

protected:
  virtual bool doExecute();
  virtual bool buildArgHandler();
};

/**
 * and id ("0")
 * and sncneNode id ("1")
 *
 * add scenenode 
 */
class _PacExport AdndCmd : public Command {
public:
  AdndCmd();
  virtual Command* clone() { return new AdndCmd(*this); }

protected:
  virtual bool doExecute();
  virtual bool buildArgHandler();
};

/**
 * and sncneNode id ("0")
 *
 * add scenenode 
 */
class _PacExport RmndCmd : public Command {
public:
  RmndCmd();
  virtual Command* clone() { return new RmndCmd(*this); }

protected:
  virtual bool doExecute();
};
}

#endif /* OGRECOMMAND_H */
