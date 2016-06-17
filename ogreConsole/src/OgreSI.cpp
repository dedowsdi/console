#include "pacStable.h"
#include "OgreSI.h"
#include "OgreParticleSystemSI.h"
#include "pacEnumUtil.h"
#include "pacArgHandler.h"
#include "pacStringInterface.h"
#include "OgreUtil.h"
#include <OgreLight.h>
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreItem.h>
#include <OgreParticleSystem.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include "OgreConsole.h"

namespace pac {
MovableSI::Visible MovableSI::msVisible;
MovableSI::ParentNode MovableSI::msParentNode;
LightSI::LightType LightSI::msLightType;
LightSI::Diffuse LightSI::msDiffuse;
LightSI::Specular LightSI::msSpecular;
LightSI::Direction LightSI::msDirection;
LightSI::SpotOuter LightSI::msSpotOuter;
LightSI::SpotInner LightSI::msSpotInner;
LightSI::SpotFalloff LightSI::msSpotFalloff;
LightSI::SpotNearClip LightSI::msSpotNearClip;
LightSI::SpotRange LightSI::msSpotRange;
LightSI::Attenuation LightSI::msAttenuation;
LightSI::PowerScale LightSI::msPowerScale;
LightSI::ShadowFarDist LightSI::msShadowFarDist;
FrustumSI::FOVy FrustumSI::mFOVy;
FrustumSI::AspectRatio FrustumSI::mAspectRatio;
FrustumSI::FocalLength FrustumSI::mFocalLength;
FrustumSI::FrustumOffset FrustumSI::mFrustumOffset;
FrustumSI::FrustumExtents FrustumSI::mFrustumExtents;
FrustumSI::ProjectionType FrustumSI::mProjectionType;
FrustumSI::FarClipDistance FrustumSI::mFarClipDistance;
FrustumSI::NearClipDistance FrustumSI::mNearClipDistance;
FrustumSI::ReflectionPlane FrustumSI::mReflectionPlane;
#if OGRE_NO_VIEWPORT_ORIENTATIONMODE == 0
FrustumSI::OrientationMode FrustumSI::mOrientationMode;
#endif
FrustumSI::OrthoWindowWidth FrustumSI::mOrthoWindowWidth;
FrustumSI::OrthoWindowHeight FrustumSI::mOrthoWindowHeight;
CameraSI::Position CameraSI::msPosition;
CameraSI::Orientation CameraSI::msOrientation;
// CameraSI::PolygonMode CameraSI::msPolygonMode;
CameraSI::Direction CameraSI::msDirection;
CameraSI::LookAt CameraSI::msLookAt;
NodeSI::Position NodeSI::msPosition;
NodeSI::Scale NodeSI::msScale;
NodeSI::Orientation NodeSI::msOrientation;
NodeSI::Parent NodeSI::msParent;
NodeSI::Yaw NodeSI::msYaw;
NodeSI::Pitch NodeSI::msPitch;
NodeSI::Roll NodeSI::msRoll;
SceneNodeSI::Direction SceneNodeSI::msDirection;
SceneNodeSI::LookAt SceneNodeSI::msLookAt;
SceneManagerSI::ShadowColour SceneManagerSI::msShadowColour;
// SceneManagerSI::AmbientLight SceneManagerSI::msAmbientLight;
SceneManagerSI::Fog SceneManagerSI::msFog;

//------------------------------------------------------------------------------
StringInterface* OgreSiUtil::createMovableSI(Ogre::MovableObject* mo) {
  const std::string& type = mo->getMovableType();
  StringInterface* si = 0;
  if (type == "Light")
    si = new LightSI(static_cast<Ogre::Light*>(mo));

  else if (type == "Item")
    si = new ItemSI(static_cast<Ogre::Item*>(mo));

  else if (type == "ParticleSystem")
    si = new ParticleSystemSI(static_cast<Ogre::ParticleSystem*>(mo));

  else if (type == "Camera")
    si = new CameraSI(static_cast<Ogre::Camera*>(mo));

  else
    PAC_EXCEPT(Exception::ERR_INVALID_STATE,
        "unknown movable type " + mo->getMovableType());
  si->initParams();
  return si;
}

//------------------------------------------------------------------------------
std::string MovableSI::Visible::doGet(const void* target) const {
  Ogre::MovableObject* mo = static_cast<const MovableSI*>(target)->getMovable();
  return Ogre::StringConverter::toString(mo->getVisible());
}

//------------------------------------------------------------------------------
void MovableSI::Visible::doSet(void* target, ArgHandler* handler) {
  Ogre::MovableObject* mo = static_cast<const MovableSI*>(target)->getMovable();
  mo->setVisible(Ogre::StringConverter::parseBool(handler->getValue()));
}

//------------------------------------------------------------------------------
std::string MovableSI::ParentNode::doGet(const void* target) const {
  Ogre::MovableObject* mo = static_cast<const MovableSI*>(target)->getMovable();
  if (mo->isAttached())
    return OgreUtil::createNameid(mo->getParentNode());
  else
    return "";
}

//------------------------------------------------------------------------------
void MovableSI::ParentNode::doSet(void* target, ArgHandler* handler) {
  Ogre::MovableObject* mo = static_cast<const MovableSI*>(target)->getMovable();
  Ogre::SceneNode* node = OgreUtil::getSceneNodeById(
      sgOgreConsole.getSceneMgr(), handler->getUniformValue());
  if (mo->isAttached()) mo->detachFromParent();
  node->attachObject(mo);
}

//------------------------------------------------------------------------------
MovableSI::MovableSI(Ogre::MovableObject* obj)
    : StringInterface(obj->getMovableType(), true), mMovable(obj) {}

//------------------------------------------------------------------------------
Ogre::MovableObject* MovableSI::getMovable() const { return mMovable; }

//------------------------------------------------------------------------------
void MovableSI::buildParams() {
  mParamDict->addParameter("visible", &msVisible);
  mParamDict->addParameter("parentNode", &msParentNode);
}

//------------------------------------------------------------------------------
std::string LightSI::LightType::doGet(const void* target) const {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  return enumToString(light->getType());
}

//------------------------------------------------------------------------------
void LightSI::LightType::doSet(void* target, ArgHandler* handler) {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  light->setType(enumFromString<Ogre::Light::LightTypes>(handler->getValue()));
}

//------------------------------------------------------------------------------
std::string LightSI::Diffuse::doGet(const void* target) const {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  return Ogre::StringConverter::toString(light->getDiffuseColour());
}

//------------------------------------------------------------------------------
void LightSI::Diffuse::doSet(void* target, ArgHandler* handler) {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  light->setDiffuseColour(
      Ogre::StringConverter::parseColourValue(handler->getValue()));
}

//------------------------------------------------------------------------------
std::string LightSI::Specular::doGet(const void* target) const {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  return Ogre::StringConverter::toString(light->getSpecularColour());
}

//------------------------------------------------------------------------------
void LightSI::Specular::doSet(void* target, ArgHandler* handler) {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  light->setSpecularColour(
      Ogre::StringConverter::parseColourValue(handler->getValue()));
}

//------------------------------------------------------------------------------
std::string LightSI::Direction::doGet(const void* target) const {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  return Ogre::StringConverter::toString(light->getDirection());
}

//------------------------------------------------------------------------------
void LightSI::Direction::doSet(void* target, ArgHandler* handler) {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  light->setDirection(Ogre::StringConverter::parseVector3(handler->getValue()));
}

//------------------------------------------------------------------------------
std::string LightSI::SpotOuter::doGet(const void* target) const {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  return Ogre::StringConverter::toString(
      light->getSpotlightOuterAngle().valueDegrees());
}

//------------------------------------------------------------------------------
void LightSI::SpotOuter::doSet(void* target, ArgHandler* handler) {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  light->setSpotlightOuterAngle(
      Ogre::Degree(Ogre::StringConverter::parseReal(handler->getValue())));
}

//------------------------------------------------------------------------------
std::string LightSI::SpotInner::doGet(const void* target) const {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  return Ogre::StringConverter::toString(
      light->getSpotlightInnerAngle().valueDegrees());
}

//------------------------------------------------------------------------------
void LightSI::SpotInner::doSet(void* target, ArgHandler* handler) {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  light->setSpotlightInnerAngle(
      Ogre::Degree(Ogre::StringConverter::parseReal(handler->getValue())));
}

//------------------------------------------------------------------------------
std::string LightSI::SpotFalloff::doGet(const void* target) const {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  return Ogre::StringConverter::toString(light->getSpotlightFalloff());
}

//------------------------------------------------------------------------------
void LightSI::SpotFalloff::doSet(void* target, ArgHandler* handler) {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  light->setSpotlightFalloff(
      Ogre::StringConverter::parseReal(handler->getValue()));
}

//------------------------------------------------------------------------------
std::string LightSI::SpotNearClip::doGet(const void* target) const {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  return Ogre::StringConverter::toString(light->getSpotlightNearClipDistance());
}

//------------------------------------------------------------------------------
void LightSI::SpotNearClip::doSet(void* target, ArgHandler* handler) {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  light->setSpotlightNearClipDistance(
      Ogre::StringConverter::parseReal(handler->getValue()));
}

//------------------------------------------------------------------------------
std::string LightSI::SpotRange::doGet(const void* target) const {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  std::stringstream ss;
  ss << light->getSpotlightInnerAngle().valueDegrees() << " "
     << light->getSpotlightOuterAngle().valueDegrees() << " "
     << light->getSpotlightFalloff();
  return ss.str();
}

//------------------------------------------------------------------------------
void LightSI::SpotRange::doSet(void* target, ArgHandler* handler) {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  Ogre::Vector3&& v = Ogre::StringConverter::parseVector3(handler->getValue());
  light->setSpotlightRange(Ogre::Degree(v[0]), Ogre::Degree(v[1]), v[2]);
}

//------------------------------------------------------------------------------
std::string LightSI::Attenuation::doGet(const void* target) const {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  std::stringstream ss;
  ss << light->getAttenuationRange() << " " << light->getAttenuationConstant()
     << " " << light->getAttenuationLinear() << " "
     << light->getAttenuationQuadric();
  return ss.str();
}

//------------------------------------------------------------------------------
void LightSI::Attenuation::doSet(void* target, ArgHandler* handler) {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  Ogre::Vector4 v = Ogre::StringConverter::parseVector4(handler->getValue());
  light->setAttenuation(v[0], v[1], v[2], v[3]);
}

//------------------------------------------------------------------------------
std::string LightSI::PowerScale::doGet(const void* target) const {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  return Ogre::StringConverter::toString(light->getPowerScale());
}

//------------------------------------------------------------------------------
void LightSI::PowerScale::doSet(void* target, ArgHandler* handler) {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  light->setPowerScale(Ogre::StringConverter::parseReal(handler->getValue()));
}

//------------------------------------------------------------------------------
std::string LightSI::ShadowFarDist::doGet(const void* target) const {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  return Ogre::StringConverter::toString(light->getShadowFarDistance());
}

//------------------------------------------------------------------------------
void LightSI::ShadowFarDist::doSet(void* target, ArgHandler* handler) {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  light->setShadowFarDistance(
      Ogre::StringConverter::parseReal(handler->getValue()));
}

//------------------------------------------------------------------------------
LightSI::LightSI(Ogre::Light* light) : MovableSI(light) {}

//------------------------------------------------------------------------------
Ogre::Light* LightSI::getLight() const {
  return static_cast<Ogre::Light*>(mMovable);
}

//------------------------------------------------------------------------------
void LightSI::buildParams() {
  MovableSI::buildParams();
  mParamDict->addParameter("lightType", &msLightType);
  mParamDict->addParameter("diffuse", &msDiffuse);
  mParamDict->addParameter("specular", &msSpecular);
  mParamDict->addParameter("direction", &msDirection);
  mParamDict->addParameter("spotOuter", &msSpotOuter);
  mParamDict->addParameter("spotInner", &msSpotInner);
  mParamDict->addParameter("spotFalloff", &msSpotFalloff);
  mParamDict->addParameter("spotNearClip", &msSpotNearClip);
  mParamDict->addParameter("spotRange", &msSpotRange);
  mParamDict->addParameter("attenuation", &msAttenuation);
  mParamDict->addParameter("powerScale", &msPowerScale);
  mParamDict->addParameter("shadowFarDist", &msShadowFarDist);
}

//------------------------------------------------------------------------------
FrustumSI::FrustumSI(Ogre::Frustum* frustum) : MovableSI(frustum) {
  setName(frustum->getMovableType());
}

//------------------------------------------------------------------------------
std::string FrustumSI::FOVy::doGet(const void* target) const {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  return Ogre::StringConverter::toString(frustum->getFOVy().valueDegrees());
}

//------------------------------------------------------------------------------
void FrustumSI::FOVy::doSet(void* target, ArgHandler* handler) {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  frustum->setFOVy(Ogre::Degree(
      Ogre::StringConverter::parseReal(handler->getUniformValue())));
}

//------------------------------------------------------------------------------
std::string FrustumSI::AspectRatio::doGet(const void* target) const {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  return Ogre::StringConverter::toString(frustum->getAspectRatio());
}

//------------------------------------------------------------------------------
void FrustumSI::AspectRatio::doSet(void* target, ArgHandler* handler) {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  frustum->setAspectRatio(
      Ogre::StringConverter::parseReal(handler->getUniformValue()));
}

//------------------------------------------------------------------------------
std::string FrustumSI::FocalLength::doGet(const void* target) const {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  return Ogre::StringConverter::toString(frustum->getFocalLength());
}

//------------------------------------------------------------------------------
void FrustumSI::FocalLength::doSet(void* target, ArgHandler* handler) {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  frustum->setFocalLength(
      Ogre::StringConverter::parseReal(handler->getUniformValue()));
}

//------------------------------------------------------------------------------
std::string FrustumSI::FrustumOffset::doGet(const void* target) const {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  return Ogre::StringConverter::toString(frustum->getFrustumOffset());
}

//------------------------------------------------------------------------------
void FrustumSI::FrustumOffset::doSet(void* target, ArgHandler* handler) {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();

  frustum->setFrustumOffset(
      Ogre::StringConverter::parseVector2(handler->getUniformValue()));
}

//------------------------------------------------------------------------------
std::string FrustumSI::FrustumExtents::doGet(const void* target) const {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  Real left, right, top, bottom;
  frustum->getFrustumExtents(left, right, top, bottom);
  Ogre::StringStream ss;
  ss << left << " " << right << " " << top << " " << bottom;
  return ss.str();
}

//------------------------------------------------------------------------------
void FrustumSI::FrustumExtents::doSet(void* target, ArgHandler* handler) {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  Ogre::Vector4 v =
      Ogre::StringConverter::parseVector4(handler->getUniformValue());
  frustum->setFrustumExtents(v[0], v[1], v[2], v[3]);
}

//------------------------------------------------------------------------------
std::string FrustumSI::ProjectionType::doGet(const void* target) const {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  return enumToString(frustum->getProjectionType());
}

//------------------------------------------------------------------------------
void FrustumSI::ProjectionType::doSet(void* target, ArgHandler* handler) {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  frustum->setProjectionType(
      enumFromString<Ogre::ProjectionType>(handler->getUniformValue()));
}

//------------------------------------------------------------------------------
std::string FrustumSI::FarClipDistance::doGet(const void* target) const {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  return Ogre::StringConverter::toString(frustum->getFarClipDistance());
}

//------------------------------------------------------------------------------
void FrustumSI::FarClipDistance::doSet(void* target, ArgHandler* handler) {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  frustum->setFarClipDistance(
      Ogre::StringConverter::parseReal(handler->getUniformValue()));
}

//------------------------------------------------------------------------------
std::string FrustumSI::NearClipDistance::doGet(const void* target) const {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  return Ogre::StringConverter::toString(frustum->getNearClipDistance());
}

//------------------------------------------------------------------------------
void FrustumSI::NearClipDistance::doSet(void* target, ArgHandler* handler) {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  frustum->setNearClipDistance(
      Ogre::StringConverter::parseReal(handler->getUniformValue()));
}

//------------------------------------------------------------------------------
std::string FrustumSI::ReflectionPlane::doGet(const void* target) const {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  return OgreUtil::toString(frustum->getReflectionPlane());
}

#if OGRE_NO_VIEWPORT_ORIENTATIONMODE == 0
//------------------------------------------------------------------------------
std::string FrustumSI::OrientationMode::doGet(const void* target) const {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  return enumToString(frustum->getOrientationMode());
}

//------------------------------------------------------------------------------
void FrustumSI::OrientationMode::doSet(void* target, ArgHandler* handler) {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  frustum->setOrientationMode(
      enumFromString<Ogre::OrientationMode>(handler->getUniformValue()));
}
#endif

//------------------------------------------------------------------------------
std::string FrustumSI::OrthoWindowWidth::doGet(const void* target) const {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  return Ogre::StringConverter::toString(frustum->getOrthoWindowWidth());
}

//------------------------------------------------------------------------------
void FrustumSI::OrthoWindowWidth::doSet(void* target, ArgHandler* handler) {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  frustum->setOrthoWindowWidth(
      Ogre::StringConverter::parseReal(handler->getUniformValue()));
}

//------------------------------------------------------------------------------
std::string FrustumSI::OrthoWindowHeight::doGet(const void* target) const {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  return Ogre::StringConverter::toString(frustum->getOrthoWindowHeight());
}

//------------------------------------------------------------------------------
void FrustumSI::OrthoWindowHeight::doSet(void* target, ArgHandler* handler) {
  Ogre::Frustum* frustum = static_cast<const FrustumSI*>(target)->getFrustum();
  frustum->setOrthoWindowHeight(
      Ogre::StringConverter::parseReal(handler->getUniformValue()));
}

//------------------------------------------------------------------------------
Ogre::Frustum* FrustumSI::getFrustum() const {
  return static_cast<Ogre::Frustum*>(mMovable);
}

//------------------------------------------------------------------------------
void FrustumSI::buildParams() {
  MovableSI::buildParams();
  mParamDict->addParameter("fovy", &mFOVy);
  mParamDict->addParameter("aspectRatio", &mAspectRatio);
  mParamDict->addParameter("focalLength", &mFocalLength);
  mParamDict->addParameter("frustumOffset", &mFrustumOffset);
  mParamDict->addParameter("frustumExtents", &mFrustumExtents);
  mParamDict->addParameter("projectionType", &mProjectionType);
  mParamDict->addParameter("farClipDistance", &mFarClipDistance);
  mParamDict->addParameter("nearClipDistance", &mNearClipDistance);
  mParamDict->addParameter("reflectionPlane", &mReflectionPlane);
#if OGRE_NO_VIEWPORT_ORIENTATIONMODE == 0
  mParamDict->addParameter("orientationMode", &mOrientationMode);
#endif
  mParamDict->addParameter("orthoWindowWidth", &mOrthoWindowWidth);
  mParamDict->addParameter("orthoWindowHeight", &mOrthoWindowHeight);
}

//------------------------------------------------------------------------------
std::string CameraSI::Position::doGet(const void* target) const {
  Ogre::Camera* camera = static_cast<const CameraSI*>(target)->getCamera();
  return Ogre::StringConverter::toString(camera->getPosition());
}

//------------------------------------------------------------------------------
void CameraSI::Position::doSet(void* target, ArgHandler* handler) {
  Ogre::Camera* camera = static_cast<const CameraSI*>(target)->getCamera();
  camera->setPosition(Ogre::StringConverter::parseVector3(handler->getValue()));
}

//------------------------------------------------------------------------------
// std::string CameraSI::PolygonMode::doGet(const void* target) const {
// Ogre::Camera* camera = static_cast<const CameraSI*>(target)->getCamera();
// return enumToString(camera->getPolygonMode());
//}

//------------------------------------------------------------------------------
// void CameraSI::PolygonMode::doSet(void* target, ArgHandler* handler) {
// Ogre::Camera* camera = static_cast<const CameraSI*>(target)->getCamera();
// camera->setPolygonMode(
// enumFromString<Ogre::PolygonMode>(handler->getValue()));
//}

//------------------------------------------------------------------------------
std::string CameraSI::Direction::doGet(const void* target) const {
  Ogre::Camera* camera = static_cast<const CameraSI*>(target)->getCamera();
  return Ogre::StringConverter::toString(camera->getDirection());
}

//------------------------------------------------------------------------------
void CameraSI::Direction::doSet(void* target, ArgHandler* handler) {
  Ogre::Camera* camera = static_cast<const CameraSI*>(target)->getCamera();
  camera->setDirection(
      Ogre::StringConverter::parseVector3(handler->getValue()));
}

//------------------------------------------------------------------------------
std::string CameraSI::LookAt::doGet(const void* target) const {
  Ogre::Camera* camera = static_cast<const CameraSI*>(target)->getCamera();
  Ogre::Vector3 xAxes, yAxes, zAxes;
  camera->getOrientation().ToAxes(xAxes, yAxes, zAxes);
  return Ogre::StringConverter::toString(
      -zAxes * 100 + camera->getDerivedPosition());
}

//------------------------------------------------------------------------------
void CameraSI::LookAt::doSet(void* target, ArgHandler* handler) {
  Ogre::Camera* camera = static_cast<const CameraSI*>(target)->getCamera();
  camera->lookAt(
      Ogre::StringConverter::parseVector3(handler->getUniformValue()));
}

//------------------------------------------------------------------------------
std::string CameraSI::Orientation::doGet(const void* target) const {
  Ogre::Camera* camera = static_cast<const CameraSI*>(target)->getCamera();
  return Ogre::StringConverter::toString(camera->getOrientation());
}

//------------------------------------------------------------------------------
void CameraSI::Orientation::doSet(void* target, ArgHandler* handler) {
  Ogre::Camera* camera = static_cast<const CameraSI*>(target)->getCamera();
  camera->setOrientation(
      Ogre::StringConverter::parseQuaternion(handler->getUniformValue()));
}

//------------------------------------------------------------------------------
CameraSI::CameraSI(Ogre::Camera* camera) : FrustumSI(camera) {}

//------------------------------------------------------------------------------
Ogre::Camera* CameraSI::getCamera() const {
  return static_cast<Ogre::Camera*>(mMovable);
}

//------------------------------------------------------------------------------
void CameraSI::buildParams() {
  FrustumSI::buildParams();
  mParamDict->addParameter("position", &msPosition);
  mParamDict->addParameter("orientation", &msOrientation);
  // mParamDict->addParameter("polygonMode", &msPolygonMode);
  mParamDict->addParameter("direction", &msDirection);
  mParamDict->addParameter("lookAt", &msLookAt);
}

//------------------------------------------------------------------------------
ItemSI::ItemSI(Ogre::Item* item) : MovableSI(item) {}

//------------------------------------------------------------------------------
Ogre::Item* ItemSI::getItem() const {
  return static_cast<Ogre::Item*>(mMovable);
}

//------------------------------------------------------------------------------
void ItemSI::buildParams() { MovableSI::buildParams(); }

//------------------------------------------------------------------------------
std::string NodeSI::Position::doGet(const void* target) const {
  Ogre::Node* node = static_cast<const NodeSI*>(target)->getNode();
  return Ogre::StringConverter::toString(node->getPosition());
}

//------------------------------------------------------------------------------
void NodeSI::Position::doSet(void* target, ArgHandler* handler) {
  Ogre::Node* node = static_cast<const NodeSI*>(target)->getNode();
  node->setPosition(Ogre::StringConverter::parseVector3(handler->getValue()));
}

//------------------------------------------------------------------------------
std::string NodeSI::Scale::doGet(const void* target) const {
  Ogre::Node* node = static_cast<const NodeSI*>(target)->getNode();
  return Ogre::StringConverter::toString(node->getScale());
}

//------------------------------------------------------------------------------
void NodeSI::Scale::doSet(void* target, ArgHandler* handler) {
  Ogre::Node* node = static_cast<const NodeSI*>(target)->getNode();
  node->setScale(Ogre::StringConverter::parseVector3(handler->getValue()));
}

//------------------------------------------------------------------------------
std::string NodeSI::Orientation::doGet(const void* target) const {
  Ogre::Node* node = static_cast<const NodeSI*>(target)->getNode();
  return Ogre::StringConverter::toString(node->getOrientation());
}

//------------------------------------------------------------------------------
void NodeSI::Orientation::doSet(void* target, ArgHandler* handler) {
  Ogre::Node* node = static_cast<const NodeSI*>(target)->getNode();
  node->setOrientation(
      Ogre::StringConverter::parseQuaternion(handler->getUniformValue()));
}

//------------------------------------------------------------------------------
std::string NodeSI::Parent::doGet(const void* target) const {
  Ogre::Node* node = static_cast<const NodeSI*>(target)->getNode();
  Ogre::Node* parent = node->getParent();
  return parent ? parent->getName() : "";
}

//------------------------------------------------------------------------------
std::string NodeSI::Yaw::doGet(const void* target) const {
  Ogre::Node* node = static_cast<const NodeSI*>(target)->getNode();
  const Ogre::Quaternion& q = node->getOrientation();
  return Ogre::StringConverter::toString(q.getYaw().valueDegrees());
}

//------------------------------------------------------------------------------
void NodeSI::Yaw::doSet(void* target, ArgHandler* handler) {
  Ogre::Node* node = static_cast<const NodeSI*>(target)->getNode();
  TreeArgHandler* tree = static_cast<TreeArgHandler*>(handler);
  auto ts = enumFromString<Ogre::Node::TransformSpace>(
      tree->getMatchedNodeValue("en_transformSpace"));
  Real degree =
      Ogre::StringConverter::parseReal(tree->getMatchedNodeValue("degree"));
  node->yaw(Ogre::Degree(degree), ts);
}

//------------------------------------------------------------------------------
std::string NodeSI::Pitch::doGet(const void* target) const {
  Ogre::Node* node = static_cast<const NodeSI*>(target)->getNode();
  const Ogre::Quaternion& q = node->getOrientation();
  return Ogre::StringConverter::toString(q.getPitch().valueDegrees());
}

//------------------------------------------------------------------------------
void NodeSI::Pitch::doSet(void* target, ArgHandler* handler) {
  Ogre::Node* node = static_cast<const NodeSI*>(target)->getNode();
  TreeArgHandler* tree = static_cast<TreeArgHandler*>(handler);
  auto ts = enumFromString<Ogre::Node::TransformSpace>(
      tree->getMatchedNodeValue("en_transformSpace"));
  Real degree =
      Ogre::StringConverter::parseReal(tree->getMatchedNodeValue("degree"));
  node->pitch(Ogre::Degree(degree), ts);
}

//------------------------------------------------------------------------------
std::string NodeSI::Roll::doGet(const void* target) const {
  Ogre::Node* node = static_cast<const NodeSI*>(target)->getNode();
  const Ogre::Quaternion& q = node->getOrientation();
  return Ogre::StringConverter::toString(q.getRoll().valueDegrees());
}

//------------------------------------------------------------------------------
void NodeSI::Roll::doSet(void* target, ArgHandler* handler) {
  Ogre::Node* node = static_cast<const NodeSI*>(target)->getNode();
  TreeArgHandler* tree = static_cast<TreeArgHandler*>(handler);
  auto ts = enumFromString<Ogre::Node::TransformSpace>(
      tree->getMatchedNodeValue("en_transformSpace"));
  Real degree =
      Ogre::StringConverter::parseReal(tree->getMatchedNodeValue("degree"));
  node->roll(Ogre::Degree(degree), ts);
}

//------------------------------------------------------------------------------
NodeSI::NodeSI(Ogre::Node* node, const std::string& name /*= "Node" */)
    : StringInterface(name, true), mNode(node) {}

//------------------------------------------------------------------------------
void NodeSI::buildParams() {
  mParamDict->addParameter("position", &msPosition);
  mParamDict->addParameter("scale", &msScale);
  mParamDict->addParameter("orientation", &msOrientation);
  mParamDict->addParameter("parent", &msParent);
  mParamDict->addParameter("yaw", &msYaw);
  mParamDict->addParameter("pitch", &msPitch);
  mParamDict->addParameter("roll", &msRoll);
}

//------------------------------------------------------------------------------
std::string SceneNodeSI::Direction::doGet(const void* target) const {
  Ogre::SceneNode* node =
      static_cast<const SceneNodeSI*>(target)->getSceneNode();
  Ogre::Vector3 xAxes, yAxes, zAxes;
  const Ogre::Quaternion& q = node->getOrientation();
  q.ToAxes(xAxes, yAxes, zAxes);
  return Ogre::StringConverter::toString(-zAxes);
}

//------------------------------------------------------------------------------
void SceneNodeSI::Direction::doSet(void* target, ArgHandler* handler) {
  Ogre::SceneNode* node =
      static_cast<const SceneNodeSI*>(target)->getSceneNode();
  TreeArgHandler* tree = static_cast<TreeArgHandler*>(handler);
  auto ts = enumFromString<Ogre::Node::TransformSpace>(
      tree->getMatchedNodeValue("en_transformSpace"));
  const Ogre::Vector3&& dir = Ogre::StringConverter::parseVector3(
      tree->getMatchedNodeUniformValue("scene_direction"));
  const std::string& localDirectionVector =
      tree->getMatchedNodeValue("localDirectionVector", {"1"});
  Ogre::Vector3 v = Ogre::StringConverter::parseVector3(
      localDirectionVector, Ogre::Vector3::NEGATIVE_UNIT_Z);
  node->setDirection(dir, ts, v);
}

//------------------------------------------------------------------------------
std::string SceneNodeSI::LookAt::doGet(const void* target) const {
  Ogre::SceneNode* node =
      static_cast<const SceneNodeSI*>(target)->getSceneNode();
  Ogre::Vector3 xAxes, yAxes, zAxes;
  node->getOrientation().ToAxes(xAxes, yAxes, zAxes);
  return Ogre::StringConverter::toString(
      -zAxes * 100 + node->_getDerivedPositionUpdated());
}

//------------------------------------------------------------------------------
void SceneNodeSI::LookAt::doSet(void* target, ArgHandler* handler) {
  Ogre::SceneNode* node =
      static_cast<const SceneNodeSI*>(target)->getSceneNode();
  TreeArgHandler* tree = static_cast<TreeArgHandler*>(handler);
  auto ts = enumFromString<Ogre::Node::TransformSpace>(
      tree->getMatchedNodeValue("en_transformSpace"));
  const Ogre::Vector3&& pos = Ogre::StringConverter::parseVector3(
      tree->getMatchedNodeValue("scene_position"));
  const std::string& localDirectionVector =
      tree->getMatchedNodeValue("localDirectionVector", {"1"});
  Ogre::Vector3 v = Ogre::StringConverter::parseVector3(
      localDirectionVector, Ogre::Vector3::NEGATIVE_UNIT_Z);
  node->lookAt(pos, ts, v);
}

//------------------------------------------------------------------------------
SceneNodeSI::SceneNodeSI(
    Ogre::SceneNode* sceneNode, const std::string& name /*="SceneNode"*/)
    : NodeSI(sceneNode, name) {}

//------------------------------------------------------------------------------
Ogre::SceneNode* SceneNodeSI::getSceneNode() const {
  return static_cast<Ogre::SceneNode*>(mNode);
}

//------------------------------------------------------------------------------
void SceneNodeSI::buildParams() {
  NodeSI::buildParams();

  mParamDict->addParameter("direction", &msDirection);
  mParamDict->addParameter("lookAt", &msLookAt);
}

//------------------------------------------------------------------------------
std::string SceneManagerSI::ShadowColour::doGet(const void* target) const {
  Ogre::SceneManager* sm =
      static_cast<const SceneManagerSI*>(target)->getSceneMgr();
  return Ogre::StringConverter::toString(sm->getShadowColour());
}
//------------------------------------------------------------------------------
void SceneManagerSI::ShadowColour::doSet(void* target, ArgHandler* handler) {
  Ogre::SceneManager* sm =
      static_cast<const SceneManagerSI*>(target)->getSceneMgr();
  sm->setShadowColour(
      Ogre::StringConverter::parseColourValue(handler->getValue()));
}

//------------------------------------------------------------------------------
// std::string SceneManagerSI::AmbientLight::doGet(const void* target) const {
// Ogre::SceneManager* sm =
// static_cast<const SceneManagerSI*>(target)->getSceneMgr();
// return Ogre::StringConverter::toString(sm->getAmbientLight());
//}

//------------------------------------------------------------------------------
// void SceneManagerSI::AmbientLight::doSet(void* target, ArgHandler* handler) {
// Ogre::SceneManager* sm =
// static_cast<const SceneManagerSI*>(target)->getSceneMgr();
// sm->setAmbientLight(
// Ogre::StringConverter::parseColourValue(handler->getValue()));
//}

//------------------------------------------------------------------------------
std::string SceneManagerSI::Fog::doGet(const void* target) const {
  Ogre::SceneManager* sm =
      static_cast<const SceneManagerSI*>(target)->getSceneMgr();
  std::stringstream ss;
  ss << enumToString(sm->getFogMode()) << " "
     << Ogre::StringConverter::toString(sm->getFogColour()) << " "
     << sm->getFogDensity() << " " << sm->getFogStart() << " "
     << sm->getFogEnd();
  return ss.str();
}

//------------------------------------------------------------------------------
void SceneManagerSI::Fog::doSet(void* target, ArgHandler* handler) {
  Ogre::SceneManager* sm =
      static_cast<const SceneManagerSI*>(target)->getSceneMgr();
  TreeArgHandler* tree = static_cast<TreeArgHandler*>(handler);
  Ogre::FogMode fogMode =
      enumFromString<Ogre::FogMode>(tree->getMatchedNodeValue("en_fogMode"));
  const Ogre::ColourValue& colour = Ogre::StringConverter::parseColourValue(
      tree->getMatchedNodeValue("colour"));
  Real expDensity =
      Ogre::StringConverter::parseReal(tree->getMatchedNodeValue("expDensity"));
  Real linearStart = Ogre::StringConverter::parseReal(
      tree->getMatchedNodeValue("linearStart"));
  Real linearEnd =
      Ogre::StringConverter::parseReal(tree->getMatchedNodeValue("linearEnd"));
  sm->setFog(fogMode, colour, expDensity, linearStart, linearEnd);
}

//------------------------------------------------------------------------------
SceneManagerSI::SceneManagerSI(Ogre::SceneManager* sceneMgr)
    : StringInterface("SceneManager", true), mSceneMgr(sceneMgr) {}

//------------------------------------------------------------------------------
void SceneManagerSI::buildParams() {
  // mParamDict->addParameter("shadowTechnique", &msShadowTechnique);
  mParamDict->addParameter("shadowColour", &msShadowColour);
  // mParamDict->addParameter("ambientLight", &msAmbientLight);
  mParamDict->addParameter("fog", &msFog);
}
}
