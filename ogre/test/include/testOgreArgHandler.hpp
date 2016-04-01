#ifndef TESTOGREARGHANDLER_HPP
#define TESTOGREARGHANDLER_HPP

#include "testOgreScene.hpp"

namespace pac {

TEST_F(TestOgreScene, handler_sceneNode) {
  auto handler = sgArgLib.createArgHandler("sceneNode");
  EXPECT_TRUE(handler->validate(mEntNode0Nameid));
  EXPECT_TRUE(handler->validate(mDynamicRootNodeNameid));
  EXPECT_TRUE(handler->validate(mStaticRootNodeNameid));
  EXPECT_TRUE(handler->validate(mEntNode1Nameid));
  EXPECT_TRUE(handler->validate(mLightNodeNameid));
  EXPECT_TRUE(handler->validate(mBlankNode2Nameid));
}

TEST_F(TestOgreScene, handler_childNode) {
  auto handler = sgArgLib.createArgHandler("childNode");
  EXPECT_TRUE(handler->validate(mEntNode0Nameid));
  EXPECT_FALSE(handler->validate(mDynamicRootNodeNameid));
  EXPECT_FALSE(handler->validate(mStaticRootNodeNameid));
  EXPECT_TRUE(handler->validate(mEntNode1Nameid));
  EXPECT_TRUE(handler->validate(mLightNodeNameid));
}

TEST_F(TestOgreScene, handler_t_sceneNode) {
  auto handler = sgArgLib.createArgHandler("t_sceneNode");
  //  sceneNode ("0")
  EXPECT_TRUE(handler->validate(mEntNode0Nameid));
  EXPECT_TRUE(handler->validate(mDynamicRootNodeNameid));
  EXPECT_TRUE(handler->validate(mStaticRootNodeNameid));
  EXPECT_TRUE(handler->validate(mEntNode1Nameid));
  EXPECT_TRUE(handler->validate(mLightNodeNameid));
  EXPECT_EQ(Ogre::StringConverter::toString(
                OgreUtil::getIdFromNameid(mLightNodeNameid)),
      handler->getUniformValue());
  //  en_smmt sceneNode ("1")
  EXPECT_TRUE(handler->validate("SCENE_DYNAMIC" + mEntNode0Nameid));
  EXPECT_TRUE(handler->validate("SCENE_DYNAMIC" + mDynamicRootNodeNameid));
  EXPECT_TRUE(handler->validate("SCENE_STATIC" + mStaticRootNodeNameid));
  EXPECT_TRUE(handler->validate("SCENE_DYNAMIC" + mEntNode1Nameid));
  EXPECT_TRUE(handler->validate("SCENE_DYNAMIC" + mLightNodeNameid));
  EXPECT_TRUE(handler->validate("SCENE_STATIC" + mBlankNode2Nameid));
  EXPECT_EQ(Ogre::StringConverter::toString(
                OgreUtil::getIdFromNameid(mBlankNode2Nameid)),
      handler->getUniformValue());
  //  ltl_childOfNode parentSceneNode sceneNode ("2")
  EXPECT_TRUE(handler->validate(
      "childOfNode " + mEntRootNodeNameid + " " + mEntNode0Nameid));
  EXPECT_TRUE(handler->validate(
      "childOfNode " + mDynamicRootNodeNameid + " " + mRootNodeNameid));
  EXPECT_TRUE(handler->validate(
      "childOfNode " + mRootNodeNameid + " " + mBlankNode2Nameid));
  EXPECT_EQ(Ogre::StringConverter::toString(
                OgreUtil::getIdFromNameid(mBlankNode2Nameid)),
      handler->getUniformValue());
  //  ltl_childOfNode parentSceneNode en_smmt sceneNode ("3")
  EXPECT_TRUE(handler->validate("childOfNode " + mEntRootNodeNameid +
                                " SCENE_DYNAMIC " + mEntNode0Nameid));
  EXPECT_TRUE(handler->validate("childOfNode " + mDynamicRootNodeNameid +
                                " SCENE_DYNAMIC " + mRootNodeNameid));
  EXPECT_TRUE(handler->validate(
      "childOfNode " + mRootNodeNameid + " SCENE_STATIC" + mBlankNode2Nameid));
  EXPECT_EQ(Ogre::StringConverter::toString(
                OgreUtil::getIdFromNameid(mBlankNode2Nameid)),
      handler->getUniformValue());
  //  ltl_descendantOfNode ancestorSceneNode sceneNode ("4")
  EXPECT_TRUE(handler->validate(
      "descendantOfNode " + mEntRootNodeNameid + " " + mEntNode0Nameid));
  EXPECT_TRUE(handler->validate(
      "descendantOfNode " + mDynamicRootNodeNameid + " " + mEntNode0Nameid));
  EXPECT_EQ(Ogre::StringConverter::toString(
                OgreUtil::getIdFromNameid(mEntNode0Nameid)),
      handler->getUniformValue());
  //  ltl_descendantOfNode ancestorSceneNode en_smmt sceneNode ("5")
  EXPECT_TRUE(handler->validate("descendantOfNode " + mDynamicRootNodeNameid +
                                " SCENE_DYNAMIC " + mBlankNode0Nameid));
  EXPECT_TRUE(handler->validate("descendantOfNode " + mDynamicRootNodeNameid +
                                " SCENE_STATIC " + mBlankNode2Nameid));
  EXPECT_EQ(Ogre::StringConverter::toString(
                OgreUtil::getIdFromNameid(mBlankNode2Nameid)),
      handler->getUniformValue());
  EXPECT_FALSE(handler->validate("descendantOfNode " + mDynamicRootNodeNameid +
                                 " SCENE_DYNAMIC " + mBlankNode2Nameid));
  //  ltl_parentOfNode childNode ("ps0")
  EXPECT_TRUE(handler->validate("parentOfNode " + mEntRootNodeNameid));
  EXPECT_TRUE(handler->validate("parentOfNode " + mBlankNode0Nameid));
  EXPECT_TRUE(handler->validate("parentOfNode " + mBlankNode2Nameid));
  EXPECT_TRUE(handler->validate("parentOfNode " + mLightNodeNameid));
  EXPECT_EQ(Ogre::StringConverter::toString(
                OgreUtil::getIdFromNameid(mRootNodeNameid)),
      handler->getUniformValue());

  EXPECT_FALSE(handler->validate("parentOfNode " + mDynamicRootNodeNameid));
  EXPECT_FALSE(handler->validate("parentOfNode " + mStaticRootNodeNameid));
  //  ltl_parentOfMovalbe moType childMovable ("pm0")
  EXPECT_TRUE(handler->validate("parentOfMovable Entity " + mOgreHeadNameid));
  EXPECT_TRUE(handler->validate("parentOfMovable Entity " + mSinbadNameid));
  EXPECT_TRUE(handler->validate("parentOfMovable Camera " + mCamera0Nameid));
  EXPECT_EQ(Ogre::StringConverter::toString(
                OgreUtil::getIdFromNameid(mCameraNodeNameid)),
      handler->getUniformValue());

  EXPECT_FALSE(handler->validate("parentOfMovable Entity " + mLeftSwordNameid));
}

TEST_F(TestOgreScene, handler_t_movable) {
  auto handler = sgArgLib.createArgHandler("t_movable");
  //  moType movable
  ASSERT_TRUE(handler->validate("Entity " + mOgreHeadNameid));
  EXPECT_EQ(Ogre::StringConverter::toString(
                OgreUtil::getIdFromNameid(mOgreHeadNameid)) +
                "@Entity",
      handler->getUniformValue());
  EXPECT_TRUE(handler->validate("Entity " + mOgreHeadNameid));
  EXPECT_TRUE(handler->validate("Light " + mLight0Nameid));
  ASSERT_TRUE(handler->validate("Camera " + mCamera0Nameid));
  EXPECT_EQ(Ogre::StringConverter::toString(
                OgreUtil::getIdFromNameid(mCamera0Nameid)) +
                "@Camera",
      handler->getUniformValue());
  ASSERT_TRUE(handler->validate("ParticleSystem " + mParticle0Nameid));
  EXPECT_EQ(Ogre::StringConverter::toString(
                OgreUtil::getIdFromNameid(mParticle0Nameid)) +
                "@ParticleSystem",
      handler->getUniformValue());
  //  t_sceneNode moType movable
  EXPECT_TRUE(
      handler->validate(mEntNode0Nameid + " Entity " + mOgreHeadNameid));
  EXPECT_TRUE(handler->validate(mLightNodeNameid + " Light " + mLight0Nameid));
  EXPECT_TRUE(
      handler->validate(mCameraNodeNameid + " Camera " + mCamera0Nameid));
  EXPECT_TRUE(handler->validate(
      mParticleNodeNameid + " ParticleSystem " + mParticle0Nameid));
  //  entity moType movable
  //  entity bone moType movable
}

TEST_F(TestOgreScene, handler_resource) {
  auto handler = sgArgLib.createArgHandler("mesh");
  EXPECT_TRUE(handler->validate("ogrehead.mesh"));
  EXPECT_TRUE(handler->validate("Sinbad.mesh"));
  handler = sgArgLib.createArgHandler("pst");
  EXPECT_TRUE(handler->validate("fireball"));
}

TEST_F(TestOgreScene, handler_position) {
  auto handler = sgArgLib.createArgHandler("position");
  EXPECT_TRUE(handler->validate("1 2 3"));
  EXPECT_TRUE(handler->validate("posOfNode " + mEntNode0Nameid));
  EXPECT_TRUE(cmpRealStrings(
      Ogre::StringConverter::toString(mEntNode0->_getDerivedPositionUpdated()),
      handler->getUniformValue()));
}

TEST_F(TestOgreScene, handler_direction) {
  auto handler = sgArgLib.createArgHandler("direction");
  EXPECT_TRUE(handler->validate("1 2 3"));
  EXPECT_TRUE(handler->validate("dirOfNode " + mEntNode0Nameid));
  Ogre::Vector3 v[3];
  mEntNode0->_getDerivedOrientationUpdated().ToAxes(v);
  EXPECT_TRUE(cmpRealStrings(
      Ogre::StringConverter::toString(v[2]), handler->getUniformValue()));
}
TEST_F(TestOgreScene, handler_position_transform) {
  auto handler = sgArgLib.createArgHandler("sceneNode_lookAt");
  EXPECT_TRUE(handler->validate("1 2 3 TS_WORLD"));
  EXPECT_TRUE(handler->validate("posOfNode " + mEntNode0Nameid + " TS_WORLD"));
  Ogre::Vector3 v[3];
  mEntNode0->_getDerivedOrientationUpdated().ToAxes(v);
  EXPECT_TRUE(cmpRealStrings(
      Ogre::StringConverter::toString(v[2]), handler->getUniformValue()));
  EXPECT_TRUE(handler->validate("1 2 3 TS_WORLD 0 1 0"));
}

TEST_F(TestOgreScene, handler_direction_transform) {
  auto handler = sgArgLib.createArgHandler("sceneNode_direction");
  EXPECT_TRUE(handler->validate("1 2 3 TS_WORLD"));
  EXPECT_TRUE(handler->validate("dirOfNode " + mEntNode0Nameid + " TS_WORLD"));
  Ogre::Vector3 v[3];
  mEntNode0->_getDerivedOrientationUpdated().ToAxes(v);
  EXPECT_TRUE(cmpRealStrings(
      Ogre::StringConverter::toString(v[2]), handler->getUniformValue()));
  EXPECT_TRUE(handler->validate("1 2 3 TS_WORLD 0 1 0"));
}
}

#endif /* TESTOGREARGHANDLER_HPP */
