#ifndef TESTOGRESI_HPP
#define TESTOGRESI_HPP

#include "OgreSI.h"
#include "OgreConsole.h"
#include "pacAbsDir.h"
#include <gtest/gtest.h>
#include "Ogre.h"
#include "pacEnumUtil.h"

namespace pac {

Real randReal(Real min = -1000, Real max = 1000) {
  return Ogre::Math::RangeRandom(min, max);
}
std::string randRealStr(Real min = -1000, Real max = 1000) {
  return Ogre::StringConverter::toString(randReal(min, max));
}
bool randBool() { return rand() % 2 == 0; }
std::string randBoolStr() {
  return Ogre::StringConverter::toString(randBool());
}

Ogre::Vector3 randVector3(bool normalize = false) {
  auto v = Ogre::Vector3(randReal(), randReal(), randReal());
  if (normalize) v.normalise();
  return v;
}
Ogre::Vector4 randVector4() {
  return Ogre::Vector4(randReal(), randReal(), randReal(), randReal());
}
Ogre::Vector3 randUnitVector3() {
  return Ogre::Vector3(Ogre::Math::UnitRandom(), Ogre::Math::UnitRandom(),
      Ogre::Math::UnitRandom());
}
std::string randVector3Str(bool normalise = false) {
  return Ogre::StringConverter::toString(randVector3(normalise));
}
std::string randVector4Str() {
  return Ogre::StringConverter::toString(randVector4());
}
std::string randUnitVector3Str() {
  return Ogre::StringConverter::toString(randUnitVector3());
}

Ogre::Quaternion randQuaternion(bool normalise = false) {
  auto q = Ogre::Quaternion(randReal(), randReal(), randReal(), randReal());
  if (normalise) q.normalise();
  return q;
}

std::string randQuaternionStr(bool normalise = false) {
  return Ogre::StringConverter::toString(randQuaternion(normalise));
}

class TestOgreSI : public ::testing::Test {
protected:
  void SetUp() {
    mSceneMgr = sgOgreConsole.getSceneMgr();
    mSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("root");
    mLight = mSceneMgr->createLight("light");
    mSinbad = mSceneMgr->createEntity("sinbad", "sinbad.mesh");
    mCamera = mSceneMgr->createCamera("camera");
    mSceneNode->attachObject(mLight);
    mSceneNode->attachObject(mSinbad);
    mSceneNode->attachObject(mCamera);
  }
  void TearDown() {
    mSceneMgr->destroyAllMovableObjects();
    mSceneMgr->destroyMovableObject(mCamera);
    mSceneMgr->destroySceneNode(mSceneNode);
  }

