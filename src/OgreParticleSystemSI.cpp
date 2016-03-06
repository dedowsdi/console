#include "pacStable.h"
#include "OgreParticleSystemSI.h"
#include "Ogre.h"
#include "OgreParticleSystemRenderer.h"

namespace pac {

//------------------------------------------------------------------------------
ParticleSystemSI::ParticleSystemSI(Ogre::ParticleSystem* ps)
    : OgreSiWrapper("ParticleSystem", ps) {
  if (createaAhDict()) bindArgHandlers();
}

//------------------------------------------------------------------------------
void ParticleSystemSI::bindArgHandlers() { bindArgHandlerByType(); }

//------------------------------------------------------------------------------
ParticleEmitterSI::ParticleEmitterSI(Ogre::ParticleEmitter* emitter)
    : OgreSiWrapper(emitter->getType(), emitter) {
  if (createaAhDict()) bindArgHandlers();
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
void ParticleRendererSI::bindArgHandlers() { bindArgHandlerByType(); }
}
