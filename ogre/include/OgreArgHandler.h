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
  MovableAH(const std::string& name, const std::string& moType = "");
  virtual void populatePromptBuffer(const std::string& s);
  virtual bool doValidate(const std::string& s);

protected:
  void runtimeInit();

private:
  std::string mMovableType;
  Ogre::Entity* mEntity;
  std::string mBone;
  Ogre::SceneNode* mSceneNode;
};

class _PacExport SceneNodeAH : public ArgHandler {
public:
  SceneNodeAH();

protected:
  virtual void populatePromptBuffer(const std::string& s);
  virtual bool doValidate(const std::string& s);
};

class _PacExport MeshAH : public ArgHandler {
public:
  MeshAH();

protected:
  virtual void populatePromptBuffer(const std::string& s);
  virtual bool doValidate(const std::string& s);
};
}

#endif /* OGREARGHANDLER_H */
