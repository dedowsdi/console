/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#include "TutorialApplication.h"
using namespace Ogre;

//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void) {}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void) {}

//------------------------------------------------------------------
void TutorialApplication::initGui(void) {
  BaseApplication::initGui();
  sgmLayoutMgr.loadLayout("term.layout");
}

//------------------------------------------------------------------
void TutorialApplication::createCamera(void) {
  BaseApplication::createCamera();
  mCamera->setPosition(0, 0, 500);
}

//------------------------------------------------------------------
bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt) {
  static Real s = 0;
  s += evt.timeSinceLastFrame;

  SceneNode* mainNode = mSceneMgr->getSceneNode("mainNode");
  Vector3 rotateAxis =
      mainNode->getUserObjectBindings().getUserAny("rotateAxis").get<Vector3>();
  Radian rotateSpeed =
      mainNode->getUserObjectBindings().getUserAny("rotateSpeed").get<Radian>();
  Quaternion q(rotateSpeed * s, rotateAxis);
  // mainNode->setOrientation(q);
  mainNode->rotate(rotateAxis, rotateSpeed * evt.timeSinceLastFrame);

  for (int i = 0; i < 12; ++i) {
    Node* node = mainNode->getChild(i);
    Vector3 rotateAxis =
        node->getUserObjectBindings().getUserAny("rotateAxis").get<Vector3>();
    Radian rotateSpeed =
        node->getUserObjectBindings().getUserAny("rotateSpeed").get<Radian>();
    node->rotate(rotateAxis, rotateSpeed * evt.timeSinceLastFrame);
  }
  return BaseApplication::frameRenderingQueued(evt);
}

//---------------------------------------------------------------------------
void TutorialApplication::createScene(void) {
  // light
  SceneNode* lightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
  Light* light = mSceneMgr->createLight();
  light->setType(Light::LT_DIRECTIONAL);
  light->setDirection(-1, -1, -1);
  lightNode->attachObject(light);

  mSceneMgr->setAmbientLight(ColourValue(0.1, 0.1, 0.1, 1));

  // main
  SceneNode* mainNode =
      mSceneMgr->getRootSceneNode()->createChildSceneNode("mainNode");
  Entity* head = mSceneMgr->createEntity("ogrehead.mesh");
  mainNode->attachObject(head);
  mainNode->getUserObjectBindings().setUserAny(
      "rotateAxis", Any(Vector3(0, 0, -1)));
  mainNode->getUserObjectBindings().setUserAny(
      "rotateSpeed", Any(Radian(Math::PI * 0.125f)));
  // children
  for (int i = 0; i < 12; ++i) {
    SceneNode* node = mainNode->createChildSceneNode(
        "childNode" + StringConverter::toString(i));
    Entity* head = mSceneMgr->createEntity("ogrehead.mesh");
    node->setScale(0.5, 0.5, 0.5);
    node->attachObject(head);
    // place it
    Quaternion q(Degree(30 * i), Vector3::UNIT_Z);
    node->setPosition(q * Vector3(0, 160, 0));
    // facing
    node->yaw(Degree(90));
    node->roll(Degree(30 * i), Node::TS_PARENT);

    node->getUserObjectBindings().setUserAny(
        "rotateAxis", Any(Vector3(0, 1, 0)));
    node->getUserObjectBindings().setUserAny(
        "rotateSpeed", Any(Radian(Math::PI * 0.125f)));
  }
}

//---------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char* argv[])
#endif
{
  // Create application object
  TutorialApplication app;

  try {
    app.go();
  } catch (Ogre::Exception& e) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    MessageBox(NULL, e.getFullDescription().c_str(),
        "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
    std::cerr << "An exception has occurred: " << e.getFullDescription().c_str()
              << std::endl;
#endif
  }

  return 0;
}

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
