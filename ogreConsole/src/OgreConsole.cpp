#include "OgreConsole.h"
#include "OgreArgHandler.h"
#include "OgreSI.h"
#include "OgreCommand.h"
#include "OgreLight.h"
#include "pacArgHandler.h"
#include "pacAbsDir.h"
#include "pacEnumUtil.h"
#include <OgreMaterialManager.h>
#include <OgreMeshManager2.h>
#include <OgreTextureManager.h>
#include <OgreParticleSystemManager.h>
#include <OgreSceneManager.h>
//#include <OgreCompositorManager.h>
#include <OgreNode.h>
#include <OgreRoot.h>

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
DEFINE_ENUM_CONVERSION(
    Ogre::, ProjectionType, (PT_ORTHOGRAPHIC)(PT_PERSPECTIVE))
DEFINE_ENUM_CONVERSION(Ogre::, OrientationMode,
    (OR_DEGREE_0)(OR_DEGREE_90)(OR_DEGREE_180)(OR_DEGREE_270)(OR_PORTRAIT)(
                           OR_LANDSCAPERIGHT)(OR_LANDSCAPELEFT))
DEFINE_ENUM_CONVERSION(pac::, AXIS,
    (AXIS_X)(AXIS_Y)(AXIS_Z)(AXIS_NEGATIVE_X)(AXIS_NEGATIVE_Y)(AXIS_NEGATIVE_Z))
//------------------------------------------------------------------------------
OgreConsole::OgreConsole(ConsoleUI* ui, Ogre::SceneManager* sceneMgr)
    : Console(ui), mSceneMgr(sceneMgr), mMovableDir(0), mNodeDir(0) {}

//------------------------------------------------------------------------------
bool OgreConsole::execute(const std::string& cmdLine /*= ""*/) {
  return Console::execute(cmdLine);
}

//------------------------------------------------------------------------------
void OgreConsole::initArghandler() {
  Console::initArghandler();
  initEnumArgHandler();
  initResourceArghandler();
  initStringArgHandler();
  initLiteralArgHandler();
  initMiscArgHandler();
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
  sgCmdLib.registerCommand(new AdemitCmd());
  sgCmdLib.registerCommand(new RmemitCmd());
  sgCmdLib.registerCommand(new AdafctCmd());
  sgCmdLib.registerCommand(new RmafctCmd());
  sgCmdLib.registerCommand(new TsnCmd());
  sgCmdLib.registerCommand(new TbCmd());
}

//------------------------------------------------------------------------------
void OgreConsole::initDir() {
  Console::initDir();

  auto si = new SceneManagerSI(mSceneMgr);
  si->initParams();
  mSceneDir = new AbsDir("scene", si);
  mMovableDir = new AbsDir("movable");
  mNodeDir = new AbsDir("node");

  sgRootDir.addChild(mSceneDir, false);
  sgRootDir.addChild(mMovableDir, false);
  sgRootDir.addChild(mNodeDir, false);
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
  sgArgLib.registerArgHandler(
      new EnumArgHandler<Ogre::ProjectionType>("en_projectionType"));
  sgArgLib.registerArgHandler(
      new EnumArgHandler<Ogre::OrientationMode>("en_orientationMode"));
  sgArgLib.registerArgHandler(
      new EnumArgHandler<pac::AXIS>("en_axis"));
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
  sgArgLib.registerArgHandler(
      new DatablockAH(Ogre::Root::getSingleton().getHlmsManager()));
  // sgArgLib.registerArgHandler(
  // new ResourceAH("compositor", Ogre::CompositorManager::getSingletonPtr()));
}

