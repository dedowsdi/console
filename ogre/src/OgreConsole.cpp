#include "OgreConosle.h"
#include "pacArgHandler.h"
#include "OgreArgHandler.h"
#include "OgreSI.h"
#include "OgreCommand.h"
#include "OgreLight.h"

namespace pac {

//------------------------------------------------------------------------------
OgreConsole::OgreConsole(Ogre::SceneManager* sceneMgr)
    : mSceneMgr(sceneMgr), mMovableDir(0), mNodeDir(0) {}

//------------------------------------------------------------------------------
void OgreConsole::initArghandler() {
  Console::initArghandler();
  initEnumArgHandler();
  initResourceArghandler();
  initMovableArgHandler();
  initNodeArgHandler();
  initStringArgHandler();
  initLiteralArgHandler();
  initTreeArgHandler();
}

//------------------------------------------------------------------------------
void OgreConsole::initCommand() {
  Console::initCommand();

  sgCmdLib.registerCommand(new AthCmd()->init());
  sgCmdLib.registerCommand(new Dthmo()->init());
  sgCmdLib.registerCommand(new EdmoCmd()->init());
  sgCmdLib.registerCommand(new EdndCmd()->init());
  sgCmdLib.registerCommand(new LsmatCmd()->init());
  sgCmdLib.registerCommand(new LsmoCmd()->init());
  sgCmdLib.registerCommand(new Lsnd()->init());
}

//------------------------------------------------------------------------------
void OgreConsole::initDir() {
  Console::initDir();

  sgDirRoot->addChild(new AbsDir("scene", new SceneSI(mSceneMgr)));
  sgDirRoot->addChild(new AbsDir("movable"));
  sgDirRoot->addChild(new AbsDir("node"));
}

//------------------------------------------------------------------------------
void OgreConsole::initEnumArgHandler() {
  // enum
  DEFINE_ENUM_CONVERSION(
      Ogre::Light::, LightTypes, (LT_POINT)(LT_DIRECTIONAL)(LT_SPOTLIGHT))
  sgArgLib.registerArgHandler(
      new EnumArgHandler<Ogre::Light::LightType>("en_lightType"));

  DEFINE_ENUM_CONVERSION(
      Ogre::, PolygonMode, (PM_POINTS)(PM_WIREFRAME)(PM_SOLID))
  sgArgLib.registerArgHandler(
      new EnumArgHandler<Ogre::PolygonMode>("en_polygonMode"));

  DEFINE_ENUM_CONVERSION(
      Ogre::, ShadowTechnique,
      (SHADOWDETAILTYPE_ADDITIVE)(SHADOWDETAILTYPE_MODULATIVE)(
          SHADOWDETAILTYPE_INTEGRATED)(SHADOWDETAILTYPE_STENCIL)(
          SHADOWDETAILTYPE_TEXTURE)(SHADOWTYPE_STENCIL_MODULATIVE)(
          SHADOWTYPE_STENCIL_ADDITIVE)(SHADOWTYPE_TEXTURE_MODULATIVE)(
          SHADOWTYPE_TEXTURE_ADDITIVE)(SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED)(
          SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED));
  sgArgLib.registerArgHandler(
      new EnumArgHandler<Ogre::ShadowTenique>("en_shadowTenique"));

  DEFINE_ENUM_CONVERSION(
      Ogre::, FogMode, (FOG_NONE)(FOG_EXP)(FOG_EXP2)(FOG_LINEAR))
  sgArgLib.registerArgHandler(new EnumArgHandler<Ogre::FogMode>("en_fogMode"));

  DEFINE_ENUM_CONVERSION(
      Ogre::, CullingMode, (CULL_NONE)(CULL_CLOCKWISE)(CULL_ANTICLOCKWISE))
  sgArgLib.registerArgHandler(
      new EnumArgHandler<Ogre::FogMode>("en_cullingMode"));
}

//------------------------------------------------------------------------------
void OgreConsole::initResourceArghandler() {
  // resource
  sgArgLib.regeisterArgHandler(
      new ResourceAh("material", Ogre::MaterialManager::getSingletonPtr());
  sgArgLib.regeisterArgHandler(
      new ResourceAh("mesh", Ogre::MeshManager::getSingletonPtr());
  sgArgLib.regeisterArgHandler(
      new ResourceAh("texture", Ogre::TextureManager::getSingletonPtr());
  sgArgLib.regeisterArgHandler(new ResourceAh("particleSystem",
      Ogre::ParticleSystemManager::getSingletonPtr());
  sgArgLib.regeisterArgHandler(new ResourceAh("compositor",
      Ogre::CompositorManager::getSingletonPtr());
}

//------------------------------------------------------------------------------
void OgreConsole::initMovableArgHandler() {
  // movable
  sgArgLib.registerArgHandler(new MovablelAH());
  sgArgLib.registerArgHandler(new MovableBaseAH("particle", "ParticleSystem"));
  sgArgLib.registerArgHandler(new MovableBaseAH("light", "Light"));
  sgArgLib.registerArgHandler(new MovableBaseAH("camera", "Camera"));
  sgArgLib.registerArgHandler(new MovableBaseAH("entity", "Entity"));

}

//------------------------------------------------------------------------------
void OgreConsole::initNodeArgHandler()
{
  sgArgLib.registerArgHandler(new SceneNodeAH());
  sgArgLib.registerArgHandler(new BoneAH());
}

//------------------------------------------------------------------------------
void OgreConsole::initStringArgHandler() {
  // mot
  StringArgHandler* moType = new StringArgHandler("moType");
  moType->sddString("ParticleSystem");
  moType->addString("Light");
  moType->addString("Camera");
  moType->addString("Entity");
  sgArgLib.registerArgHandler(moType);


  // particle related
  StringArgHandler forceApp =
      new StringArgHandler("affector_force_application");
  forceApp.insert("average");
  forceApp.insert("add");
  sgArgLib.registerArgHandler(forceApp);
}

//------------------------------------------------------------------------------
void OgreConsole::initLiteralArgHandler() {
  // literal
  sgArgLib.registerArgHandler(new LiteralArgHandler("sceneNode"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("tagPoint"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("light"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("entity"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("particle"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("direct"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("all"));
}

//------------------------------------------------------------------------------
void OgreConsole::initTreeArgHandler() {
  // scenemanager related
  TreeArgHandler* fog = new TreeArgHandler("fog");
  root = fog->getRoot();
  // fogMode colorValue expDensity linearStart linearEnd
  root->acn("fogMode")
      ->acn("colour", "real4")
      ->acn("expDensity", "real")
      ->acn("linearStart", "real")
      ->acn("linearEnd", "real")
      ->eb("0");
  sgArgLib.registerArgHandler(fog);
}
}
