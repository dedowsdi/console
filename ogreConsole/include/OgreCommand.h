#ifndef OGRECOMMAND_H
#define OGRECOMMAND_H

#include "OgreConsolePreRequisite.h"
#include "pacCommand.h"
#include "OgreSI.h"

namespace pac {

Ogre::ResourceManager* getResourceManager(const std::string& resType);
void printNodeTree(Ogre::Node* node, size_t level, size_t maxLevel);
void printBoneTree(Ogre::Bone* node, size_t level, size_t maxLevel);

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
 * lsnd ("g0")
 * lsnd en_smmt ("g1")
 * lsnd [-r] sceneNode ("s0")      [-r] recursively list children
 * lsnd [-r] sceneNode en_smmt("s1")
 * lsnd ltl_regex regex ("r0")
 * lsnd ltl_regex regex en_smmt("r1")
 * lsnd ltl_parentOfNode t_sceneNode ("ps0")
 * lsnd ltl_parentOfMovable moType movable ("pm0")
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
  void outputNode(const Ogre::Node* node, int smmt);
  /**
   * recursive output node name if it matches regex
   * @param node : target node
   * @param regex : regex expression
   */
  void outputNode(const Ogre::Node* node, int smmt, boost::regex& regex);
};

/**
 * ath ltl_sceneNode t_sceneNode ltl_light id ("sn0")
 * ath ltl_sceneNode t_sceneNode ltl_item id mesh ("sn1")
 * ath ltl_sceneNode t_sceneNode ltl_particle id pst ("sn2")
 * ath ltl_sceneNode t_sceneNode ltl_camera id("sn3")
 * ath ltl_tagPoint item bone ltl_light id ("tag0")
 * ath ltl_tagPoint item bone ltl_item id mesh ("tag1")
 * ath ltl_tagPoint item bone ltl_particle id pst ("tag2")
 * ath ltl_tagPoint item bone ltl_camera id ("tag3")
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
 * lsmo ltl_tagPoint Item ("tag0")
 * lsmo ltl_tagPoint Item moType ("tag1")
 * lsmo ltl_tagPoint Item bone ("tag2")
 * lsmo ltl_tagPoint Item bone moType ("tag3")
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
 * dth ltl_sceneNode t_sceneNode ("sn0")
 * dth ltl_sceneNode t_sceneNode moType ("sn1")
 * dth ltl_sceneNode t_sceneNode moType movable("sn2")
 *
 *
 * not supported
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

class _PacExport EdmoCmd : public Command {
public:
  EdmoCmd();
  virtual Command* clone() { return new EdmoCmd(*this); }

protected:
  virtual bool doExecute();
};

/**
 * ednd t_sceneNode ("0")
 *
 * Edit scenenode
 */
class _PacExport EdndCmd : public Command {
public:
  EdndCmd();
  virtual Command* clone() { return new EdndCmd(*this); }

protected:
  virtual bool doExecute();
};

/**
 * adnd t_sncneNode en_smmt id ("1")
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
 * rmnd t_sncneNode ("0")
 * rmnd t_sncneNode ltl_childonly ("1")
 *
 * destroy scenenode , all it's children and all the movable attached to them
 */
class _PacExport RmndCmd : public Command {
public:
  RmndCmd();
  virtual Command* clone() { return new RmndCmd(*this); }

protected:
  virtual bool doExecute();
  virtual bool buildArgHandler();
};

/**
 * ademit particle emitterType
 */
class _PacExport AdemitCmd : public Command {
public:
  AdemitCmd();
  virtual Command* clone() { return new AdemitCmd(*this); }

protected:
  virtual bool doExecute();
  virtual bool buildArgHandler();
};

/**
 * rmemit particle emitterType index
 */
class _PacExport RmemitCmd : public Command {
public:
  RmemitCmd();
  virtual Command* clone() { return new RmemitCmd(*this); }

protected:
  virtual bool doExecute();
  virtual bool buildArgHandler();
};

/**
 * adafct particle affectorType
 */
class _PacExport AdafctCmd : public Command {
public:
  AdafctCmd();
  virtual Command* clone() { return new AdafctCmd(*this); }

protected:
  virtual bool doExecute();
  virtual bool buildArgHandler();
};

/**
 * rmafct particle affectorType index
 */
class _PacExport RmafctCmd : public Command {
public:
  RmafctCmd();
  virtual Command* clone() { return new RmafctCmd(*this); }

protected:
  virtual bool doExecute();
  virtual bool buildArgHandler();
};

/**
 * tsn t_sceneNode ("0")
 * tsn t_sceneNode maxLevel ("1")
 */
class _PacExport TsnCmd : public Command {
public:
  TsnCmd();
  virtual Command* clone() { return new TsnCmd(*this); }

protected:
  virtual bool doExecute();
  virtual bool buildArgHandler();
};

/**
 * tb t_sceneNode ("0")
 * tb t_sceneNode maxLevel ("1")
 */
class _PacExport TbCmd : public Command {
public:
  TbCmd();
  virtual Command* clone() { return new TbCmd(*this); }

protected:
  virtual bool doExecute();
  virtual bool buildArgHandler();
};

class _PacExport LswnCmd: public Command {
public:
  LswnCmd();
  virtual Command* clone() { return new LswnCmd(*this); }

protected:
  virtual bool doExecute();
  virtual bool buildArgHandler();

};
}

#endif /* OGRECOMMAND_H */
