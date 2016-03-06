#include "pacStable.h"
#include "OgreSI.h"
#include "Ogre.h"
#include "OgreParticleSystemSI.h"
#include "pacEnumUtil.h"
#include "pacArgHandler.h"

namespace pac {
LightSI::Visible MovableSI::msVisible;
LightSI::LightType LightSI::msLightType;
LightSI::Position LightSI::msPosition;
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
SceneManagerSI::ShadowTechnique SceneManagerSI::msShadowTechnique;
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
  const Ogre::MovableObject* mo =
      static_cast<const MovableSI*>(target)->getMovable();
  return Ogre::StringConverter::toString(mo->getVisible());
}

//------------------------------------------------------------------------------
void MovableSI::Visible::doSet(void* target, ArgHandler* handler) {
  const Ogre::MovableObject* mo =
      static_cast<const MovableSI*>(target)->getMovable();
  mo->setVisible(Ogre::StringConverter::parseBool(handler->getValue()));
}

//------------------------------------------------------------------------------
MovableSI::MovableSI(Ogre::MovableObject* obj)
    : StringInterface(obj->getMovableType()), mMovable(obj) {
  if (createParamDict()) initParams();
}

//------------------------------------------------------------------------------
Ogre::MovableObject* MovableSI::getMovable() const { return mMovable; }

//------------------------------------------------------------------------------
void MovableSI::initParams() {
  mParamDict->addParameter("visible", &msVisible);
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
std::string LightSI::Position::doGet(const void* target) const {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  return Ogre::StringConverter::toString(light->getPosition());
}

//------------------------------------------------------------------------------
void LightSI::Position::doSet(void* target, ArgHandler* handler) {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  light->setVisible(Ogre::StringConverter::parseVector3(handler->getValue()));
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
      light->getSpotlightOuterAngle().valueRadians());
}

//------------------------------------------------------------------------------
void LightSI::SpotOuter::doSet(void* target, ArgHandler* handler) {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  light->setSpotlightOuterAngle(
      Ogre::StringConverter::parseReal(handler->getValue()));
}

//------------------------------------------------------------------------------
std::string LightSI::SpotInner::doGet(const void* target) const {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  return Ogre::StringConverter::toString(
      light->getSpotlightInnerAngle().valueRadians());
}

//------------------------------------------------------------------------------
void LightSI::SpotInner::doSet(void* target, ArgHandler* handler) {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  light->setSpotlightInnerAngle(
      Ogre::StringConverter::parseReal(handler->getValue()));
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
  ss << light->getSpotlightInnerAngle().valueRadians() << " "
     << light->getSpotlightOuterAngle().valueRadians() << " "
     << light->getSpotlightFalloff();
  return ss.str();
}

//------------------------------------------------------------------------------
void LightSI::SpotRange::doSet(void* target, ArgHandler* handler) {
  Ogre::Light* light = static_cast<const LightSI*>(target)->getLight();
  Ogre::Vector3& v = Ogre::StringConverter::parseVector3(handler->getValue());
  light->setSpotlightRange(v[0], v[1], v[2]);
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
  if (createParamDict()) initParams();
}

//------------------------------------------------------------------------------
Ogre::Light* LightSI::getLight() const { return static_cast<Light*>(mMovable); }

//------------------------------------------------------------------------------
void LightSI::initParams() {
  mParamDict->addParameter("LightType", msLightType);
  mParamDict->addParameter("Position", msPosition);
  mParamDict->addParameter("Diffuse", msDiffuse);
  mParamDict->addParameter("Specular", msSpecular);
  mParamDict->addParameter("Direction", msDirection);
  mParamDict->addParameter("SpotOuter", msSpotOuter);
  mParamDict->addParameter("SpotInner", msSpotInner);
  mParamDict->addParameter("SpotFalloff", msSpotFalloff);
  mParamDict->addParameter("SpotNearClip", msSpotNearClip);
  mParamDict->addParameter("SpotRange", msSpotRange);
  mParamDict->addParameter("Attenuation", msAttenuation);
  mParamDict->addParameter("PowerScale", msPowerScale);
  mParamDict->addParameter("ShadowFarDist", msShadowFarDist);
}

