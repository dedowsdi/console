#include "OgreConsole.h"
#include "OgreArgHandler.h"
#include "OgreSI.h"
#include "OgreCommand.h"
#include "OgreLight.h"
#include "pacArgHandler.h"
#include "pacAbsDir.h"
#include "pacEnumUtil.h"
#include <OgreMaterialManager.h>
#include <OgreMeshManager.h>
#include <OgreTextureManager.h>
#include <OgreParticleSystemManager.h>
//#include <OgreCompositorManager.h>
#include <OgreNode.h>

namespace pac {

// define enum conversions
DEFINE_ENUM_CONVERSION(
    Ogre::Light::, LightTypes, (LT_POINT)(LT_DIRECTIONAL)(LT_SPOTLIGHT))
DEFINE_ENUM_CONVERSION(Ogre::, PolygonMode, (PM_POINTS)(PM_WIREFRAME)(PM_SOLID))
// DEFINE_ENUM_CONVERSION(
// Ogre::, ShadowTechnique,
//(SHADOWDETAILTYPE_ADDITIVE)(SHADOWDETAILTYPE_MODULATIVE)(
// SHADOWDETAILTYPE_INTEGRATED)(SHADOWDETAILTYPE_STENCIL)(
// SHADOWDETAILTYPE_TEXTURE)(SHADOWTYPE_STENCIL_MODULATIVE)(
// SHADOWTYPE_STENCIL_ADDITIVE)(SHADOWTYPE_TEXTURE_MODULATIVE)(
// SHADOWTYPE_TEXTURE_ADDITIVE)(SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED)(
// SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED));
DEFINE_ENUM_CONVERSION(
    Ogre::, FogMode, (FOG_NONE)(FOG_EXP)(FOG_EXP2)(FOG_LINEAR))
DEFINE_ENUM_CONVERSION(
    Ogre::, CullingMode, (CULL_NONE)(CULL_CLOCKWISE)(CULL_ANTICLOCKWISE))
DEFINE_ENUM_CONVERSION(
    Ogre::Node::, TransformSpace, (TS_LOCAL)(TS_PARENT)(TS_WORLD))
DEFINE_ENUM_CONVERSION(
    Ogre::, SceneMemoryMgrTypes, (SCENE_DYNAMIC)(SCENE_STATIC))

//------------------------------------------------------------------------------
OgreConsole::OgreConsole(ConsoleUI* ui, Ogre::SceneManager* sceneMgr)
    : Console(ui), mSceneMgr(sceneMgr), mMovableDir(0), mNodeDir(0) {}

//------------------------------------------------------------------------------
void OgreConsole::initArghandler() {
  Console::initArghandler();
  initEnumArgHandler();
  initResourceArghandler();
  initStringArgHandler();
  initLiteralArgHandler();
  initTreeArgHandler();
  initNodeArgHandler();
  initMovableArgHandler();
}

//------------------------------------------------------------------------------
void OgreConsole::initCommand() {
  Console::initCommand();

  sgCmdLib.registerCommand(new AthCmd());
  sgCmdLib.registerCommand(new DthCmd());
  sgCmdLib.registerCommand(new EdmoCmd());
  sgCmdLib.registerCommand(new LsresCmd());
  sgCmdLib.registerCommand(new LsmoCmd());
  sgCmdLib.registerCommand(new LsndCmd());
  sgCmdLib.registerCommand(new AdndCmd());
  sgCmdLib.registerCommand(new EdndCmd());
  sgCmdLib.registerCommand(new RmndCmd());
}

//------------------------------------------------------------------------------
void OgreConsole::initDir() {
  Console::initDir();

  mSceneDir = new AbsDir("scene", new SceneManagerSI(mSceneMgr));
  mMovableDir = new AbsDir("movable");
  mNodeDir = new AbsDir("node");

  sgRootDir.addChild(mSceneDir);
  sgRootDir.addChild(mMovableDir);
  sgRootDir.addChild(mNodeDir);
}

//------------------------------------------------------------------------------
void OgreConsole::initEnumArgHandler() {
  // enum
  sgArgLib.registerArgHandler(
      new EnumArgHandler<Ogre::Light::LightTypes>("en_lightType"));
  sgArgLib.registerArgHandler(
      new EnumArgHandler<Ogre::PolygonMode>("en_polygonMode"));
  // sgArgLib.registerArgHandler(
  // new EnumArgHandler<Ogre::ShadowTechnique>("en_shadowTenique"));
  sgArgLib.registerArgHandler(new EnumArgHandler<Ogre::FogMode>("en_fogMode"));
  sgArgLib.registerArgHandler(
      new EnumArgHandler<Ogre::FogMode>("en_cullingMode"));
  sgArgLib.registerArgHandler(
      new EnumArgHandler<Ogre::Node::TransformSpace>("en_transformSpace"));
  sgArgLib.registerArgHandler(
      new EnumArgHandler<Ogre::SceneMemoryMgrTypes>("en_smmt"));
}

//------------------------------------------------------------------------------
void OgreConsole::initResourceArghandler() {
  // resource
  sgArgLib.registerArgHandler(
      new ResourceAH("material", Ogre::MaterialManager::getSingletonPtr()));
  sgArgLib.registerArgHandler(new PassiveResourceAH(
      "mesh", Ogre::MeshManager::getSingletonPtr(), {"mesh"}));
  sgArgLib.registerArgHandler(new PassiveResourceAH("texture",
      Ogre::TextureManager::getSingletonPtr(), {"png", "jpg", "jpeg", "dds"}));
  sgArgLib.registerArgHandler(new ParticleSystemTemplateAH(
      Ogre::ParticleSystemManager::getSingletonPtr()));
  // sgArgLib.registerArgHandler(
  // new ResourceAH("compositor", Ogre::CompositorManager::getSingletonPtr()));
}

//------------------------------------------------------------------------------
void OgreConsole::initMovableArgHandler() {
  // movable
  sgArgLib.registerArgHandler(new MovableAH());
  sgArgLib.registerArgHandler(new MovableAH("particle", "ParticleSystem"));
  sgArgLib.registerArgHandler(new MovableAH("light", "Light"));
  sgArgLib.registerArgHandler(new MovableAH("camera", "Camera"));
  sgArgLib.registerArgHandler(new MovableAH("entity", "Entity"));
  sgArgLib.registerArgHandler(new MovableTH("t_movable", "movable"));
  sgArgLib.registerArgHandler(new MovableTH("t_light", "light"));
  sgArgLib.registerArgHandler(new MovableTH("t_camera", "camera"));
  sgArgLib.registerArgHandler(new MovableTH("t_entity", "entity"));
  sgArgLib.registerArgHandler(new MovableTH("t_particle", "particle"));
}

//------------------------------------------------------------------------------
void OgreConsole::initNodeArgHandler() {
  sgArgLib.registerArgHandler(new SceneNodeAH());
  sgArgLib.registerArgHandler(new SceneNodeTH());
  sgArgLib.registerArgHandler(new BoneAH());
}

//------------------------------------------------------------------------------
void OgreConsole::initStringArgHandler() {
  StringArgHandler* moType = new StringArgHandler(
      "moType", {"ParticleSystem", "Light", "Camera", "Entity"});
  sgArgLib.registerArgHandler(moType);

  StringArgHandler* forceApp =
      new StringArgHandler("affector_force_application", {"average", "add"});
  sgArgLib.registerArgHandler(forceApp);

  StringArgHandler* resType = new StringArgHandler(
      "resType", {"material", "mesh", "texture", "compositor", "pst"});
  sgArgLib.registerArgHandler(resType);
}

//------------------------------------------------------------------------------
void OgreConsole::initLiteralArgHandler() {
  // literal
  sgArgLib.registerArgHandler(new LiteralArgHandler("all"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("ancestor"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("camera"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("childOnly"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("direct"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("entity"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("light"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("parent"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("particle"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("sceneNode"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("tagPoint"));
}

//------------------------------------------------------------------------------
void OgreConsole::initTreeArgHandler() {
  // scenemanager related
  TreeArgHandler* handler = new TreeArgHandler("handler");
  Node* root = handler->getRoot();
  // fogMode colorValue expDensity linearStart linearEnd
  root->acn("en_fogMode")
      ->acn("colour", "real4")
      ->acn("expDensity", "real")
      ->acn("linearStart", "real")
      ->acn("linearEnd", "real")
      ->eb("0");
  sgArgLib.registerArgHandler(handler);

  // node related
  handler = new TreeArgHandler("ypr");  // ypr as yaw pitch roll
  root = handler->getRoot();
  root->acn("en_transformSpace")->acn("real")->eb("0");
  sgArgLib.registerArgHandler(handler);
}
}
