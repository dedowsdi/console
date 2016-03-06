#ifndef OGREPARTICLESYSTEMSI_H
#define OGREPARTICLESYSTEMSI_H

#include "OgreConsolePreRequisite.h"
#include "OgreSiWrapper.h"

namespace pac {

class _PacExport ParticleSystemSI : public OgreSiWrapper {
public:
  ParticleSystemSI(Ogre::ParticleSystem* ps);

private:
  void bindArgHandlers();
};

class _PacExport ParticleEmitterSI : public OgreSiWrapper {
public:
  ParticleEmitterSI(Ogre::ParticleEmitter* emitter);

private:
  void bindArgHandlers();
};

class _PacExport ParticleAffectorSI : public OgreSiWrapper {
public:
  ParticleAffectorSI(Ogre::ParticleAffector* affector);

private:
  void bindArgHandlers();
};


class _PacExport ParticleRendererSI : public OgreSiWrapper {
public:
  ParticleRendererSI(Ogre::ParticleSystemRenderer* renderer);

private:
  void bindArgHandlers();
};
}

#endif /* OGREPARTICLESYSTEMSI_H */
