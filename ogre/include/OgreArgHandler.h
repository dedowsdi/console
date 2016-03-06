#ifndef OGREARGHANDLER_H
#define OGREARGHANDLER_H

#include "OgreConsolePreRequisite.h"
#include "pacIntrinsicArgHandler.h"

namespace pac {

/**
 * movable, must follow moType. It will search back sceneNode and entity bone to
 * filter result.
 */
class _PacExport MovableAH : public ArgHandler {
public:
  MovableAH();

protected:
  virtual void populatePromptBuffer(const std::string& s);
  virtual bool doValidate(const std::string& s);
  void runtimeInit();
  virtual void onLinked(Node* grandNode);

private:
  Ogre::Entity* mEntity;
  Ogre::SceneNode* mSceneNode;
  Node* mEntityNode, mBoneNode, mSnNode, mMoTypeNode;
};

/**
 * bone, must follow entity? or skeleton ?
 */
class _PacExport BoneAH : public ArgHandler {
public:
  BoneAH();

protected:
  virtual void populatePromptBuffer(const std::string& s);
  virtual bool doValidate(const std::string& s);
  void runtimeInit();
  virtual void onLinked(Node* grandNode);

protected:
  Ogre::Entity* mEntity;
  Node* mEntityNode;
};

/**
 * base clase of movable like particle, light, camera, entity
 */
class _PacExport MovableBaseAH : public ArgHandler {
public:
  /**
   * @param name : handler name
   * @return :
   */
  MovableBaseAH(const std::string& name, const std::string& moType);

protected:
  virtual void populatePromptBuffer(const std::string& s);
  virtual bool doValidate(const std::string& s);
};

class _PacExport SceneNodeAH : public ArgHandler {
public:
  SceneNodeAH();

protected:
  virtual void populatePromptBuffer(const std::string& s);
  virtual bool doValidate(const std::string& s);
  /**
   * Don't have access to scene nodes list in scene manager, have to do it
   * manually
   */
  void loopNode(Ogre::SceneNode* sceneNode, const std::string& s);
};

class _PacExport ResourceAh : public ArgHandler {
public:
  ResourceAh(const std::string& name, Ogre::ResourceManager* rm);

protected:
  virtual void populatePromptBuffer(const std::string& s);
  virtual bool doValidate(const std::string& s);

protected:
  Ogre::ResourceManager* mResourceMgr;
};
}

#endif /* OGREARGHANDLER_H */
