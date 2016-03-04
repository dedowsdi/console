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

  // build mot
  StringArgHandler* mot = new StringArgHandler("mot");
  mod->sddString("ParticleSystem");
  mod->addString("Light");
  mod->addString("Camera");
  mod->addString("Entity");
  sgArgLib.registerArgHandler(mod);

  // literal
  sgArgLib.registerArgHandler(new LiteralArgHandler("sceneNode"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("tagPoint"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("light"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("entity"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("particle"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("direct"));
  sgArgLib.registerArgHandler(new LiteralArgHandler("all"));

  // enum and their regex form
  DEFINE_ENUM_CONVERSION(
      Ogre::Light::, LightTypes, (LT_POINT)(LT_DIRECTIONAL)(LT_SPOTLIGHT))
  sgArgLib.registerStringAH(
      new EnumArgHandler<Ogre::LightType>("en_lightType"));

  // movable, scenenode, mesh  and their regex form
  sgArgLib.registerStringAH(new MovableAH("movable"));
  sgArgLib.registerStringAH(new MovableAH("particle", "ParticleSystem"));
  sgArgLib.registerStringAH(new MovableAH("light", "Light"));
  sgArgLib.registerStringAH(new MovableAH("camera", "Camera"));
  sgArgLib.registerStringAH(new MovableAH("entity", "Entity"));
  sgArgLib.registerStringAH(new SceneNodeAH());
  sgArgLib.registerStringAH(new MeshAH());
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
}