//------------------------------------------------------------------------------
std::string CameraSI::Position::doGet(const void* target) const {
  Ogre::Camera* camera = static_cast<const CameraSI*>(target)->getCamera();
  return Ogre::StringConverter::toString(camera->getPosition());
}

//------------------------------------------------------------------------------
void CameraSI::Position::doSet(void* target, ArgHandler* handler) {
  Ogre::Camera* camera = static_cast<const CameraSI*>(target)->getCamera();
  camera->setVisible(Ogre::StringConverter::parseVector3(handler->getValue()));
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
  if (createParamDict()) initParams();
}

//------------------------------------------------------------------------------
Ogre::Camera* CameraSI::getCamera() const {
  return static_cast<Camera*>(mMovable);
}

//------------------------------------------------------------------------------
void CameraSI::initParams() {
  mparamDict->addParameter("position", msPosition);
  mparamDict->addParameter("orientation", msOrientation);
  mparamDict->addParameter("polygonMode", msPolygonMode);
  mparamDict->addParameter("direction", msDirection);
}

//------------------------------------------------------------------------------
EntitySI::EntitySI(Ogre::Entity* ent) : MovableSI(ent) {
  if (createParamDict()) initParams();
}

//------------------------------------------------------------------------------
Ogre::Entity* EntitySI::getEntity() const {
  return static_cast<Entity*>(mMovable);
}

//------------------------------------------------------------------------------
void EntitySI::initParams() {}

//------------------------------------------------------------------------------
std::string NodeSI::Position::doGet(const void* target) const {
  Ogre::Node* node = static_cast<const Ogre::NodeSI*>(target)->getNode();
  return Ogre::StringConverter::toString(node->getPosition());
}

//------------------------------------------------------------------------------
void NodeSI::Position::doSet(void* target, ArgHandler* handler) {
  Ogre::Node* node = static_cast<const Ogre::NodeSI*>(target)->getNode();
  node->getPosition(Ogre::StringConverter::parseVector3(handler->getValue()));
}

//------------------------------------------------------------------------------
std::string NodeSI::Scale::doGet(const void* target) const {
  Ogre::Node* node = static_cast<const Ogre::NodeSI*>(target)->getNode();
  return Ogre::StringConverter::toString(node->getScale());
}

//------------------------------------------------------------------------------
void NodeSI::Scale::doSet(void* target, ArgHandler* handler) {
  Ogre::Node* node = static_cast<const Ogre::NodeSI*>(target)->getNode();
  node->getPosition(Ogre::StringConverter::parseReal(handler->getValue()));
}

//------------------------------------------------------------------------------
std::string NodeSI::Orientation::doGet(const void* target) const {
  Ogre::Node* node = static_cast<const Ogre::NodeSI*>(target)->getNode();
  return Ogre::StringConverter::toString(node->getOrientation());
}

//------------------------------------------------------------------------------
void NodeSI::Orientation::doSet(void* target, ArgHandler* handler) {
  Ogre::Node* node = static_cast<const Ogre::NodeSI*>(target)->getNode();
  node->getPosition(
      Ogre::StringConverter::parseVector3(handler->getUniformValue()));
}

//------------------------------------------------------------------------------
std::string NodeSI::Parent::doGet(const void* target) const {
  Ogre::Node* node = static_cast<const Ogre::NodeSI*>(target)->getNode();
  Node* parent = node->getParent();
  return parent ? parent->getName() : "";
}

//------------------------------------------------------------------------------
NodeSI::NodeSI(Ogre::Node* sceneNode) : StringInterface("Node") {
  if (createParamDict()) initParams()
}

//------------------------------------------------------------------------------
void NodeSI::initParams() {
  mparamDict->addParameter("Position", msPosition);
  mparamDict->addParameter("Scale", msScale);
  mparamDict->addParameter("Orientation", msOrientation);
  mparamDict->addParameter("Parent", msParent);
}

//------------------------------------------------------------------------------
SceneNodeSI::SceneNodeSI(Ogre::SceneNode* sceneNode)
    : StringInterface("SceneNode") {
  if (createParamDict) initParams();
}

