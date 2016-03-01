#ifndef OGRECOMMAND_H
#define OGRECOMMAND_H

#include "OgreConsolePreRequisite.h"
#include "pacCommand.h"

namespace pac {

/**
 * List moveable object in scene
 * lsmo <moType> [reMoveable] ("0")
 */
class _PacExport LsmoCmd : public Command {
public:
  LsmoCmd();
};

/**
 * lsmat [reMaterial] ("0")
 */
class _PacExport LsmatCmd : public Command {
public:
  LsmatCmd();
};

/**
 * lsnd [reNode] ("0")
 * lsnd [ltSub] [node] ("1")
 */
class _PacExport Lsnd : public Command {
public:
  Lsnd();
};

/**
 * ath <ltlSceneNode> <sceneNode> <ltlLight> <id> <lightType> ("sn0")
 * ath <ltlSceneNode> <sceneNode> <ltlEntity> <id> <mesh> ("sn1")
 * ath <ltlSceneNode> <sceneNode> <ltlParticle> <id> <pst> ("sn2")
 * ath <ltlTagPoint> <entity> <bone> <ltlLight> <id> <lightType> ("tag0")
 * ath <ltlTagPoint> <entity> <bone> <ltlEntity> <id> <mesh> ("tag1")
 * ath <ltlTagPoint> <entity> <bone> <ltlParticle> <id> <pst> ("tag2")
 */
class _PacExport AthCmd {
public:
  AthCmd();
};


/**
 * dthmo <ltlSceneNode> <mot> <id>
 * dthmo <ltlEntity> <mot> id
 */
class _PacExport Dthmo {
public:
  Dthmo();
};

/**
 * Edit movable object
 * edmo <mot> <movable> ("0")
 */
class _PacExport EdmoCmd {
public:
  EdmoCmd();
};

/**
 * Edit scene node
 */
class _PacExport EdndCmd {
public:
  EdndCmd();
};

}

#endif /* OGRECOMMAND_H */
