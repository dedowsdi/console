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
  virtual ArgHandler* clone() { return new MovableAH(*this); }

protected:
  virtual void populatePromptBuffer(const std::string& s);
  virtual bool doValidate(const std::string& s);
  void runtimeInit();
  virtual void onLinked(Node* grandNode);

private:
  Ogre::Entity* mEntity;
  Ogre::SceneNode* mSceneNode;
  std::string mMoType, mBone;
  Node* mEntityNode, *mBoneNode, *mSnNode, *mMoTypeNode;
};

/**
 * bone, must follow entity? or skeleton ?
 */
class _PacExport BoneAH : public ArgHandler {
public:
  BoneAH();
  virtual ArgHandler* clone() { return new BoneAH(*this); }

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
   * ctor of specific movable type
   * @param name : name
   * @param moType : movable object type
   */
  MovableBaseAH(const std::string& name, const std::string& moType);
  virtual ArgHandler* clone() { return new MovableBaseAH(*this); }

protected:
  virtual void populatePromptBuffer(const std::string& s);
  virtual bool doValidate(const std::string& s);

protected:
  std::string mMoType;
};

class _PacExport SceneNodeAH : public ArgHandler {
public:
  SceneNodeAH();
  virtual ArgHandler* clone() { return new SceneNodeAH(*this); }

protected:
  virtual void populatePromptBuffer(const std::string& s);
  virtual bool doValidate(const std::string& s);
  /**
   * Don't have access to scene nodes list in scene manager, have to do it
   * manually
   */
  void loopNode(Ogre::Node* sceneNode, const std::string& s);
};

/**
 * deal with material, compositor
 */
class _PacExport ResourceAH : public ArgHandler {
public:
  ResourceAH(const std::string& name, Ogre::ResourceManager* rm);
  virtual ArgHandler* clone() { return new ResourceAH(*this); }

protected:
  virtual void populatePromptBuffer(const std::string& s);
  virtual bool doValidate(const std::string& s);

protected:
  Ogre::ResourceManager* mResourceMgr;
};

/**
 * deal with mesh, texture. Only loaded mesh and texture appears in it's
 * manager, but i need to get the unloaded ones too.
 */
class _PacExport PassiveResourceAH : public ArgHandler {
public:
  PassiveResourceAH(const std::string& name, Ogre::ResourceManager* rm,
      std::initializer_list<std::string> exts);
  virtual ArgHandler* clone() { return new PassiveResourceAH(*this); }
  StringSet::const_iterator beginResourceIter() const;
  StringSet::const_iterator endResourceIter() const;

protected:
  virtual void populatePromptBuffer(const std::string& s);
  virtual bool doValidate(const std::string& s);

protected:
  Ogre::ResourceManager* mResourceMgr;
  StringSet mResources;  // resources on disk
};

class _PacExport ParticleSystemTemplateAH : public ArgHandler {
public:
  ParticleSystemTemplateAH(Ogre::ParticleSystemManager* pm);
  virtual ArgHandler* clone() { return new ParticleSystemTemplateAH(*this); }

protected:
  virtual void populatePromptBuffer(const std::string& s);
  virtual bool doValidate(const std::string& s);

protected:
  Ogre::ParticleSystemManager* mManager;
};
}

#endif /* OGREARGHANDLER_H */
