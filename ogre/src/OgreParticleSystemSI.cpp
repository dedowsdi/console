#include "pacStable.h"
#include "pacAbsDir.h"
#include "OgreParticleSystemSI.h"
#include "OgreParticleSystemRenderer.h"
#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>
#include <OgreParticleAffector.h>
#include <OgreParticleSystemRenderer.h>
#include <OgreStringConverter.h>

namespace pac {

//------------------------------------------------------------------------------
ParticleSystemSI::ParticleSystemSI(Ogre::ParticleSystem* ps)
    : OgreSiWrapper("ParticleSystem", ps) {
  if (createaAhDict()) bindArgHandlers();
}

//------------------------------------------------------------------------------
void ParticleSystemSI::onCreateDir(AbsDir* dir) {
  Ogre::ParticleSystem* ps = getParticleSystem();
  //add emitters
  for (size_t i = 0; i < ps->getNumEmitters(); i++) {
    Ogre::ParticleEmitter* emitter = ps->getEmitter(i);
    dir->addChild(new AbsDir(
        emitter->getType() + "_" + Ogre::StringConverter::toString(i),
        new ParticleEmitterSI(emitter)));
  }

  //add affectors
  for (size_t i = 0; i < ps->getNumAffectors(); i++) {
    Ogre::ParticleAffector* affector = ps->getAffector(i);
    dir->addChild(new AbsDir(
        affector->getType() + "_" + Ogre::StringConverter::toString(i),
        new ParticleAffectorSI(affector)));
  }

  // add renderer
  if (ps->getRenderer()) {
    Ogre::ParticleSystemRenderer* renderer = ps->getRenderer();
    dir->addChild(
        new AbsDir(renderer->getType(), new ParticleRendererSI(renderer)));
  }

  // sort by name
  // std::sort(mChildren.begin(), mChildren.end(),
  //[=](AbstractDir* pA, AbstractDir* pB)
  //-> bool { return pA->getName() < pB->getName(); });
}

//------------------------------------------------------------------------------
Ogre::ParticleSystem* ParticleSystemSI::getParticleSystem() const
{
  return static_cast<Ogre::ParticleSystem*>(mOgreSI);
}

//------------------------------------------------------------------------------
void ParticleSystemSI::bindArgHandlers() { bindArgHandlerByType(); }

//------------------------------------------------------------------------------
ParticleEmitterSI::ParticleEmitterSI(Ogre::ParticleEmitter* emitter)
    : OgreSiWrapper(emitter->getType(), emitter) {
  if (createaAhDict()) bindArgHandlers();
}

//------------------------------------------------------------------------------
Ogre::ParticleEmitter* ParticleEmitterSI::getParticleEmitter() {
  return static_cast<Ogre::ParticleEmitter*>(mOgreSI);
}

//------------------------------------------------------------------------------
void ParticleEmitterSI::bindArgHandlers() {
  bindArgHandlerByType();
  mAhDict->bindArgHandler("material", "material");
}

//------------------------------------------------------------------------------
ParticleAffectorSI::ParticleAffectorSI(Ogre::ParticleAffector* affector)
    : OgreSiWrapper(affector->getType(), affector) {
  if (createaAhDict()) {
    bindArgHandlers();
  }
}

//------------------------------------------------------------------------------
Ogre::ParticleAffector* ParticleAffectorSI::getParticleAffector() const {
  return static_cast<Ogre::ParticleAffector*>(mOgreSI);
}

//------------------------------------------------------------------------------
void ParticleAffectorSI::bindArgHandlers() {
  bindArgHandlerByType();
  if (mName == "LinearForceAffector") {
    mAhDict->bindArgHandler("force_application", "affector_force_application");

  } else if (mName == "ColourImageAffector") {
    mAhDict->bindArgHandler("image", "texture");
  }
}

//------------------------------------------------------------------------------
ParticleRendererSI::ParticleRendererSI(Ogre::ParticleSystemRenderer* renderer)
    : OgreSiWrapper(renderer->getType(), renderer) {
  if (createaAhDict()) bindArgHandlers();
}

//------------------------------------------------------------------------------
Ogre::ParticleSystemRenderer* ParticleRendererSI::getParticleSystemRenderer()
    const {
  return static_cast<Ogre::ParticleSystemRenderer*>(mOgreSI);
}

//------------------------------------------------------------------------------
void ParticleRendererSI::bindArgHandlers() { bindArgHandlerByType(); }
}
