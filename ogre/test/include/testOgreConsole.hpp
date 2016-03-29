#ifndef TESTOGRECONSOLE_H
#define TESTOGRECONSOLE_H

#include "OgreConsole.h"
#include "pacAbsDir.h"
#include <gtest/gtest.h>
#include <OgreSceneManager.h>
#include <limits>
#include <OgreEntity.h>
#include "pacIntrinsicArgHandler.h"
#include "../test/include/testConsoleUI.hpp"
#include "OgreUtil.h"
#include "OgreArgHandler.h"

namespace pac {

StringVector getSortedVector(std::initializer_list<std::string> items) {
  StringVector sv(items.begin(), items.end());
  std::sort(sv.begin(), sv.end());
  return sv;
}

class TestOgreConsole : public ::testing::Test {
protected:
  /**
   * build a simple scene to test ogre command
   * root->light(light0)
   *     ->camera(camera0)
   *     ->entRoot->en0(ogrehead)
   *              ->en1(sinbad)
   *     ->blank0
   *     ->blank1
   *     ->blank2(static)
   */
  void SetUp() {
    d = pac::delim;

    mSceneMgr = sgOgreConsole.getSceneMgr();
    mRootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mRootNode->setPosition(100, 200, 300);
    mRootNode->setName("root");
    mLightNode = mRootNode->createChildSceneNode();
    mLightNode->setName("light");
    mCameraNode = mRootNode->createChildSceneNode();
    mCameraNode->setName("camera");
    mEntRootNode = mRootNode->createChildSceneNode();
    mEntRootNode->setName("entRoot");
    mEntNode0 = mEntRootNode->createChildSceneNode();
    mEntNode0->setName("en0");
    mEntNode1 = mEntRootNode->createChildSceneNode();
    mEntNode1->setName("en1");
    mBlankNode0 = mRootNode->createChildSceneNode();
    mBlankNode0->setName("blank0");
    mBlankNode1 = mRootNode->createChildSceneNode();
    mBlankNode1->setName("blank1");
    mBlankNode2 = mRootNode->createChildSceneNode(Ogre::SCENE_STATIC);
    mBlankNode2->setName("blank2");
    mLight0 = mSceneMgr->createLight();
    mLight0->setName("light0");
    mCamera0 = mSceneMgr->createCamera("camera0");
    mOgreHead = mSceneMgr->createEntity("ogrehead.mesh");
    mOgreHead->setName("ogrehead");
    mSinbad = mSceneMgr->createEntity("sinbad.mesh");
    mSinbad->setName("sinbad");
    mLeftSword = mSceneMgr->createEntity("sword.mesh");
    mLeftSword->setName("leftSword");
    mRightSword = mSceneMgr->createEntity("sword.mesh");
    mRightSword->setName("rightSword");

    mCamera0->detachFromParent();
    mCameraNode->attachObject(mCamera0);
    mLightNode->attachObject(mLight0);
    mEntNode0->attachObject(mOgreHead);
    mEntNode1->attachObject(mSinbad);
    mSinbad->attachObjectToBone("Hand.R", mRightSword);
    mSinbad->attachObjectToBone("Hand.L", mLeftSword);

    mDynamicRootNodeNameid = OgreUtil::createNameid(
        mSceneMgr->getRootSceneNode(Ogre::SCENE_DYNAMIC));
    mStaticRootNodeNameid =
        OgreUtil::createNameid(mSceneMgr->getRootSceneNode(Ogre::SCENE_STATIC));
    mRootNodeNameid = OgreUtil::createNameid(mRootNode);
    mLightNodeNameid = OgreUtil::createNameid(mLightNode);
    mCameraNodeNameid = OgreUtil::createNameid(mCameraNode);
    mEntRootNodeNameid = OgreUtil::createNameid(mEntRootNode);
    mEntNode0Nameid = OgreUtil::createNameid(mEntNode0);
    mEntNode1Nameid = OgreUtil::createNameid(mEntNode1);
    mBlankNode0Nameid = OgreUtil::createNameid(mBlankNode0);
    mBlankNode1Nameid = OgreUtil::createNameid(mBlankNode1);
    mBlankNode2Nameid = OgreUtil::createNameid(mBlankNode2);

    mOgreHeadNameid = OgreUtil::createNameid(mOgreHead);
    mSinbadNameid = OgreUtil::createNameid(mSinbad);
    mLeftSwordNameid = OgreUtil::createNameid(mLeftSword);
    mRightSwordNameid = OgreUtil::createNameid(mRightSword);
    mLight0Nameid = OgreUtil::createNameid(mLight0);
    mCamera0Nameid = OgreUtil::createNameid(mCamera0);
    srand(static_cast<unsigned>(time(0)));
    mUi = static_cast<ImplConsoleUI*>(sgOgreConsole.getUi());
  }

