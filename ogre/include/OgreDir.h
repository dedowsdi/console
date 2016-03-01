#ifndef OGREDIR_H
#define OGREDIR_H

#include "OgreConsolePreRequisite.h"
#include "pacAbsDir.h"
#include "OgreMovableObject.h"

namespace pac {



/**
 * Used to contain external stringinterface to ogre movable object
 */
class _PacExport MolDir : public AbsDir, public Ogre::MovableObject::Listener {
public:
  MolDir();
  ~MolDir();

  virtual void objectDestroyed(Ogre::MovableObject*) { delete this; }
};
}

#endif /* OGREDIR_H */
