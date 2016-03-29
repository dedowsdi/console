#ifndef OGREUTIL_H
#define OGREUTIL_H

#include "pacConsolePreRequisite.h"
#include "Ogre.h"
namespace pac {

class OgreUtil {
public:
  static Ogre::IdType nid;
  static Ogre::IdType getIdFromNameid(const std::string& nameid);
  static std::string createNameid(const Ogre::Node* node);
  static std::string createNameid(const Ogre::MovableObject* mo);
  static Ogre::SceneNode* getSceneNodeById(
      Ogre::SceneManager* mgr, const std::string& id);
  static Ogre::SceneNode* getSceneNodeById(
      Ogre::SceneManager* mgr, Ogre::IdType id);
  static Ogre::SceneNode* getSceneNodeByIdNoThrow(
      Ogre::SceneManager* mgr, Ogre::IdType id);

  static std::tuple<Ogre::IdType, std::string> parseIdtype(
      const std::string& it);
  static std::string createIdtype(Ogre::MovableObject* mo);
  static Ogre::MovableObject* getMovableByIdtype(
      Ogre::SceneManager* mgr, const std::string& it);
  static Ogre::MovableObject* getMovableByIdtype(
      Ogre::SceneManager* mgr, Ogre::IdType id, const std::string& type);
  static Ogre::MovableObject* getMovableByIdtypeNoThrow(
      Ogre::SceneManager* mgr, Ogre::IdType id, const std::string& type);

  static Ogre::SceneMemoryMgrTypes getSceneType(const Ogre::Node* node);

  /**
   * destroy scenenode, all it's children, and all the movable object attached
   * to them
   * @param node : target scenenode
   */
  static void destroySceneNodeTotally(Ogre::SceneNode* node);

  static bool hasSceneNode(Ogre::SceneManager* mgr, const std::string& name);

  static bool hasNode(Ogre::Node* node, const std::string& name);

  static bool hasMovableObject(Ogre::SceneManager* mgr, const std::string& name,
      const std::string& type);

private:
  OgreUtil(){};
};
}

#endif /* OGREUTIL_H */
