#include "OgreConsole.h"
#include "OgreArgHandler.h"
#include "OgreSI.h"
#include "OgreCommand.h"
#include "OgreLight.h"
#include "pacArgHandler.h"
#include "pacAbsDir.h"
#include "pacEnumUtil.h"
#include "OgreMaterialManager.h"
#include "OgreMeshManager.h"
#include "OgreTextureManager.h"
#include "OgreParticleSystemManager.h"
#include "OgreCompositorManager.h"

namespace pac {

// define enum conversions
DEFINE_ENUM_CONVERSION(
    Ogre::Light::, LightTypes, (LT_POINT)(LT_DIRECTIONAL)(LT_SPOTLIGHT))
DEFINE_ENUM_CONVERSION(Ogre::, PolygonMode, (PM_POINTS)(PM_WIREFRAME)(PM_SOLID))
DEFINE_ENUM_CONVERSION(
    Ogre::, ShadowTechnique,
    (SHADOWDETAILTYPE_ADDITIVE)(SHADOWDETAILTYPE_MODULATIVE)(
        SHADOWDETAILTYPE_INTEGRATED)(SHADOWDETAILTYPE_STENCIL)(
        SHADOWDETAILTYPE_TEXTURE)(SHADOWTYPE_STENCIL_MODULATIVE)(
        SHADOWTYPE_STENCIL_ADDITIVE)(SHADOWTYPE_TEXTURE_MODULATIVE)(
        SHADOWTYPE_TEXTURE_ADDITIVE)(SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED)(
        SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED));
DEFINE_ENUM_CONVERSION(
    Ogre::, FogMode, (FOG_NONE)(FOG_EXP)(FOG_EXP2)(FOG_LINEAR))
DEFINE_ENUM_CONVERSION(
    Ogre::, CullingMode, (CULL_NONE)(CULL_CLOCKWISE)(CULL_ANTICLOCKWISE))

//------------------------------------------------------------------------------
OgreConsole::OgreConsole(UiConsole* ui, Ogre::SceneManager* sceneMgr)
    : Console(ui), mSceneMgr(sceneMgr), mMovableDir(0), mNodeDir(0) {}

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

  sgCmdLib.registerCommand(new AthCmd());
  sgCmdLib.registerCommand(new DthCmd());
  sgCmdLib.registerCommand(new EdmoCmd());
  sgCmdLib.registerCommand(new EdndCmd());
  sgCmdLib.registerCommand(new LsmatCmd());
  sgCmdLib.registerCommand(new LsmoCmd());
  sgCmdLib.registerCommand(new Lsnd());
}

//------------------------------------------------------------------------------
void OgreConsole::initDir() {
  Console::initDir();

  sgRootDir.addChild(new AbsDir("scene", new SceneManagerSI(mSceneMgr)));
  sgRootDir.addChild(new AbsDir("movable"));
  sgRootDir.addChild(new AbsDir("node"));
}

//------------------------------------------------------------------------------
void OgreConsole::initEnumArgHandler() {
  // enum
  sgArgLib.registerArgHandler(
      new EnumArgHandler<Ogre::Light::LightTypes>("en_lightType"));

  sgArgLib.registerArgHandler(
      new EnumArgHandler<Ogre::PolygonMode>("en_polygonMode"));

  sgArgLib.registerArgHandler(
      new EnumArgHandler<Ogre::ShadowTechnique>("en_shadowTenique"));

  sgArgLib.registerArgHandler(new EnumArgHandler<Ogre::FogMode>("en_fogMode"));

  sgArgLib.registerArgHandler(
      new EnumArgHandler<Ogre::FogMode>("en_cullingMode"));
}

//------------------------------------------------------------------------------
void OgreConsole::initResourceArghandler() {
  // resource
  sgArgLib.registerArgHandler(
      new ResourceAH("material", Ogre::MaterialManager::getSingletonPtr()));
  sgArgLib.registerArgHandler(
      new ResourceAH("mesh", Ogre::MeshManager::getSingletonPtr()));
  sgArgLib.registerArgHandler(
      new ResourceAH("texture", Ogre::TextureManager::getSingletonPtr()));
  sgArgLib.registerArgHandler(new ParticleSystemTemplateAH(
      Ogre::ParticleSystemManager::getSingletonPtr()));

  sgArgLib.registerArgHandler(
      new ResourceAH("compositor", Ogre::CompositorManager::getSingletonPtr()));
}

//------------------------------------------------------------------------------
void OgreConsole::initMovableArgHandler() {
  // movable
  sgArgLib.registerArgHandler(new MovableAH());
  sgArgLib.registerArgHandler(new MovableBaseAH("particle", "ParticleSystem"));
  sgArgLib.registerArgHandler(new MovableBaseAH("light", "Light"));
  sgArgLib.registerArgHandler(new MovableBaseAH("camera", "Camera"));
  sgArgLib.registerArgHandler(new MovableBaseAH("entity", "Entity"));
}

//------------------------------------------------------------------------------
void OgreConsole::initNodeArgHandler() {
  sgArgLib.registerArgHandler(new SceneNodeAH());
  sgArgLib.registerArgHandler(new BoneAH());
}

//------------------------------------------------------------------------------
void OgreConsole::initStringArgHandler() {
  // mot
  StringArgHandler* moType = new StringArgHandler(
      "moType", {"ParticleSystem", "Light", "Camera", "Entity"});
  sgArgLib.registerArgHandler(moType);

  // particle related
  StringArgHandler* forceApp =
      new StringArgHandler("affector_force_application", {"average", "add"});
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
  Node* root = fog->getRoot();
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