//------------------------------------------------------------------------------
Ogre::SceneNode* SceneNodeSI::getSceneNode() const {
  return static_cast<SceneNode*>(mNode);
}

//------------------------------------------------------------------------------
void SceneNodeSI::initParams() {}

//------------------------------------------------------------------------------
std::string SceneManagerSI::ShadowTechnique::doGet(const void* target) const {
  Ogre::SceneManager* sm =
      static_cast<const Ogre::SceneManagerSI*>(target)->getNode();
  return enumFromString(sm->getShadowTechnique());
}

//------------------------------------------------------------------------------
void SceneManagerSI::ShadowTechnique::doSet(void* target, ArgHandler* handler) {
  Ogre::SceneManager* sm =
      static_cast<const Ogre::SceneManagerSI*>(target)->getSceneMgr();
  sm->setShadowTechnique<Ogre::ShadowTechnique>(handler->getValue());
}

//------------------------------------------------------------------------------
std::string SceneManagerSI::ShadowColour::doGet(const void* target) const {
  Ogre::SceneManager* sm =
      static_cast<const Ogre::SceneManagerSI*>(target)->getSceneMgr();
  return Ogre::StringConverter::toString(sm->getShadowColour());
}
//------------------------------------------------------------------------------
void SceneManagerSI::ShadowColour::doSet(void* target, ArgHandler* handler) {
  Ogre::SceneManager* sm =
      static_cast<const Ogre::SceneManagerSI*>(target)->getSceneMgr();
  sm->setShadowColour(
      Ogre::StringConverter::parseColourValue(handler->getValue()));
}

//------------------------------------------------------------------------------
std::string SceneManagerSI::AmbientLight::doGet(const void* target) const {
  Ogre::SceneManager* sm =
      static_cast<const Ogre::SceneManagerSI*>(target)->getSceneMgr();
  return Ogre::StringConverter::toString(sm->getAmbientLight());
}

//------------------------------------------------------------------------------
void SceneManagerSI::AmbientLight::doSet(void* target, ArgHandler* handler) {
  Ogre::SceneManager* sm =
      static_cast<const Ogre::SceneManagerSI*>(target)->getSceneMgr();
  sm->setAmbientLight(
      Ogre::StringConverter::parseColourValue(handler->getValue()));
}

//------------------------------------------------------------------------------
std::string SceneManagerSI::Fog::doGet(const void* target) const {
  Ogre::SceneManager* sm =
      static_cast<const Ogre::SceneManagerSI*>(target)->getSceneMgr();
  std::stringstream ss;
  ss << enumToString(sm->getFogMode()) << " "
     << Ogre::StringConverter::toString(sm->getFogColour) << " "
     << sm->getFogDensity() << " " << sm->getFogStart() << " "
     << sm->getFogEnd();
}

//------------------------------------------------------------------------------
void SceneManagerSI::Fog::doSet(void* target, ArgHandler* handler) {
  Ogre::SceneManager* sm =
      static_cast<const Ogre::SceneManagerSI*>(target)->getSceneMgr();
  TreeArgHandler* tree = static_cast<TreeArgHandler*>(handler);
  Ogre::FogMode fogMode =
      enumFromString<Ogre::FogMode>(tree->getMatchedNodeValue("fogMode"));
  const ColourValue& colour = tree->getMatchedNodeValue("colour");
  Real expDensity = tree->getMatchedNodeValue("expDensity");
  Real linearStart = tree->getMatchedNodeValue("linearStart");
  Real linearEnd = tree->getMatchedNodeValue("linearEnd");
  sm->setFog(
      defaultOrganisationMode, colour, expDensity, linearStart, linearEnd);
}

//------------------------------------------------------------------------------
SceneManagerSI::SceneManagerSI(Ogre::SceneManager* sceneMgr)
    : StringInterface("SceneManager") {
  if (createParamDict) initParams();
}

//------------------------------------------------------------------------------
void SceneManagerSI::initParams() {
  mParamDict->addParameter("ShadowTechnique", msShadowTechnique);
  mParamDict->addParameter("ShadowColour", msShadowColour);
  mParamDict->addParameter("AmbientLight", msAmbientLight);
  mParamDict->addParameter("Fog", msFog);
}
}
