#ifndef OGREARGHANDLER_H
#define OGREARGHANDLER_H

#include "pacIntrinsicArgHandler.h"

namespace pac {

class _PacExport MovableAH : public StringArgHandler {
public:
  MovableAH(const std::string& name, const std::string& mot = "");

protected:
  void runtimeInit();
  std::string mMovableType;
};

class _PacExport SceneNodeAH : public StringArgHandler {
public:
  SceneNodeAH();
};

class _PacExport MeshAH : public StringArgHandler {
public:
  MeshAH();
};

}

#endif /* OGREARGHANDLER_H */
