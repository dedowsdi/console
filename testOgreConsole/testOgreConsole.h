#ifndef TESTOGRECONSOLE_H
#define TESTOGRECONSOLE_H

#include "testOgreScene.h"
#include "OgreConsole.h"
#include "pacAbsDir.h"
#include <gtest/gtest.h>
#include <OgreSceneManager.h>
#include <limits>
#include <OgreEntity.h>
#include "pacIntrinsicArgHandler.h"
#include "../testConsole/testConsoleUI.h"
#include "OgreUtil.h"
#include "OgreArgHandler.h"

namespace pac {

TEST_F(TestOgreScene, testAddNode) {
  ASSERT_TRUE(
      sgOgreConsole.execute("adnd " + mRootNodeNameid + " SCENE_STATIC node0"));
  EXPECT_TRUE(OgreUtil::hasSceneNode(mSceneMgr, "node0"));
  ASSERT_TRUE(sgOgreConsole.execute(
      "adnd " + mRootNodeNameid + " SCENE_DYNAMIC node1"));
  EXPECT_TRUE(OgreUtil::hasSceneNode(mSceneMgr, "node1"));
}

TEST_F(TestOgreScene, testEditNode) {
  AbsDir* nodeRoot = sgRootDir.getChildByName("node");
  ASSERT_TRUE(sgOgreConsole.execute("ednd " + mEntNode0Nameid));
  EXPECT_TRUE(nodeRoot->hasChild(mEntNode0Nameid));
  ASSERT_TRUE(sgOgreConsole.execute(
      "ednd childOfNode " + mEntRootNodeNameid + " " + mEntNode1Nameid));
  EXPECT_TRUE(nodeRoot->hasChild(mEntNode1Nameid));
  ASSERT_TRUE(sgOgreConsole.execute("ednd childOfNode " + mRootNodeNameid +
                                    " SCENE_DYNAMIC " + mBlankNode0Nameid));
  EXPECT_TRUE(nodeRoot->hasChild(mBlankNode0Nameid));
  ASSERT_TRUE(
      sgOgreConsole.execute("ednd descendantOfNode " + mDynamicRootNodeNameid +
                            " " + mBlankNode1Nameid));
  EXPECT_TRUE(nodeRoot->hasChild(mBlankNode1Nameid));
  ASSERT_TRUE(
      sgOgreConsole.execute("ednd descendantOfNode " + mDynamicRootNodeNameid +
                            " SCENE_STATIC " + mBlankNode2Nameid));
  EXPECT_TRUE(nodeRoot->hasChild(mBlankNode2Nameid));
  EXPECT_FALSE(
      sgOgreConsole.execute("ednd parentOfNode " + mDynamicRootNodeNameid));
  EXPECT_FALSE(
      sgOgreConsole.execute("ednd parentOfNode " + mStaticRootNodeNameid));
  EXPECT_FALSE(
      sgOgreConsole.execute("ednd parentOfmovable " + mLeftSwordNameid));
  EXPECT_TRUE(
      sgOgreConsole.execute("ednd parentOfMovable Item " + mOgreHeadNameid));
}

TEST_F(TestOgreScene, testRmNode) {
  ASSERT_TRUE(sgOgreConsole.execute("rmnd " + mBlankNode0Nameid));
  EXPECT_FALSE(OgreUtil::hasSceneNode(mSceneMgr, "blank0"));
  ASSERT_TRUE(sgOgreConsole.execute("rmnd " + mBlankNode1Nameid));
  EXPECT_FALSE(OgreUtil::hasSceneNode(mSceneMgr, "blank1"));
}

TEST_F(TestOgreScene, testLsnd) {
  EXPECT_TRUE(sgOgreConsole.execute("lsnd"));
  EXPECT_EQ(getSortedVector(
                {mDynamicRootNodeNameid, mStaticRootNodeNameid, mRootNodeNameid,
                 mLightNodeNameid, mCameraNodeNameid, mEntRootNodeNameid,
                 mEntNode0Nameid, mEntNode1Nameid, mBlankNode0Nameid,
                 mBlankNode1Nameid, mBlankNode2Nameid, mParticleNodeNameid}),
      mUi->getItems());
  EXPECT_TRUE(sgOgreConsole.execute("lsnd SCENE_STATIC"));
  EXPECT_EQ(getSortedVector({mStaticRootNodeNameid, mBlankNode2Nameid}),
      mUi->getItems());
  EXPECT_TRUE(sgOgreConsole.execute("lsnd SCENE_DYNAMIC"));
  EXPECT_EQ(getSortedVector({mDynamicRootNodeNameid, mRootNodeNameid,
                mLightNodeNameid, mCameraNodeNameid, mEntRootNodeNameid,
                mEntNode0Nameid, mEntNode1Nameid, mBlankNode0Nameid,
                mBlankNode1Nameid, mParticleNodeNameid}),
      mUi->getItems());
  EXPECT_TRUE(sgOgreConsole.execute("lsnd " + mRootNodeNameid));
  EXPECT_EQ(getSortedVector({mLightNodeNameid, mCameraNodeNameid,
                mEntRootNodeNameid, mBlankNode0Nameid, mBlankNode1Nameid,
                mBlankNode2Nameid, mParticleNodeNameid}),
      mUi->getItems());
  EXPECT_TRUE(
      sgOgreConsole.execute("lsnd " + mRootNodeNameid + " SCENE_STATIC"));
  EXPECT_EQ(getSortedVector({mBlankNode2Nameid}), mUi->getItems());
  EXPECT_TRUE(
      sgOgreConsole.execute("lsnd " + mRootNodeNameid + " SCENE_DYNAMIC"));
  EXPECT_EQ(
      getSortedVector({mLightNodeNameid, mCameraNodeNameid, mEntRootNodeNameid,
          mBlankNode0Nameid, mBlankNode1Nameid, mParticleNodeNameid}),
      mUi->getItems());
  EXPECT_TRUE(sgOgreConsole.execute("lsnd -r " + mRootNodeNameid));
  EXPECT_EQ(
      getSortedVector({mLightNodeNameid, mCameraNodeNameid, mEntRootNodeNameid,
          mEntNode0Nameid, mEntNode1Nameid, mBlankNode0Nameid,
          mBlankNode1Nameid, mBlankNode2Nameid, mParticleNodeNameid}),
      mUi->getItems());
  EXPECT_TRUE(
      sgOgreConsole.execute("lsnd -r" + mRootNodeNameid + " SCENE_STATIC"));
  EXPECT_EQ(getSortedVector({mBlankNode2Nameid}), mUi->getItems());
  EXPECT_TRUE(
      sgOgreConsole.execute("lsnd -r" + mRootNodeNameid + " SCENE_DYNAMIC"));
  EXPECT_EQ(getSortedVector({mLightNodeNameid, mCameraNodeNameid,
                mEntRootNodeNameid, mEntNode0Nameid, mEntNode1Nameid,
                mBlankNode0Nameid, mBlankNode1Nameid, mParticleNodeNameid}),
      mUi->getItems());
  EXPECT_TRUE(sgOgreConsole.execute("lsnd regex .*"));
  EXPECT_EQ(getSortedVector(
                {mDynamicRootNodeNameid, mStaticRootNodeNameid, mRootNodeNameid,
                 mLightNodeNameid, mCameraNodeNameid, mEntRootNodeNameid,
                 mEntNode0Nameid, mEntNode1Nameid, mBlankNode0Nameid,
                 mBlankNode1Nameid, mBlankNode2Nameid, mParticleNodeNameid}),
      mUi->getItems());
  EXPECT_TRUE(sgOgreConsole.execute("lsnd regex .* SCENE_STATIC"));
  EXPECT_EQ(getSortedVector({mStaticRootNodeNameid, mBlankNode2Nameid}),
      mUi->getItems());
  EXPECT_TRUE(sgOgreConsole.execute("lsnd regex .* SCENE_DYNAMIC"));
  EXPECT_EQ(getSortedVector({mDynamicRootNodeNameid, mRootNodeNameid,
                mLightNodeNameid, mCameraNodeNameid, mEntRootNodeNameid,
                mEntNode0Nameid, mEntNode1Nameid, mBlankNode0Nameid,
                mBlankNode1Nameid, mParticleNodeNameid}),
      mUi->getItems());
  EXPECT_TRUE(sgOgreConsole.execute("lsnd parentOfNode " + mEntNode0Nameid));
  EXPECT_EQ(getSortedVector({mEntRootNodeNameid}), mUi->getItems());
  EXPECT_TRUE(
      sgOgreConsole.execute("lsnd parentOfNode " + mDynamicRootNodeNameid));
  EXPECT_TRUE(
      sgOgreConsole.execute("lsnd parentOfMovable Item " + mOgreHeadNameid));
  EXPECT_EQ(getSortedVector({mEntNode0Nameid}), mUi->getItems());
  EXPECT_TRUE(
      sgOgreConsole.execute("lsnd parentOfMovable Item " + mLeftSwordNameid));
}

TEST_F(TestOgreScene, testLsNodePrompt) {
  mUi->setCmdLine("lsnd pare");
  sgConsole.prompt();
  EXPECT_STREQ("lsnd parentOf", getCmdLine().c_str());
}

TEST_F(TestOgreScene, testLsres) {
  EXPECT_TRUE(sgOgreConsole.execute("lsres material"));
  EXPECT_TRUE(sgOgreConsole.execute("lsres material regex .*"));
  EXPECT_TRUE(sgOgreConsole.execute("lsres mesh"));
  EXPECT_TRUE(sgOgreConsole.execute("lsres mesh regex .*"));
  EXPECT_TRUE(sgOgreConsole.execute("lsres texture"));
  EXPECT_TRUE(sgOgreConsole.execute("lsres texture regex .*"));
  // EXPECT_TRUE(sgOgreConsole.execute("lsres compositor"));
  // EXPECT_TRUE(sgOgreConsole.execute("lsres compositor regex .*"));
  EXPECT_TRUE(sgOgreConsole.execute("lsres pst"));
  EXPECT_TRUE(sgOgreConsole.execute("lsres pst regex .*"));
  EXPECT_TRUE(sgOgreConsole.execute("lsres datablock"));
  EXPECT_TRUE(sgOgreConsole.execute("lsres datablock regex .*"));
}
TEST_F(TestOgreScene, testLsresPrompt) {
  sgConsole.getUi()->setCmdLine("lsres material regex S");
  sgConsole.prompt();
  EXPECT_STREQ("lsres material regex S", getCmdLine().c_str());
}

TEST_F(TestOgreScene, testLsmoOutput) {
  // EXPECT_TRUE(sgOgreConsole.execute("lsmo Entity"));
  // EXPECT_EQ(getSortedVector({mOgreHeadNameid, mSinbadNameid,
  // mLeftSwordNameid,
  // mRightSwordNameid}),
  // mUi->getItems());
  // EXPECT_TRUE(sgOgreConsole.execute("lsmo Entity regex .*"));
  // EXPECT_EQ(getSortedVector({mOgreHeadNameid, mSinbadNameid,
  // mLeftSwordNameid,
  // mRightSwordNameid}),
  // mUi->getItems());
  EXPECT_TRUE(sgOgreConsole.execute("lsmo Item"));
  EXPECT_EQ(getSortedVector({mOgreHeadNameid, mSinbadNameid, mLeftSwordNameid,
                mRightSwordNameid}),
      mUi->getItems());
  EXPECT_TRUE(sgOgreConsole.execute("lsmo Item regex .*"));
  EXPECT_EQ(getSortedVector({mOgreHeadNameid, mSinbadNameid, mLeftSwordNameid,
                mRightSwordNameid}),
      mUi->getItems());

  EXPECT_TRUE(sgOgreConsole.execute("lsmo sceneNode " + mEntNode0Nameid));
  EXPECT_EQ(getSortedVector({mOgreHeadNameid}), mUi->getItems());
  // EXPECT_TRUE(sgOgreConsole.execute(
  //"lsmo sceneNode childOfNode " + mEntRootNodeNameid + " " +
  // mEntNode0Nameid));
  // EXPECT_EQ(getSortedVector({mOgreHeadNameid}), mUi->getItems());
  // EXPECT_TRUE(
  // sgOgreConsole.execute("lsmo sceneNode childOfNode " + mEntRootNodeNameid +
  //" SCENE_DYNAMIC " + mEntNode0Nameid));
  // EXPECT_EQ(getSortedVector({mOgreHeadNameid}), mUi->getItems());
  // EXPECT_TRUE(
  // sgOgreConsole.execute("lsmo sceneNode descendantOfNode " +
  // mEntRootNodeNameid +
  //" SCENE_DYNAMIC " + mEntNode0Nameid));
  // EXPECT_EQ(getSortedVector({mOgreHeadNameid}), mUi->getItems());
  // EXPECT_TRUE(
  // sgOgreConsole.execute("lsmo sceneNode descendantOfNode " + mRootNodeNameid
  // +
  //" SCENE_DYNAMIC " + mEntNode0Nameid));
  // EXPECT_EQ(getSortedVector({mOgreHeadNameid}), mUi->getItems());
  EXPECT_TRUE(
      sgOgreConsole.execute("lsmo sceneNode " + mEntNode0Nameid + " Item"));
  EXPECT_EQ(getSortedVector({mOgreHeadNameid}), mUi->getItems());
}

TEST_F(TestOgreScene, testLsmo) {
  StringArgHandler* handler =
      static_cast<StringArgHandler*>(sgArgLib.createArgHandler("moType"));
  std::for_each(handler->beginStringIter(), handler->endStringIter(),
      [&](const std::string& moType) -> void {
        // lsmo moType ("g0")
        // lsmo moType ltl_regex regex ("g1")
        // lsmo ltl_sceneNode t_sceneNode ("sn0")
        // lsmo ltl_sceneNode t_sceneNode moType ("sn1")
        // lsmo ltl_tagPoint entity ("tag0")
        // lsmo ltl_tagPoint entity moType ("tag1")
        // lsmo ltl_tagPoint entity bone ("tag2")
        // lsmo ltl_tagPoint entity bone moType ("tag3")
        EXPECT_TRUE(sgOgreConsole.execute("lsmo " + moType));
        EXPECT_TRUE(sgOgreConsole.execute("lsmo " + moType + " regex .*"));
        EXPECT_TRUE(sgOgreConsole.execute("lsmo sceneNode " + mEntNode0Nameid));
        // EXPECT_TRUE(
        // sgOgreConsole.execute("lsmo sceneNode childOfNode " +
        // mEntRootNodeNameid + " " + mEntNode0Nameid));
        // EXPECT_TRUE(sgOgreConsole.execute("lsmo sceneNode childOfNode " +
        // mEntRootNodeNameid +
        //" SCENE_DYNAMIC " + mEntNode0Nameid));
        // EXPECT_TRUE(sgOgreConsole.execute("lsmo sceneNode descendantOfNode "
        // +
        // mEntRootNodeNameid +
        //" SCENE_DYNAMIC " + mEntNode0Nameid));
        // EXPECT_TRUE(
        // sgOgreConsole.execute("lsmo sceneNode descendantOfNode " +
        // mRootNodeNameid +
        //" SCENE_DYNAMIC " + mEntNode0Nameid));
        EXPECT_TRUE(sgOgreConsole.execute(
            "lsmo sceneNode " + mEntNode0Nameid + moType));
        // EXPECT_TRUE(sgOgreConsole.execute("lsmo tagPoint sinbad "));
        // EXPECT_TRUE(sgOgreConsole.execute("lsmo tagPoint sinbad " + moType));
        // EXPECT_TRUE(
        // sgOgreConsole.execute("lsmo tagPoint sinbad IndexFingerDist.L "));
        // EXPECT_TRUE(sgOgreConsole.execute(
        //"lsmo tagPoint sinbad IndexFingerDist.L " + moType));
      });
}

TEST_F(TestOgreScene, testAth) {
  // ath ltl_sceneNode sceneNode ltl_light id ("sn0")
  // ath ltl_sceneNode sceneNode ltl_entity id mesh ("sn1")
  // ath ltl_sceneNode sceneNode ltl_particle id pst ("sn2")
  // ath ltl_sceneNode sceneNode ltl_camera id("sn3")
  EXPECT_TRUE(sgOgreConsole.execute(
      "ath sceneNode " + mBlankNode0Nameid + " light sn0"));
  EXPECT_TRUE(sgOgreConsole.execute(
      "ath sceneNode " + mBlankNode0Nameid + " item sn0 ogrehead.mesh"));
  //EXPECT_TRUE(sgOgreConsole.execute(
      //"ath sceneNode " + mBlankNode0Nameid + " particle sn0 fireball"));
  EXPECT_TRUE(sgOgreConsole.execute(
      "ath sceneNode " + mBlankNode0Nameid + " camera sn0"));
  // ath ltl_tagPoint entity bone ltl_light id ("tag0")
  // ath ltl_tagPoint entity bone ltl_entity id mesh ("tag1")
  // ath ltl_tagPoint entity bone ltl_particle id pst ("tag2")
  // ath ltl_tagPoint entity bone ltl_camera id ("tag3")
  EXPECT_TRUE(sgOgreConsole.execute(
      "ath tagPoint " + mSinbadNameid + " IndexFingerDist.L light ebl0 "));
  EXPECT_TRUE(
      sgOgreConsole.execute("ath tagPoint " + mSinbadNameid +
                            " IndexFingerDist.L item ebe0 ogrehead.mesh"));
  //EXPECT_TRUE(
      //sgOgreConsole.execute("ath tagPoint " + mSinbadNameid +
                            //" IndexFingerDist.L particle ebp0 fireball"));
  EXPECT_TRUE(sgOgreConsole.execute(
      "ath tagPoint " + mSinbadNameid + " IndexFingerDist.L camera ebc0"));
}

TEST_F(TestOgreScene, testAthPrompt) {
  sgConsole.getUi()->setCmdLine("ath s");
  sgConsole.prompt();
  EXPECT_STREQ("ath sceneNode", getCmdLine().c_str());
  sgConsole.getUi()->setCmdLine("ath sceneNode ");
  sgConsole.prompt();
  EXPECT_STREQ("ath sceneNode ", getCmdLine().c_str());
  sgConsole.getUi()->setCmdLine("ath t");
  sgConsole.prompt();
  EXPECT_STREQ("ath tagPoint", getCmdLine().c_str());
  sgConsole.getUi()->setCmdLine("ath sceneNode ca");
  sgConsole.prompt();
  EXPECT_EQ("ath sceneNode " + mCameraNodeNameid, getCmdLine());
}

TEST_F(TestOgreScene, testDth0) {
  // dth moType movable... ("m0")
  EXPECT_TRUE(sgOgreConsole.execute(
      "dth Item " + mOgreHeadNameid + " " + mSinbadNameid));
  EXPECT_FALSE(OgreUtil::hasMovableObject(mSceneMgr, "ogrehead", "Item"));
  EXPECT_FALSE(OgreUtil::hasMovableObject(mSceneMgr, "sinbad", "Item"));
  EXPECT_TRUE(sgOgreConsole.execute("dth Light " + mLight0Nameid));
  EXPECT_FALSE(OgreUtil::hasMovableObject(mSceneMgr, "light0", "Light"));
  EXPECT_TRUE(sgOgreConsole.execute("dth Camera " + mCamera0Nameid));
  EXPECT_FALSE(OgreUtil::hasMovableObject(mSceneMgr, "camera0", "Camera"));
}
TEST_F(TestOgreScene, testDth1) {
  // dth ltl_sceneNode sceneNode ("sn0")
  // dth ltl_sceneNode sceneNode moType ("sn1")
  // dth ltl_sceneNode sceneNode moType movable("sn2")
  EXPECT_TRUE(sgOgreConsole.execute("dth sceneNode " + mEntNode0Nameid));
  EXPECT_TRUE(
      sgOgreConsole.execute("dth sceneNode " + mEntNode1Nameid + " Item"));
  EXPECT_TRUE(
      sgOgreConsole.execute("dth sceneNode " + mLightNodeNameid + " Light"));
  EXPECT_FALSE(OgreUtil::hasMovableObject(mSceneMgr, "light0", "Light"));
  EXPECT_TRUE(sgOgreConsole.execute(
      "dth sceneNode " + mCameraNodeNameid + " Camera " + mCamera0Nameid));
  EXPECT_FALSE(OgreUtil::hasMovableObject(mSceneMgr, "camera0", "Camera"));
}

TEST_F(TestOgreScene, testDth2) {
  // dth ltl_tagPoint entity ("tag0")
  // dth ltl_tagPoint entity moType  ("tag1")
  // dth ltl_tagPoint entity bone ("tag2")
  // dth ltl_tagPoint entity bone moType ("tag3")
  // dth ltl_tagPoint entity bone moType movable("tag4")
  // EXPECT_TRUE(
  // sgOgreConsole.execute("dth tagPoint sinbad Hand.R Entity rightSword"));
  // EXPECT_FALSE(OgreUtil::hasMovableObject(mSceneMgr, "rightSword",
  // "Entity"));
  // EXPECT_TRUE(sgOgreConsole.execute("dth tagPoint sinbad"));
  // EXPECT_TRUE(sgOgreConsole.execute("dth tagPoint sinbad Light"));
  // EXPECT_TRUE(sgOgreConsole.execute("dth tagPoint sinbad Hand.L"));
  // EXPECT_TRUE(sgOgreConsole.execute("dth tagPoint sinbad Hand.L Camera"));
}

TEST_F(TestOgreScene, testDthPrompt) {
  sgConsole.getUi()->setCmdLine("dth Item o");
  sgConsole.prompt();
  EXPECT_EQ("dth Item " + mOgreHeadNameid, getCmdLine());
  sgConsole.getUi()->setCmdLine(
      "dth sceneNode " + mEntNode0Nameid + " Item o");
  sgConsole.prompt();
  EXPECT_EQ("dth sceneNode " + mEntNode0Nameid + " Item " + mOgreHeadNameid,
      getCmdLine());
}

TEST_F(TestOgreScene, testEdmo) {
  EXPECT_TRUE(sgOgreConsole.execute("edmo Item " + mOgreHeadNameid));
  EXPECT_EQ(mOgreHeadNameid, sgOgreConsole.getCwd()->getName());
  EXPECT_TRUE(sgOgreConsole.execute(
      "edmo " + mEntNode1Nameid + " Item " + mSinbadNameid));
  EXPECT_EQ(mSinbadNameid, sgOgreConsole.getCwd()->getName());
  // EXPECT_TRUE(
  // sgOgreConsole.execute("edmo tagPoint sinbad Hand.R Entity rightSword"));
  // EXPECT_STREQ("rightSword", sgOgreConsole.getCwd()->getName().c_str());
}
}

#endif /* TESTOGRECONSOLE_H */
