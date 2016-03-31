#include "pacStable.h"
#include "OgreSI.h"
#include "OgreParticleSystemSI.h"
#include "pacEnumUtil.h"
#include "pacArgHandler.h"
#include "OgreUtil.h"
#include <OgreLight.h>
#include <OgreCamera.h>
#include <OgreEntity.h>
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
CameraSI::Position CameraSI::msPosition;
CameraSI::Orientation CameraSI::msOrientation;
CameraSI::PolygonMode CameraSI::msPolygonMode;
CameraSI::Direction CameraSI::msDirection;
NodeSI::Position NodeSI::msPosition;
NodeSI::Scale NodeSI::msScale;
NodeSI::Orientation NodeSI::msOrientation;
NodeSI::Parent NodeSI::msParent;
NodeSI::Yaw NodeSI::msYaw;
NodeSI::Pitch NodeSI::msPitch;
NodeSI::Roll NodeSI::msRoll;
SceneNodeSI::Direction SceneNodeSI::msDirection;
SceneNodeSI::LookAt SceneNodeSI::msLookAt;
// SceneManagerSI::ShadowTechnique SceneManagerSI::msShadowTechnique;
SceneManagerSI::ShadowColour SceneManagerSI::msShadowColour;
SceneManagerSI::AmbientLight SceneManagerSI::msAmbientLight;
SceneManagerSI::Fog SceneManagerSI::msFog;

