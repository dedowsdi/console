#ifndef TESTOGRESI_HPP
#define TESTOGRESI_HPP

#include "testOgreScene.h"
#include "OgreSI.h"
#include "OgreConsole.h"
#include "pacAbsDir.h"
#include "pacEnumUtil.h"
#include <OgreStringConverter.h>

namespace pac {

TEST_F(TestOgreScene, testSceneNodeSI) {
  ASSERT_TRUE(sgOgreConsole.execute("ednd " + mBlankNode0Nameid));
  AbsDir* dir = sgOgreConsole.getCwd();
  ASSERT_EQ(mBlankNode0Nameid, dir->getName());
  // set random position, quaternion, yaw, pitch, roll
  for (int i = 0; i < 20; ++i) {
    // yaw
    mBlankNode0->setOrientation(Ogre::Quaternion::IDENTITY);
    std::string yaw = randRealStr(-180, 180);
    ASSERT_TRUE(sgOgreConsole.execute("set yaw " + yaw + " TS_LOCAL"));
    EXPECT_EQ(yaw, dir->getParameter("yaw"));
    // pitch
    mBlankNode0->setOrientation(Ogre::Quaternion::IDENTITY);
    std::string pitch = randRealStr(-180, 180);
    ASSERT_TRUE(sgOgreConsole.execute("set pitch " + pitch + " TS_LOCAL"));
    EXPECT_EQ(pitch, dir->getParameter("pitch"));
    // roll
    mBlankNode0->setOrientation(Ogre::Quaternion::IDENTITY);
    std::string roll = randRealStr(-180, 180);
    ASSERT_TRUE(sgOgreConsole.execute("set roll " + roll + " TS_LOCAL"));
    EXPECT_EQ(roll, dir->getParameter("roll"));
    // direction
    mBlankNode0->setOrientation(Ogre::Quaternion::IDENTITY);
    std::string direction = randVector3Str(true);
    ASSERT_TRUE(
        sgOgreConsole.execute("set direction " + direction + " TS_PARENT"));
    EXPECT_TRUE(cmpRealStrings(direction, dir->getParameter("direction")));
    // lookAt
    mBlankNode0->setOrientation(Ogre::Quaternion::IDENTITY);
    std::string lookAt = randVector3Str();
    ASSERT_TRUE(sgOgreConsole.execute("set lookAt " + lookAt + " TS_PARENT"));
    Ogre::Vector3 lookAtDir = Ogre::StringConverter::parseVector3(lookAt) -
                              mBlankNode0->getPosition();
    lookAtDir.normalise();
    EXPECT_TRUE(cmpRealStrings(Ogre::StringConverter::toString(lookAtDir),
        dir->getParameter("direction")));

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
    EXPECT_TRUE(cmpRealStrings(orientation, dir->getParameter("orientation")));
  }
}
TEST_F(TestOgreScene, testMovableSI) {
  ASSERT_TRUE(sgOgreConsole.execute("edmo Light " + mLight0Nameid));
  AbsDir* dir = sgOgreConsole.getCwd();
  ASSERT_EQ(mLight0Nameid, dir->getName().c_str());
  for (int i = 0; i < 20; ++i) {
    // visible
    std::string visible = randBoolStr();
    EXPECT_TRUE(sgOgreConsole.execute("set visible " + visible));
    EXPECT_EQ(visible, dir->getParameter("visible"));
  }
  ASSERT_EQ(mLightNodeNameid, dir->getParameter("parentNode"));
  EXPECT_TRUE(sgOgreConsole.execute("set parentNode " + mLightNodeNameid));
  EXPECT_FALSE(sgOgreConsole.execute(
      "set parentNode parentOfNode " + mDynamicRootNodeNameid));
  EXPECT_FALSE(sgOgreConsole.execute(
      "set parentNode parentOfNode " + mStaticRootNodeNameid));
  EXPECT_EQ(mLightNodeNameid, dir->getParameter("parentNode"));
}

TEST_F(TestOgreScene, testLightSI) {
  ASSERT_TRUE(sgOgreConsole.execute("edmo Light " + mLight0Nameid));
  AbsDir* dir = sgOgreConsole.getCwd();
  ASSERT_EQ(mLight0Nameid, dir->getName().c_str());
  for (int i = 0; i < 20; ++i) {
    // lightType
    const std::string& lightType =
        EnumData<Ogre::Light::LightTypes>::randomEnumStr();
    EXPECT_TRUE(sgOgreConsole.execute("set lightType " + lightType));
    EXPECT_EQ(lightType, dir->getParameter("lightType"));
    // position
    // std::string position = randVector3Str();
    // EXPECT_TRUE(sgOgreConsole.execute("set position " + position));
    // EXPECT_EQ(position, dir->getParameter("position"));
    // diffuse
    std::string diffuse = randUnitVector3Str();
    EXPECT_TRUE(sgOgreConsole.execute("set diffuse " + diffuse));
    EXPECT_EQ(diffuse + " 1", dir->getParameter("diffuse"));
    // specular
    std::string specular = randUnitVector3Str();
    EXPECT_TRUE(sgOgreConsole.execute("set specular " + specular));
    EXPECT_EQ(specular + " 1", dir->getParameter("specular"));
    // direction
    std::string direction = randVector3Str(true);
    EXPECT_TRUE(sgOgreConsole.execute("set direction " + direction));
    EXPECT_TRUE(cmpRealStrings(direction, dir->getParameter("direction")));
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

TEST_F(TestOgreScene, testFrustumSI) {
  ASSERT_TRUE(sgOgreConsole.execute("edmo Camera " + mCamera0Nameid));
  AbsDir* dir = sgOgreConsole.getCwd();
  ASSERT_EQ(mCamera0Nameid, dir->getName());

  for (int i = 0; i < 10; ++i) {
    std::string fovy = randRealStr(0, 180);
    EXPECT_TRUE(sgOgreConsole.execute("set fovy             " + fovy));
    EXPECT_EQ(fovy, dir->getParameter("fovy"));
    std::string aspectRatio = randRealStr(0, 2);
    EXPECT_TRUE(sgOgreConsole.execute("set aspectRatio      " + aspectRatio));
    EXPECT_EQ(aspectRatio, dir->getParameter("aspectRatio"));
    std::string focalLength = randRealStr(0, 10);
    EXPECT_TRUE(sgOgreConsole.execute("set focalLength      " + focalLength));
    EXPECT_EQ(focalLength, dir->getParameter("focalLength"));
    std::string frustumOffset = randRealStr(0, 10) + " " + randRealStr(0, 10);
    EXPECT_TRUE(sgOgreConsole.execute("set frustumOffset    " + frustumOffset));
    EXPECT_EQ(frustumOffset, dir->getParameter("frustumOffset"));
    std::string frustumExtents = randRealStr(-10, 0) + " " +
                                 randRealStr(0, 10) + " " + randRealStr(0, 10) +
                                 " " + randRealStr(0, -10);
    EXPECT_TRUE(
        sgOgreConsole.execute("set frustumExtents   " + frustumExtents));
    EXPECT_EQ(frustumExtents, dir->getParameter("frustumExtents"));
    std::string projectionType =
        enumToString(randBool() ? Ogre::PT_PERSPECTIVE : Ogre::PT_ORTHOGRAPHIC);
    EXPECT_TRUE(
        sgOgreConsole.execute("set projectionType   " + projectionType));
    EXPECT_EQ(projectionType, dir->getParameter("projectionType"));
    std::string farClipDistance = randRealStr(10, 10000);
    EXPECT_TRUE(
        sgOgreConsole.execute("set farClipDistance  " + farClipDistance));
    EXPECT_EQ(farClipDistance, dir->getParameter("farClipDistance"));
    std::string nearClipDistance = randRealStr(0.1, 10);
    EXPECT_TRUE(
        sgOgreConsole.execute("set nearClipDistance " + nearClipDistance));
    EXPECT_EQ(nearClipDistance, dir->getParameter("nearClipDistance"));
// EXPECT_TRUE(sgOgreConsole.execute("set reflectionPlane  " +
// reflectionPlane
// ));

#if OGRE_NO_VIEWPORT_ORIENTATIONMODE == 0
    std::string orientationMode =
        enumToString(static_cast<Ogre::OrientationMode>(rand() % 4));
    EXPECT_TRUE(
        sgOgreConsole.execute("set orientationMode  " + orientationMode));
    EXPECT_EQ(orientationMode, dir->getParameter("orientationMode"));
#endif
    std::string orthoWindowWidth = randRealStr(0, 180);
    EXPECT_TRUE(
        sgOgreConsole.execute("set orthoWindowWidth " + orthoWindowWidth));
    EXPECT_EQ(orthoWindowWidth, dir->getParameter("orthoWindowWidth"));
    std::string orthoWindowHeight = randRealStr(0, 180);
    EXPECT_TRUE(
        sgOgreConsole.execute("set orthoWindowHeight " + orthoWindowHeight));
    EXPECT_EQ(orthoWindowHeight, dir->getParameter("orthoWindowHeight"));
  }
}

TEST_F(TestOgreScene, testCameraSI) {
  ASSERT_TRUE(sgOgreConsole.execute("edmo Camera " + mCamera0Nameid));
  AbsDir* dir = sgOgreConsole.getCwd();
  ASSERT_EQ(mCamera0Nameid, dir->getName());
  for (int i = 0; i < 20; ++i) {
    // position
    std::string position = randVector3Str();
    EXPECT_TRUE(sgOgreConsole.execute("set position " + position));
    EXPECT_EQ(position, dir->getParameter("position"));
    // orientation
    std::string orientation = randQuaternionStr(true);
    EXPECT_TRUE(sgOgreConsole.execute("set orientation " + orientation));
    EXPECT_TRUE(cmpRealStrings(orientation, dir->getParameter("orientation")));
    // polygonMode
    // std::string polygonMode = EnumData<Ogre::PolygonMode>::randomEnumStr();
    // EXPECT_TRUE(sgOgreConsole.execute("set polygonMode " + polygonMode));
    // EXPECT_EQ(polygonMode, dir->getParameter("polygonMode"));
    // direction
    std::string direction = randVector3Str(true);
    EXPECT_TRUE(sgOgreConsole.execute("set direction " + direction));
    EXPECT_TRUE(cmpRealStrings(direction, dir->getParameter("direction")));
  }
}

TEST_F(TestOgreScene, testSceneSI) {
  ASSERT_TRUE(sgOgreConsole.execute("cd " + d + "scene"));
  AbsDir* dir = sgOgreConsole.getCwd();
  ASSERT_STREQ("scene", dir->getName().c_str());
  std::string ambientLight = randUnitVector4Str();
  // EXPECT_TRUE(sgOgreConsole.execute("set ambientLight " + ambientLight));
  // EXPECT_TRUE(cmpRealStrings(ambientLight,
  // dir->getParameter("ambientLight")));
  std::string shadowColour = randUnitVector4Str();
  EXPECT_TRUE(sgOgreConsole.execute("set shadowColour " + shadowColour));
  EXPECT_TRUE(cmpRealStrings(shadowColour, dir->getParameter("shadowColour")));

  EXPECT_TRUE(sgOgreConsole.execute("set fog FOG_EXP 0.2 0.2 0.2 1 1 2 3"));
}
}
#endif /* TESTOGRESI_HPP */
