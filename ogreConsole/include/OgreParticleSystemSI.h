#ifndef OGREPARTICLESYSTEMSI_H
#define OGREPARTICLESYSTEMSI_H

#include "OgreConsolePreRequisite.h"
#include "OgreSiWrapper.h"

namespace pac {

class _PacExport ParticleSystemSI : public OgreSiWrapper {
public:
  ParticleSystemSI(Ogre::ParticleSystem* ps);

  virtual void onCreateDir(AbsDir* dir);

  Ogre::ParticleSystem* getParticleSystem() const;

private:
  void bindArgHandlers();
};

class _PacExport ParticleEmitterSI : public OgreSiWrapper {
public:
  ParticleEmitterSI(Ogre::ParticleEmitter* emitter);

  Ogre::ParticleEmitter* getParticleEmitter();

private:
  void bindArgHandlers();
};

class _PacExport ParticleAffectorSI : public OgreSiWrapper {
public:
  ParticleAffectorSI(Ogre::ParticleAffector* affector);

  Ogre::ParticleAffector* getParticleAffector() const;

private:
  void bindArgHandlers();
};

class _PacExport ParticleRendererSI : public OgreSiWrapper {
public:
  ParticleRendererSI(Ogre::ParticleSystemRenderer* renderer);

  Ogre::ParticleSystemRenderer* getParticleSystemRenderer() const;

private:
  void bindArgHandlers();
};
}

#endif /* OGREPARTICLESYSTEMSI_H */