  Ogre::SceneManager* mSceneMgr;
  Ogre::SceneNode* mSceneNode;
  Ogre::Light* mLight;
  Ogre::Camera* mCamera;
  Ogre::Entity* mSinbad;
};

TEST_F(TestOgreSI, testSceneNodeSI) {
  ASSERT_TRUE(sgOgreConsole.execute("ednd root"));
  AbsDir* dir = sgOgreConsole.getCwd();
  ASSERT_STREQ("root", dir->getName().c_str());
  // set random position, quaternion, yaw, pitch, roll
  for (int i = 0; i < 10; ++i) {
    // yaw
    mSceneNode->setOrientation(Ogre::Quaternion::IDENTITY);
    std::string yaw = randRealStr(-180, 180);
    ASSERT_TRUE(sgOgreConsole.execute("set yaw TS_LOCAL " + yaw));
    EXPECT_EQ(yaw, dir->getParameter("yaw"));
    // pitch
    mSceneNode->setOrientation(Ogre::Quaternion::IDENTITY);
    std::string pitch = randRealStr(-180, 180);
    ASSERT_TRUE(sgOgreConsole.execute("set pitch TS_LOCAL " + pitch));
    EXPECT_EQ(pitch, dir->getParameter("pitch"));
    // roll
    mSceneNode->setOrientation(Ogre::Quaternion::IDENTITY);
    std::string roll = randRealStr(-180, 180);
    ASSERT_TRUE(sgOgreConsole.execute("set roll TS_LOCAL " + roll));
    EXPECT_EQ(roll, dir->getParameter("roll"));
    // position
    std::string position = randVector3Str();
    EXPECT_TRUE(sgOgreConsole.execute("set position " + position));
    EXPECT_EQ(position, dir->getParameter("position"));
    // scale
    std::string scale = randVector3Str();
    EXPECT_TRUE(sgOgreConsole.execute("set scale " + scale));
    EXPECT_EQ(scale, dir->getParameter("scale"));
    // orientation
    std::string orientation = randQuaternionStr(true);
    EXPECT_TRUE(sgOgreConsole.execute("set orientation " + orientation));
    EXPECT_EQ(orientation, dir->getParameter("orientation"));
  }
}
TEST_F(TestOgreSI, testMovableSI) {
  ASSERT_TRUE(sgOgreConsole.execute("edmo Light light"));
  AbsDir* dir = sgOgreConsole.getCwd();
  ASSERT_STREQ("light", dir->getName().c_str());
  for (int i = 0; i < 10; ++i) {
    // visible
    std::string visible = randBoolStr();
    EXPECT_TRUE(sgOgreConsole.execute("set visible " + visible));
    EXPECT_EQ(visible, dir->getParameter("visible"));
  }
}

TEST_F(TestOgreSI, testLightSI) {
  ASSERT_TRUE(sgOgreConsole.execute("edmo Light light"));
  AbsDir* dir = sgOgreConsole.getCwd();
  ASSERT_STREQ("light", dir->getName().c_str());
  for (int i = 0; i < 10; ++i) {
    // lightType
    const std::string& lightType =
        EnumData<Ogre::Light::LightTypes>::randomEnumStr();
    EXPECT_TRUE(sgOgreConsole.execute("set lightType " + lightType));
    EXPECT_EQ(lightType, dir->getParameter("lightType"));
    // position
    std::string position = randVector3Str();
    EXPECT_TRUE(sgOgreConsole.execute("set position " + position));
    EXPECT_EQ(position, dir->getParameter("position"));
    // diffuse
    std::string diffuse = randUnitVector3Str();
    EXPECT_TRUE(sgOgreConsole.execute("set diffuse " + diffuse));
    EXPECT_EQ(diffuse + " 1", dir->getParameter("diffuse"));
    // specular
    std::string specular = randUnitVector3Str();
    EXPECT_TRUE(sgOgreConsole.execute("set specular " + specular));
    EXPECT_EQ(specular + " 1", dir->getParameter("specular"));
    // direction
    std::string direction = randVector3Str();
    EXPECT_TRUE(sgOgreConsole.execute("set direction " + direction));
    EXPECT_EQ(direction, dir->getParameter("direction"));
    // spotOuter
    std::string spotOuter = randRealStr();
    EXPECT_TRUE(sgOgreConsole.execute("set spotOuter " + spotOuter));
    EXPECT_EQ(spotOuter, dir->getParameter("spotOuter"));
    // spotInner
    std::string spotInner = randRealStr();
    EXPECT_TRUE(sgOgreConsole.execute("set spotInner " + spotInner));
    EXPECT_EQ(spotInner, dir->getParameter("spotInner"));
    // spotFalloff
    std::string spotFalloff = randRealStr();
    EXPECT_TRUE(sgOgreConsole.execute("set spotFalloff " + spotFalloff));
    EXPECT_EQ(spotFalloff, dir->getParameter("spotFalloff"));
    // spotNearClip
    std::string spotNearClip = randRealStr();
    EXPECT_TRUE(sgOgreConsole.execute("set spotNearClip " + spotNearClip));
    EXPECT_EQ(spotNearClip, dir->getParameter("spotNearClip"));
    // spotRange
    std::string spotRange = randVector3Str();
    EXPECT_TRUE(sgOgreConsole.execute("set spotRange " + spotRange));
    EXPECT_EQ(spotRange, dir->getParameter("spotRange"));
    // attenuation
    std::string attenuation = randVector4Str();
    EXPECT_TRUE(sgOgreConsole.execute("set attenuation " + attenuation));
    EXPECT_EQ(attenuation, dir->getParameter("attenuation"));
    // powerScale
    std::string powerScale = randRealStr();
    EXPECT_TRUE(sgOgreConsole.execute("set powerScale " + powerScale));
    EXPECT_EQ(powerScale, dir->getParameter("powerScale"));
    // shadowFarDist
    std::string shadowFarDist = randRealStr();
    EXPECT_TRUE(sgOgreConsole.execute("set shadowFarDist " + shadowFarDist));
    EXPECT_EQ(shadowFarDist, dir->getParameter("shadowFarDist"));
  }
  EXPECT_TRUE(sgOgreConsole.execute("set lightType LT_DIRECTIONAL"));
}
TEST_F(TestOgreSI, testCameraSI) {
  ASSERT_TRUE(sgOgreConsole.execute("edmo Camera camera"));
  AbsDir* dir = sgOgreConsole.getCwd();
  ASSERT_STREQ("camera", dir->getName().c_str());
  for (int i = 0; i < 10; ++i) {
    // position
    std::string position = randVector3Str();
    EXPECT_TRUE(sgOgreConsole.execute("set position " + position));
    EXPECT_EQ(position, dir->getParameter("position"));
    // orientation
    std::string orientation = randQuaternionStr(true);
    EXPECT_TRUE(sgOgreConsole.execute("set orientation " + orientation));
    EXPECT_EQ(orientation, dir->getParameter("orientation"));
    // polygonMode
    std::string polygonMode = EnumData<Ogre::PolygonMode>::randomEnumStr();
    EXPECT_TRUE(sgOgreConsole.execute("set polygonMode " + polygonMode));
    EXPECT_EQ(polygonMode, dir->getParameter("polygonMode"));
    // direction
    std::string direction = randVector3Str(true);
    EXPECT_TRUE(sgOgreConsole.execute("set direction " + direction));
    EXPECT_EQ(direction, dir->getParameter("direction"));
  }
}
}
#endif /* TESTOGRESI_HPP */