//------------------------------------------------------------------------------
StringInterface* OgreSiUtil::createMovableSI(Ogre::MovableObject* mo) {
  const std::string& type = mo->getMovableType();
  if (type == "Light") return new LightSI(static_cast<Ogre::Light*>(mo));

  if (type == "Entity") return new EntitySI(static_cast<Ogre::Entity*>(mo));

  if (type == "ParticleSystem")
    return new ParticleSystemSI(static_cast<Ogre::ParticleSystem*>(mo));

  if (type == "Camera")
    return new CameraSI(static_cast<Ogre::Camera*>(mo));

  else
    PAC_EXCEPT(Exception::ERR_INVALID_STATE,
        "unknown movable type " + mo->getMovableType());
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
    : StringInterface("movable", true), mMovable(obj) {
  if (createParamDict()) initParams();
}

//------------------------------------------------------------------------------
Ogre::MovableObject* MovableSI::getMovable() const { return mMovable; }

//------------------------------------------------------------------------------
void MovableSI::initParams() {
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
LightSI::LightSI(Ogre::Light* light) : MovableSI(light) {
  setName(light->getMovableType());
  if (createParamDict()) initParams();
}

//------------------------------------------------------------------------------
Ogre::Light* LightSI::getLight() const {
  return static_cast<Ogre::Light*>(mMovable);
}

//------------------------------------------------------------------------------
void LightSI::initParams() {
  MovableSI::initParams();
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
std::string CameraSI::PolygonMode::doGet(const void* target) const {
  Ogre::Camera* camera = static_cast<const CameraSI*>(target)->getCamera();
  return enumToString(camera->getPolygonMode());
}

//------------------------------------------------------------------------------
void CameraSI::PolygonMode::doSet(void* target, ArgHandler* handler) {
  Ogre::Camera* camera = static_cast<const CameraSI*>(target)->getCamera();
  camera->setPolygonMode(
      enumFromString<Ogre::PolygonMode>(handler->getValue()));
}

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
CameraSI::CameraSI(Ogre::Camera* camera) : MovableSI(camera) {
  setName(camera->getMovableType());
  if (createParamDict()) initParams();
}

//------------------------------------------------------------------------------
Ogre::Camera* CameraSI::getCamera() const {
  return static_cast<Ogre::Camera*>(mMovable);
}

//------------------------------------------------------------------------------
void CameraSI::initParams() {
  MovableSI::initParams();
  mParamDict->addParameter("position", &msPosition);
  mParamDict->addParameter("orientation", &msOrientation);
  mParamDict->addParameter("polygonMode", &msPolygonMode);
  mParamDict->addParameter("direction", &msDirection);
}

//------------------------------------------------------------------------------
EntitySI::EntitySI(Ogre::Entity* ent) : MovableSI(ent) {
  setName(ent->getMovableType());
  if (createParamDict()) initParams();
}

//------------------------------------------------------------------------------
Ogre::Entity* EntitySI::getEntity() const {
  return static_cast<Ogre::Entity*>(mMovable);
}

//------------------------------------------------------------------------------
void EntitySI::initParams() { MovableSI::initParams(); }

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
NodeSI::NodeSI(Ogre::Node* node) : StringInterface("Node", true), mNode(node) {
  if (createParamDict()) initParams();
}

//------------------------------------------------------------------------------
void NodeSI::initParams() {
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
      tree->getMatchedNodeUniformValue("direction"));
  node->setDirection(dir, ts);
}

//------------------------------------------------------------------------------
std::string SceneNodeSI::LookAt::doGet(const void* target) const {
  Ogre::SceneNode* node =
      static_cast<const SceneNodeSI*>(target)->getSceneNode();
  Ogre::Vector3 xAxes, yAxes, zAxes;
  const Ogre::Quaternion& q = node->getOrientation();
  q.ToAxes(xAxes, yAxes, zAxes);
  return Ogre::StringConverter::toString(-zAxes * 100 + node->getPosition());
}

//------------------------------------------------------------------------------
void SceneNodeSI::LookAt::doSet(void* target, ArgHandler* handler) {
  Ogre::SceneNode* node =
      static_cast<const SceneNodeSI*>(target)->getSceneNode();
  TreeArgHandler* tree = static_cast<TreeArgHandler*>(handler);
  auto ts = enumFromString<Ogre::Node::TransformSpace>(
      tree->getMatchedNodeValue("en_transformSpace"));
  const Ogre::Vector3&& pos = Ogre::StringConverter::parseVector3(
      tree->getMatchedNodeValue("position"));
  node->lookAt(pos, ts);
}

//------------------------------------------------------------------------------
SceneNodeSI::SceneNodeSI(Ogre::SceneNode* sceneNode) : NodeSI(sceneNode) {
  setName("SceneNode");
  if (createParamDict()) initParams();
}

//------------------------------------------------------------------------------
Ogre::SceneNode* SceneNodeSI::getSceneNode() const {
  return static_cast<Ogre::SceneNode*>(mNode);
}

//------------------------------------------------------------------------------
void SceneNodeSI::initParams() {
  NodeSI::initParams();

  mParamDict->addParameter("direction", &msDirection);
  mParamDict->addParameter("lookAt", &msLookAt);
}

//------------------------------------------------------------------------------
// std::string SceneManagerSI::ShadowTechnique::doGet(const void* target) const
// {
// Ogre::SceneManager* sm =
// static_cast<const SceneManagerSI*>(target)->getSceneMgr();
// return enumToString(sm->getShadowTechnique());
//}

//------------------------------------------------------------------------------
// void SceneManagerSI::ShadowTechnique::doSet(void* target, ArgHandler*
// handler) {
// Ogre::SceneManager* sm =
// static_cast<const SceneManagerSI*>(target)->getSceneMgr();
// sm->setShadowTechnique(
// enumFromString<Ogre::ShadowTechnique>(handler->getValue()));
//}

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
std::string SceneManagerSI::AmbientLight::doGet(const void* target) const {
  Ogre::SceneManager* sm =
      static_cast<const SceneManagerSI*>(target)->getSceneMgr();
  return Ogre::StringConverter::toString(sm->getAmbientLight());
}

//------------------------------------------------------------------------------
void SceneManagerSI::AmbientLight::doSet(void* target, ArgHandler* handler) {
  Ogre::SceneManager* sm =
      static_cast<const SceneManagerSI*>(target)->getSceneMgr();
  sm->setAmbientLight(
      Ogre::StringConverter::parseColourValue(handler->getValue()));
}

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
    : StringInterface("SceneManager", true), mSceneMgr(sceneMgr) {
  if (createParamDict()) initParams();
}

//------------------------------------------------------------------------------
void SceneManagerSI::initParams() {
  // mParamDict->addParameter("shadowTechnique", &msShadowTechnique);
  mParamDict->addParameter("shadowColour", &msShadowColour);
  mParamDict->addParameter("ambientLight", &msAmbientLight);
  mParamDict->addParameter("fog", &msFog);
}
}