  void TearDown() {
    mSceneMgr->destroyAllMovableObjects();
    mSceneMgr->destroyAllCameras();
    OgreUtil::destroySceneNodeTotally(mRootNode);
  }

  ImplConsoleUI* getImplUi() {
    return static_cast<ImplConsoleUI*>(sgConsole.getUi());
  }
  const std::string& getLastOutput() { return getImplUi()->getLastOutput(); }
  std::string getCwd() { return getImplUi()->getCwd(); }
  std::string getCmdLine() { return getImplUi()->getCmdLine(); }

  Ogre::SceneManager* mSceneMgr;
  Ogre::SceneNode* mRootNode, *mLightNode, *mCameraNode, *mEntRootNode,
      *mEntNode0, *mEntNode1, *mBlankNode0, *mBlankNode1, *mBlankNode2;
  Ogre::Light* mLight0;
  Ogre::Camera* mCamera0;
  Ogre::Entity* mOgreHead, *mSinbad, *mLeftSword, *mRightSword;
  ImplConsoleUI* mUi;
  std::string d;
  std::string mDynamicRootNodeNameid, mStaticRootNodeNameid, mRootNodeNameid,
      mLightNodeNameid, mCameraNodeNameid, mEntRootNodeNameid, mEntNode0Nameid,
      mEntNode1Nameid, mBlankNode0Nameid, mBlankNode1Nameid, mBlankNode2Nameid,
      mOgreHeadNameid, mSinbadNameid, mLeftSwordNameid, mRightSwordNameid,
      mLight0Nameid, mCamera0Nameid;
};

TEST_F(TestOgreConsole, testAddNode) {
  ASSERT_TRUE(
      sgOgreConsole.execute("adnd " + mRootNodeNameid + " SCENE_STATIC node0"));
  EXPECT_TRUE(OgreUtil::hasSceneNode(mSceneMgr, "node0"));
  ASSERT_TRUE(sgOgreConsole.execute(
      "adnd " + mRootNodeNameid + " SCENE_DYNAMIC node1"));
  EXPECT_TRUE(OgreUtil::hasSceneNode(mSceneMgr, "node1"));
}

TEST_F(TestOgreConsole, testEditNode) {
  ASSERT_TRUE(sgOgreConsole.execute("ednd " + mEntNode0Nameid));
  AbsDir* nodeRoot = sgRootDir.getChildByName("node");
  EXPECT_TRUE(nodeRoot->hasChild(mEntNode0Nameid));
}

TEST_F(TestOgreConsole, testRmNode) {
  ASSERT_TRUE(sgOgreConsole.execute("rmnd " + mBlankNode0Nameid));
  EXPECT_FALSE(OgreUtil::hasSceneNode(mSceneMgr, "blank0"));
  ASSERT_TRUE(sgOgreConsole.execute("rmnd " + mBlankNode1Nameid));
  EXPECT_FALSE(OgreUtil::hasSceneNode(mSceneMgr, "blank1"));
}

TEST_F(TestOgreConsole, testLsnd) {
  EXPECT_TRUE(sgOgreConsole.execute("lsnd"));
  EXPECT_EQ(getSortedVector({mDynamicRootNodeNameid, mStaticRootNodeNameid,
                mRootNodeNameid, mLightNodeNameid, mCameraNodeNameid,
                mEntRootNodeNameid, mEntNode0Nameid, mEntNode1Nameid,
                mBlankNode0Nameid, mBlankNode1Nameid, mBlankNode2Nameid}),
      mUi->getItems());
  EXPECT_TRUE(sgOgreConsole.execute("lsnd SCENE_STATIC"));
  EXPECT_EQ(getSortedVector({mStaticRootNodeNameid, mBlankNode2Nameid}),
      mUi->getItems());
  EXPECT_TRUE(sgOgreConsole.execute("lsnd SCENE_DYNAMIC"));
  EXPECT_EQ(getSortedVector(
                {mDynamicRootNodeNameid, mRootNodeNameid, mLightNodeNameid,
                 mCameraNodeNameid, mEntRootNodeNameid, mEntNode0Nameid,
                 mEntNode1Nameid, mBlankNode0Nameid, mBlankNode1Nameid}),
      mUi->getItems());
  EXPECT_TRUE(sgOgreConsole.execute("lsnd " + mRootNodeNameid));
  EXPECT_EQ(
      getSortedVector({mLightNodeNameid, mCameraNodeNameid, mEntRootNodeNameid,
          mBlankNode0Nameid, mBlankNode1Nameid, mBlankNode2Nameid}),
      mUi->getItems());
  EXPECT_TRUE(
      sgOgreConsole.execute("lsnd " + mRootNodeNameid + " SCENE_STATIC"));
  EXPECT_EQ(getSortedVector({mBlankNode2Nameid}), mUi->getItems());
  EXPECT_TRUE(
      sgOgreConsole.execute("lsnd " + mRootNodeNameid + " SCENE_DYNAMIC"));
  EXPECT_EQ(getSortedVector({mLightNodeNameid, mCameraNodeNameid,
                mEntRootNodeNameid, mBlankNode0Nameid, mBlankNode1Nameid}),
      mUi->getItems());
  EXPECT_TRUE(sgOgreConsole.execute("lsnd -r " + mRootNodeNameid));
  EXPECT_EQ(getSortedVector({mLightNodeNameid, mCameraNodeNameid,
                mEntRootNodeNameid, mEntNode0Nameid, mEntNode1Nameid,
                mBlankNode0Nameid, mBlankNode1Nameid, mBlankNode2Nameid}),
      mUi->getItems());
  EXPECT_TRUE(
      sgOgreConsole.execute("lsnd -r" + mRootNodeNameid + " SCENE_STATIC"));
  EXPECT_EQ(getSortedVector({mBlankNode2Nameid}), mUi->getItems());
  EXPECT_TRUE(
      sgOgreConsole.execute("lsnd -r" + mRootNodeNameid + " SCENE_DYNAMIC"));
  EXPECT_EQ(getSortedVector({mLightNodeNameid, mCameraNodeNameid,
                mEntRootNodeNameid, mEntNode0Nameid, mEntNode1Nameid,
                mBlankNode0Nameid, mBlankNode1Nameid}),
      mUi->getItems());
  EXPECT_TRUE(sgOgreConsole.execute("lsnd regex .*"));
  EXPECT_EQ(getSortedVector({mDynamicRootNodeNameid, mStaticRootNodeNameid,
                mRootNodeNameid, mLightNodeNameid, mCameraNodeNameid,
                mEntRootNodeNameid, mEntNode0Nameid, mEntNode1Nameid,
                mBlankNode0Nameid, mBlankNode1Nameid, mBlankNode2Nameid}),
      mUi->getItems());
  EXPECT_TRUE(sgOgreConsole.execute("lsnd regex .* SCENE_STATIC"));
  EXPECT_EQ(getSortedVector({mStaticRootNodeNameid, mBlankNode2Nameid}),
      mUi->getItems());
  EXPECT_TRUE(sgOgreConsole.execute("lsnd regex .* SCENE_DYNAMIC"));
  EXPECT_EQ(getSortedVector(
                {mDynamicRootNodeNameid, mRootNodeNameid, mLightNodeNameid,
                 mCameraNodeNameid, mEntRootNodeNameid, mEntNode0Nameid,
                 mEntNode1Nameid, mBlankNode0Nameid, mBlankNode1Nameid}),
      mUi->getItems());
}

TEST_F(TestOgreConsole, testLsres) {
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
}
TEST_F(TestOgreConsole, testLsresPrompt) {
  sgConsole.getUi()->setCmdLine("lsres material regex S");
  sgConsole.prompt();
  EXPECT_STREQ("lsres material regex S", getCmdLine().c_str());
}

TEST_F(TestOgreConsole, testLsmoOutput) {
  EXPECT_TRUE(sgOgreConsole.execute("lsmo Entity"));
  EXPECT_EQ(getSortedVector({mOgreHeadNameid, mSinbadNameid, mLeftSwordNameid,
                mRightSwordNameid}),
      mUi->getItems());
  EXPECT_TRUE(sgOgreConsole.execute("lsmo Entity regex .*"));
  EXPECT_EQ(getSortedVector({mOgreHeadNameid, mSinbadNameid, mLeftSwordNameid,
                mRightSwordNameid}),
      mUi->getItems());
  EXPECT_TRUE(sgOgreConsole.execute("lsmo sceneNode " + mEntNode0Nameid));
  EXPECT_EQ(getSortedVector({mOgreHeadNameid}), mUi->getItems());
  EXPECT_TRUE(sgOgreConsole.execute(
      "lsmo sceneNode parent " + mEntRootNodeNameid + " " + mEntNode0Nameid));
  EXPECT_EQ(getSortedVector({mOgreHeadNameid}), mUi->getItems());
  EXPECT_TRUE(
      sgOgreConsole.execute("lsmo sceneNode parent " + mEntRootNodeNameid +
                            " SCENE_DYNAMIC " + mEntNode0Nameid));
  EXPECT_EQ(getSortedVector({mOgreHeadNameid}), mUi->getItems());
  EXPECT_TRUE(
      sgOgreConsole.execute("lsmo sceneNode ancestor " + mEntRootNodeNameid +
                            " SCENE_DYNAMIC " + mEntNode0Nameid));
  EXPECT_EQ(getSortedVector({mOgreHeadNameid}), mUi->getItems());
  EXPECT_TRUE(
      sgOgreConsole.execute("lsmo sceneNode ancestor " + mRootNodeNameid +
                            " SCENE_DYNAMIC " + mEntNode0Nameid));
  EXPECT_EQ(getSortedVector({mOgreHeadNameid}), mUi->getItems());
  EXPECT_TRUE(
      sgOgreConsole.execute("lsmo sceneNode " + mEntNode0Nameid + " Entity"));
  EXPECT_EQ(getSortedVector({mOgreHeadNameid}), mUi->getItems());
}

TEST_F(TestOgreConsole, testLsmo) {
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
        EXPECT_TRUE(
            sgOgreConsole.execute("lsmo sceneNode parent " +
                                  mEntRootNodeNameid + " " + mEntNode0Nameid));
        EXPECT_TRUE(sgOgreConsole.execute("lsmo sceneNode parent " +
                                          mEntRootNodeNameid +
                                          " SCENE_DYNAMIC " + mEntNode0Nameid));
        EXPECT_TRUE(sgOgreConsole.execute("lsmo sceneNode ancestor " +
                                          mEntRootNodeNameid +
                                          " SCENE_DYNAMIC " + mEntNode0Nameid));
        EXPECT_TRUE(
            sgOgreConsole.execute("lsmo sceneNode ancestor " + mRootNodeNameid +
                                  " SCENE_DYNAMIC " + mEntNode0Nameid));
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

TEST_F(TestOgreConsole, testAth) {
  // ath ltl_sceneNode sceneNode ltl_light id ("sn0")
  // ath ltl_sceneNode sceneNode ltl_entity id mesh ("sn1")
  // ath ltl_sceneNode sceneNode ltl_particle id pst ("sn2")
  // ath ltl_sceneNode sceneNode ltl_camera id("sn3")
  EXPECT_TRUE(sgOgreConsole.execute(
      "ath sceneNode " + mBlankNode0Nameid + " light sn0 "));
  EXPECT_TRUE(sgOgreConsole.execute(
      "ath sceneNode " + mBlankNode0Nameid + " entity sn0 ogrehead.mesh"));
  EXPECT_TRUE(sgOgreConsole.execute(
      "ath sceneNode " + mBlankNode0Nameid + " particle sn0 fireball"));
  EXPECT_TRUE(sgOgreConsole.execute(
      "ath sceneNode " + mBlankNode0Nameid + " camera sn0"));
  // ath ltl_tagPoint entity bone ltl_light id ("tag0")
  // ath ltl_tagPoint entity bone ltl_entity id mesh ("tag1")
  // ath ltl_tagPoint entity bone ltl_particle id pst ("tag2")
  // ath ltl_tagPoint entity bone ltl_camera id ("tag3")
  // EXPECT_TRUE(sgOgreConsole.execute(
  //"ath tagPoint sinbad IndexFingerDist.L light ebl0 "));
  // EXPECT_FALSE(sgOgreConsole.execute(
  //"ath tagPoint sinbad IndexFingerDist.L light ebl0 "));
  // EXPECT_TRUE(sgOgreConsole.execute(
  //"ath tagPoint sinbad IndexFingerDist.L entity ebe0 ogrehead.mesh"));
  // EXPECT_FALSE(sgOgreConsole.execute(
  //"ath tagPoint sinbad IndexFingerDist.L entity ebe0 ogrehead.mesh"));
  // EXPECT_TRUE(sgOgreConsole.execute(
  //"ath tagPoint sinbad IndexFingerDist.L particle ebp0 fireball"));
  // EXPECT_FALSE(sgOgreConsole.execute(
  //"ath tagPoint sinbad IndexFingerDist.L particle ebp0 fireball"));
  // EXPECT_TRUE(sgOgreConsole.execute(
  //"ath tagPoint sinbad IndexFingerDist.L camera ebc0"));
  // EXPECT_FALSE(sgOgreConsole.execute(
  //"ath tagPoint sinbad IndexFingerDist.L camera ebc0"));
}

TEST_F(TestOgreConsole, testAthPrompt) {
  sgConsole.getUi()->setCmdLine("ath s");
  sgConsole.prompt();
  EXPECT_STREQ("ath sceneNode", getCmdLine().c_str());
  // sgConsole.getUi()->setCmdLine("ath t");
  // sgConsole.prompt();
  // EXPECT_STREQ("ath tagPoint", getCmdLine().c_str());
  sgConsole.getUi()->setCmdLine("ath sceneNode ca");
  sgConsole.prompt();
  EXPECT_EQ("ath sceneNode " + mCameraNodeNameid, getCmdLine());
}

TEST_F(TestOgreConsole, testDth0) {
  // dth moType movable... ("m0")
  EXPECT_TRUE(sgOgreConsole.execute(
      "dth Entity " + mOgreHeadNameid + " " + mSinbadNameid));
  EXPECT_FALSE(OgreUtil::hasMovableObject(mSceneMgr, "ogrehead", "Entity"));
  EXPECT_FALSE(OgreUtil::hasMovableObject(mSceneMgr, "sinbad", "Entity"));
  EXPECT_TRUE(sgOgreConsole.execute("dth Light " + mLight0Nameid));
  EXPECT_FALSE(OgreUtil::hasMovableObject(mSceneMgr, "light0", "Light"));
  EXPECT_TRUE(sgOgreConsole.execute("dth Camera " + mCamera0Nameid));
  EXPECT_FALSE(OgreUtil::hasMovableObject(mSceneMgr, "camera0", "Camera"));
}
TEST_F(TestOgreConsole, testDth1) {
  // dth ltl_sceneNode sceneNode ("sn0")
  // dth ltl_sceneNode sceneNode moType ("sn1")
  // dth ltl_sceneNode sceneNode moType movable("sn2")
  EXPECT_TRUE(sgOgreConsole.execute("dth sceneNode " + mEntNode0Nameid));
  EXPECT_TRUE(
      sgOgreConsole.execute("dth sceneNode " + mEntNode1Nameid + " Entity"));
  EXPECT_TRUE(
      sgOgreConsole.execute("dth sceneNode " + mLightNodeNameid + " Light"));
  EXPECT_FALSE(OgreUtil::hasMovableObject(mSceneMgr, "light0", "Light"));
  EXPECT_TRUE(sgOgreConsole.execute(
      "dth sceneNode " + mCameraNodeNameid + " Camera " + mCamera0Nameid));
  EXPECT_FALSE(OgreUtil::hasMovableObject(mSceneMgr, "camera0", "Camera"));
}

TEST_F(TestOgreConsole, testDth2) {
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

TEST_F(TestOgreConsole, testDthPrompt) {
  sgConsole.getUi()->setCmdLine("dth Entity o");
  sgConsole.prompt();
  EXPECT_EQ("dth Entity " + mOgreHeadNameid, getCmdLine());
  sgConsole.getUi()->setCmdLine(
      "dth sceneNode " + mEntNode0Nameid + " Entity o");
  sgConsole.prompt();
  EXPECT_EQ("dth sceneNode " + mEntNode0Nameid + " Entity " + mOgreHeadNameid,
      getCmdLine());
}

TEST_F(TestOgreConsole, testEdmo) {
  EXPECT_TRUE(sgOgreConsole.execute("edmo Entity " + mOgreHeadNameid));
  EXPECT_EQ(mOgreHeadNameid, sgOgreConsole.getCwd()->getName());
  EXPECT_TRUE(sgOgreConsole.execute(
      "edmo " + mEntNode1Nameid + " Entity " + mSinbadNameid));
  EXPECT_EQ(mSinbadNameid, sgOgreConsole.getCwd()->getName());
  // EXPECT_TRUE(
  // sgOgreConsole.execute("edmo tagPoint sinbad Hand.R Entity rightSword"));
  // EXPECT_STREQ("rightSword", sgOgreConsole.getCwd()->getName().c_str());
}
}

#endif /* TESTOGRECONSOLE_H */