//------------------------------------------------------------------------------
void OgreConsole::initMiscArgHandler() {
  // scenenode
  sgArgLib.registerArgHandler(new SceneNodeAH());
  sgArgLib.registerArgHandler(new SceneNodeAH("childNode", false));
  sgArgLib.registerArgHandler(new BoneAH());

  // movable
  sgArgLib.registerArgHandler(new MovableAH());
  sgArgLib.registerArgHandler(new MovableAH("childMovable", "", true));
  sgArgLib.registerArgHandler(new MovableAH("particle", "ParticleSystem"));
  sgArgLib.registerArgHandler(new MovableAH("light", "Light"));
  sgArgLib.registerArgHandler(new MovableAH("camera", "Camera"));
  sgArgLib.registerArgHandler(new MovableAH("item", "Item"));

  // t_sceneNode
  sgArgLib.registerArgHandler(new SceneNodeTH());
  // t_movable
  sgArgLib.registerArgHandler(new MovableTH("t_movable", "movable"));
  sgArgLib.registerArgHandler(new MovableTH("t_light", "light"));
  sgArgLib.registerArgHandler(new MovableTH("t_camera", "camera"));
  sgArgLib.registerArgHandler(new MovableTH("t_item", "item"));
  sgArgLib.registerArgHandler(new MovableTH("t_particle", "particle"));
  // tree
  sgArgLib.registerArgHandler(new PositionTH());
  sgArgLib.registerArgHandler(new DirectionTH());

  // used to handler yaw, pitch row
  auto handler = new TreeArgHandler("degree_transform");
  Node* root = handler->getRoot();
  root = handler->getRoot();
  root->acn("degree")->acn("en_transformSpace")->eb("0");
  sgArgLib.registerArgHandler(handler);
  // used to handle SceneNode::LookAt
  handler = new TreeArgHandler("sceneNode_lookAt");
  root = handler->getRoot();
  Node* node = root->acn("scene_position")->acn("en_transformSpace");
  node->eb("0");
  node->acn("localDirectionVector", "real3")->eb("1");
  sgArgLib.registerArgHandler(handler);
  // used to handle SceneNode::setDirection
  handler = new TreeArgHandler("sceneNode_direction");
  root = handler->getRoot();
  node = root->acn("scene_direction")->acn("en_transformSpace");
  node->eb("0");
  node->acn("localDirectionVector", "real3")->eb("1");
  sgArgLib.registerArgHandler(handler);

  // scenemanager related
  handler = new TreeArgHandler("fog");
  root = handler->getRoot();
  // fogMode colorValue expDensity linearStart linearEnd
  root->acn("en_fogMode")
      ->acn("colour", "real4")
      ->acn("expDensity", "real")
      ->acn("linearStart", "real")
      ->acn("linearEnd", "real")
      ->eb("0");
  sgArgLib.registerArgHandler(handler);
}

//------------------------------------------------------------------------------
void OgreConsole::initStringArgHandler() {
  StringArgHandler* moType = new StringArgHandler(
      "moType", {"ParticleSystem", "Light", "Camera", "Item"});
  sgArgLib.registerArgHandler(moType);

  StringArgHandler* forceApp =
      new StringArgHandler("affector_force_application", {"average", "add"});
  sgArgLib.registerArgHandler(forceApp);

  StringArgHandler* resType = new StringArgHandler(
      "resType", {"material", "mesh", "texture", "pst", "datablock"});
  sgArgLib.registerArgHandler(resType);

  StringArgHandler* emitterType = new StringArgHandler("emitterType",
      {"Point", "Box", "Cylinder", "Ellipsoid", "HollowEllipsoid", "Ring"});
  sgArgLib.registerArgHandler(emitterType);

  StringArgHandler* affectorType = new StringArgHandler(
      "affectorType", {"LinearForce", "ColourFader", "ColourFader2", "Scaler",
                          "Rotator", "ColourInterpolator", "ColourImage",
                          "DeflectorPlane", "DirectionRandomiser"});
  sgArgLib.registerArgHandler(affectorType);
}

//------------------------------------------------------------------------------
void OgreConsole::initLiteralArgHandler() {
  // literal
  sgArgLib.registerArgHandler(new LiteralArgHandler("all"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("ancestor"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("camera"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("childOnly"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("direct"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("item"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("light"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("parent"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("parentOfMovable"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("parentOfNode"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("particle"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("sceneNode"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("tagPoint"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("childOfNode"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("descendantOfNode"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("posOfNode"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("dirOfNode"));
}
}
