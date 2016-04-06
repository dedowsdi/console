#ifndef OGREARGHANDLER_H
#define OGREARGHANDLER_H

#include "OgreConsolePreRequisite.h"
#include "pacIntrinsicArgHandler.h"
#include "OgreId.h"
#include "OgreUtil.h"
#include <OgreMovableObject.h>
#include <OgreItem.h>
#include <OgreCamera.h>

namespace pac {

/**
 * movable, must follow moType. It will search back parentSceneNode and
 * parentEntity, parentBone to filter result.
 */
class _PacExport MovableAH : public ArgHandler {
public:
  /**
   * movable arg handler
   * @param name : ah name
   * @param moType : movable type, if you set this, this handler will ignore
   * previous motype node
   * @return :
   */
  MovableAH(const std::string& name = "movable", const std::string& moType = "",
      bool attachedOnly = false);
  virtual ArgHandler* clone() { return new MovableAH(*this); }

protected:
  virtual void populatePromptBuffer(const std::string& s);
  virtual bool doValidate(const std::string& s);
  /**
   * get id@moType
   */
  virtual std::string getUniformValue() const;

  void runtimeInit();
  virtual void onLinked(Node* grandNode);

private:
  template <typename T>
  void populateFromOgreIterator(T oi, const std::string& s) {
    while (oi.hasMoreElements()) {
      auto mo = oi.getNext();
      const std::string&& nameid = OgreUtil::createNameid(mo);
      if ((!mAttachedOnly || mo->isAttached()) &&
          (s.empty() || StringUtil::startsWith(nameid, s)))
        appendPromptBuffer(nameid);
    }
  }

private:
  bool mFixType, mAttachedOnly;
  Ogre::Item* mItem;
  Ogre::SceneNode* mSceneNode;
  Node* mItemNode, *mBoneNode, *mSnNode, *mMoTypeNode;
  std::string mMoType, mBone;
};

/**
 *  moType movable
 *  t_sceneNode moType movable
 *  entity moType movable
 *  entity bone moType movable
 *
 * all the way to locate a movable, moType only exists if moAhName is movable
 */
class _PacExport MovableTH : public TreeArgHandler {
public:
  MovableTH(const std::string& name, const std::string& moAhName);
  virtual ArgHandler* clone() { return new MovableTH(*this); }
  Ogre::MovableObject* getMovableObject() const;
  virtual std::string getUniformValue() const;

protected:
  std::string mMoAhName;
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
  Ogre::Item* mItem;
  Node* mItemNode;
};

/**
 * name is only name, not identity in ogre2.0, so i combine namd and id with
 * '@', name@id
 * might follow en_smmt, parentNode
 */
class _PacExport SceneNodeAH : public ArgHandler {
public:
  SceneNodeAH(const std::string& name = "sceneNode", bool includeRoot = true);
  virtual ArgHandler* clone() { return new SceneNodeAH(*this); }
  virtual std::string getUniformValue() const;
  Ogre::SceneNode* getSceneNode();

protected:
  virtual void populatePromptBuffer(const std::string& s);
  virtual bool doValidate(const std::string& s);
  /**
   * Don't have access to scene nodes list in scene manager, have to do it
   * manually
   */
  void loopNode(Ogre::Node* sceneNode, const std::string& s);

  virtual void onLinked(Node* grandNode);
  virtual void runtimeInit();

private:
  bool mIncludeRoot;
  Node* mParentSnNode, *mAncestorSnNode, *mSmmtNode;
  Ogre::SceneNode* mParentSceneNode, *mAncestorSceneNode;
  int mSmmt;
};

/**
 *  sceneNode ("0")
 *  en_smmt sceneNode ("1")
 *  ltl_childOfNode parentSceneNode sceneNode ("2")
 *  ltl_childOfNode parentSceneNode en_smmt sceneNode ("3")
 *  ltl_descendantOfNode ancestorSceneNode sceneNode ("4")
 *  ltl_descendantOfNode ancestorSceneNode en_smmt sceneNode ("5")
 *  ltl_parentOfNode childNode ("ps0")
 *  ltl_parentOfMovalbe moType childMovable ("pm0")
 *
 *  all the way to locate a scene node
 */
class _PacExport SceneNodeTH : public TreeArgHandler {
public:
  SceneNodeTH();
  virtual ArgHandler* clone() { return new SceneNodeTH(*this); }
  Ogre::SceneNode* getSceneNode();
  virtual std::string getUniformValue() const;
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

class _PacExport DatablockAH : public ArgHandler {
public:
  DatablockAH(Ogre::HlmsManager* mgr);
  virtual ArgHandler* clone() { return new DatablockAH(*this); }

protected:
  virtual void populatePromptBuffer(const std::string& s);
  virtual bool doValidate(const std::string& s);

protected:
  Ogre::HlmsManager* mManager;
};

/**
 * real3 ("0")
 * ltl_posOfNode t_sceneNode ("1")
 * all the way to locate a position
 */
class _PacExport PositionTH : public TreeArgHandler {
public:
  PositionTH();
  std::string getUniformValue() const;
};

/**
 * real3 ("0")
 * ltl_posOfNode t_sceneNode ("1" , negative-z of  node)
 * all the way to locate a direction 
 */
class _PacExport DirectionTH : public TreeArgHandler {
public:
  DirectionTH();
  std::string getUniformValue() const;
};
}

#endif /* OGREARGHANDLER_H */
