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

namespace pac {

/**
 * destroy scenenode, all it's children, and all the movable object attached
 * to them
 * @param node : target scenenode
 */
void destroySceneNodeTotally(Ogre::SceneNode* node) {
  Ogre::SceneManager* sceneMgr = node->getCreator();
  auto oi = node->getChildIterator();
  while (oi.hasMoreElements()) {
    Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(oi.getNext());
    destroySceneNodeTotally(node);
  }
  auto moi = node->getAttachedObjectIterator();
  while (moi.hasMoreElements()) {
    Ogre::MovableObject* mo = moi.getNext();
    if(mo->getMovableType() == "Camera")
      sceneMgr->destroyCamera(mo->getName());
    else
      sceneMgr->destroyMovableObject(mo);
  }
  sceneMgr->destroySceneNode(node);
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
   */
  void SetUp() {
    d = pac::delim;

    mSceneMgr = sgOgreConsole.getSceneMgr();
    mRootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("root");
    mLightNode = mRootNode->createChildSceneNode("light");
    mCameraNode = mRootNode->createChildSceneNode("camera");
    mEntRootNode = mRootNode->createChildSceneNode("entRoot");
    mEntNode0 = mEntRootNode->createChildSceneNode("en0");
    mEntNode1 = mEntRootNode->createChildSceneNode("en1");
    mBlankNode0 = mEntRootNode->createChildSceneNode("blank0");
    mBlankNode1 = mEntRootNode->createChildSceneNode("blank1");
    mLight0 = mSceneMgr->createLight("light0");
    mCamera0 = mSceneMgr->createCamera("camera0");
    mOgreHead = mSceneMgr->createEntity("ogrehead", "ogrehead.mesh");
    mSinbad = mSceneMgr->createEntity("sinbad", "sinbad.mesh");
    mLeftSword = mSceneMgr->createEntity("leftSword", "sword.mesh");
    mRightSword = mSceneMgr->createEntity("rightSword", "sword.mesh");

    mCameraNode->attachObject(mCamera0);
    mLightNode->attachObject(mLight0);
    mEntNode0->attachObject(mOgreHead);
    mEntNode1->attachObject(mSinbad);
    mSinbad->attachObjectToBone("Hand.R", mRightSword);
    mSinbad->attachObjectToBone("Hand.L", mLeftSword);

    srand(static_cast<unsigned>(time(0)));
  }

  void TearDown() {
    mSceneMgr->destroyAllMovableObjects();
    mSceneMgr->destroyAllCameras();
    destroySceneNodeTotally(mRootNode);
  }

  ImplConsoleUI* getImplUi() {
    return static_cast<ImplConsoleUI*>(sgConsole.getUi());
  }
  const std::string& getLastOutput() { return getImplUi()->getLastOutput(); }
  std::string getCwd() { return getImplUi()->getCwd(); }
  std::string getCmdLine() { return getImplUi()->getCmdLine(); }

  Ogre::SceneManager* mSceneMgr;
  Ogre::SceneNode* mRootNode, *mLightNode, *mCameraNode, *mEntRootNode,
      *mEntNode0, *mEntNode1, *mBlankNode0, *mBlankNode1;
  Ogre::Light* mLight0;
  Ogre::Camera* mCamera0;
  Ogre::Entity* mOgreHead, *mSinbad, *mLeftSword, *mRightSword;
  std::string d;
};


TEST_F(TestOgreConsole, testAddNode) {
  ASSERT_TRUE(sgOgreConsole.execute("adnd root node0"));
  EXPECT_TRUE(mSceneMgr->hasSceneNode("node0"));
  ASSERT_TRUE(sgOgreConsole.execute("adnd root node1"));
  EXPECT_TRUE(mSceneMgr->hasSceneNode("node1"));
  ASSERT_TRUE(sgOgreConsole.execute("adnd root1"));
  EXPECT_TRUE(mSceneMgr->hasSceneNode("root1"));
}

TEST_F(TestOgreConsole, testEditNode) {
  ASSERT_TRUE(sgOgreConsole.execute("ednd en0"));
  AbsDir* nodeRoot = sgRootDir.getChildByName("node");
  ASSERT_TRUE(nodeRoot->hasChild("en0"));
}

TEST_F(TestOgreConsole, testRmNode) {
  ASSERT_TRUE(sgOgreConsole.execute("rmnd blank0"));
  EXPECT_FALSE(mSceneMgr->hasSceneNode("blank0"));
  ASSERT_TRUE(sgOgreConsole.execute("rmnd blank1"));
  EXPECT_FALSE(mSceneMgr->hasSceneNode("blank1"));
}

TEST_F(TestOgreConsole, testLsnd) {
  EXPECT_TRUE(sgOgreConsole.execute("lsnd"));
  EXPECT_TRUE(sgOgreConsole.execute("lsnd en0"));
  EXPECT_TRUE(sgOgreConsole.execute("lsnd direct root"));
  EXPECT_TRUE(sgOgreConsole.execute("lsnd regex .*"));
}

TEST_F(TestOgreConsole, testLsres) {
  EXPECT_TRUE(sgOgreConsole.execute("lsres material"));
  EXPECT_TRUE(sgOgreConsole.execute("lsres material regex .*"));
  EXPECT_TRUE(sgOgreConsole.execute("lsres mesh"));
  EXPECT_TRUE(sgOgreConsole.execute("lsres mesh regex .*"));
  EXPECT_TRUE(sgOgreConsole.execute("lsres texture"));
  EXPECT_TRUE(sgOgreConsole.execute("lsres texture regex .*"));
  EXPECT_TRUE(sgOgreConsole.execute("lsres compositor"));
  EXPECT_TRUE(sgOgreConsole.execute("lsres compositor regex .*"));
  EXPECT_TRUE(sgOgreConsole.execute("lsres pst"));
  EXPECT_TRUE(sgOgreConsole.execute("lsres pst regex .*"));
}
TEST_F(TestOgreConsole, testLsresPrompt) {
  sgConsole.getUi()->setCmdLine("lsres material regex S");
  sgConsole.prompt();
  EXPECT_STREQ("lsres material regex S", getCmdLine().c_str());
}

TEST_F(TestOgreConsole, testLsmo) {
  StringArgHandler* handler =
      static_cast<StringArgHandler*>(sgArgLib.createArgHandler("moType"));
  std::for_each(handler->beginStringIter(), handler->endStringIter(),
      [&](const std::string& moType) -> void {
        // lsmo moType ("g0")
        // lsmo moType ltl_regex regex ("g1")
        // lsmo ltl_sceneNode sceneNode ("sn0")
        // lsmo ltl_sceneNode sceneNode moType ("sn1")
        // lsmo ltl_tagPoint entity ("tag0")
        // lsmo ltl_tagPoint entity moType ("tag1")
        // lsmo ltl_tagPoint entity bone ("tag2")
        // lsmo ltl_tagPoint entity bone moType ("tag3")
        EXPECT_TRUE(sgOgreConsole.execute("lsmo " + moType));
        EXPECT_TRUE(sgOgreConsole.execute("lsmo " + moType + " regex .*"));
        EXPECT_TRUE(sgOgreConsole.execute("lsmo sceneNode en0 "));
        EXPECT_TRUE(sgOgreConsole.execute("lsmo sceneNode en0 " + moType));
        EXPECT_TRUE(sgOgreConsole.execute("lsmo tagPoint sinbad "));
        EXPECT_TRUE(sgOgreConsole.execute("lsmo tagPoint sinbad " + moType));
        EXPECT_TRUE(
            sgOgreConsole.execute("lsmo tagPoint sinbad IndexFingerDist.L "));
        EXPECT_TRUE(sgOgreConsole.execute(
            "lsmo tagPoint sinbad IndexFingerDist.L " + moType));
      });
}

TEST_F(TestOgreConsole, testAth) {
  // ath ltl_sceneNode sceneNode ltl_light id ("sn0")
  // ath ltl_sceneNode sceneNode ltl_entity id mesh ("sn1")
  // ath ltl_sceneNode sceneNode ltl_particle id pst ("sn2")
  // ath ltl_sceneNode sceneNode ltl_camera id("sn3")
  EXPECT_TRUE(sgOgreConsole.execute("ath sceneNode blank0 light sn0 "));
  EXPECT_FALSE(sgOgreConsole.execute("ath sceneNode blank0 light sn0 "));
  EXPECT_TRUE(
      sgOgreConsole.execute("ath sceneNode blank0 entity sn0 ogrehead.mesh"));
  EXPECT_FALSE(
      sgOgreConsole.execute("ath sceneNode blank0 entity sn0 ogrehead.mesh"));
  EXPECT_TRUE(
      sgOgreConsole.execute("ath sceneNode blank0 particle sn0 fireball"));
  EXPECT_FALSE(
      sgOgreConsole.execute("ath sceneNode blank0 particle sn0 fireball"));
  EXPECT_TRUE(sgOgreConsole.execute("ath sceneNode blank0 camera sn0"));
  EXPECT_FALSE(sgOgreConsole.execute("ath sceneNode blank0 camera sn0"));
  // ath ltl_tagPoint entity bone ltl_light id ("tag0")
  // ath ltl_tagPoint entity bone ltl_entity id mesh ("tag1")
  // ath ltl_tagPoint entity bone ltl_particle id pst ("tag2")
  // ath ltl_tagPoint entity bone ltl_camera id ("tag3")
  EXPECT_TRUE(sgOgreConsole.execute(
      "ath tagPoint sinbad IndexFingerDist.L light ebl0 "));
  EXPECT_FALSE(sgOgreConsole.execute(
      "ath tagPoint sinbad IndexFingerDist.L light ebl0 "));
  EXPECT_TRUE(sgOgreConsole.execute(
      "ath tagPoint sinbad IndexFingerDist.L entity ebe0 ogrehead.mesh"));
  EXPECT_FALSE(sgOgreConsole.execute(
      "ath tagPoint sinbad IndexFingerDist.L entity ebe0 ogrehead.mesh"));
  EXPECT_TRUE(sgOgreConsole.execute(
      "ath tagPoint sinbad IndexFingerDist.L particle ebp0 fireball"));
  EXPECT_FALSE(sgOgreConsole.execute(
      "ath tagPoint sinbad IndexFingerDist.L particle ebp0 fireball"));
  EXPECT_TRUE(sgOgreConsole.execute(
      "ath tagPoint sinbad IndexFingerDist.L camera ebc0"));
  EXPECT_FALSE(sgOgreConsole.execute(
      "ath tagPoint sinbad IndexFingerDist.L camera ebc0"));
}

TEST_F(TestOgreConsole, testAthPrompt) {
  sgConsole.getUi()->setCmdLine("ath s");
  sgConsole.prompt();
  EXPECT_STREQ("ath sceneNode", getCmdLine().c_str());
  sgConsole.getUi()->setCmdLine("ath t");
  sgConsole.prompt();
  EXPECT_STREQ("ath tagPoint", getCmdLine().c_str());
  sgConsole.getUi()->setCmdLine("ath sceneNode ca");
  sgConsole.prompt();
  EXPECT_STREQ("ath sceneNode camera", getCmdLine().c_str());
}


TEST_F(TestOgreConsole, testDth0) {
  // dth moType movable... ("m0")
  EXPECT_TRUE(sgOgreConsole.execute("dth Entity ogrehead sinbad"));
  EXPECT_FALSE(mSceneMgr->hasMovableObject("ogrehead", "Entity"));
  EXPECT_FALSE(mSceneMgr->hasMovableObject("sinbad", "Entity"));
  EXPECT_TRUE(sgOgreConsole.execute("dth Light light0"));
  EXPECT_FALSE(mSceneMgr->hasMovableObject("light0", "Light"));
  EXPECT_TRUE(sgOgreConsole.execute("dth Camera camera0"));
  EXPECT_FALSE(mSceneMgr->hasMovableObject("camera0", "Camera"));
}
TEST_F(TestOgreConsole, testDth1) {
  // dth ltl_sceneNode sceneNode ("sn0")
  // dth ltl_sceneNode sceneNode moType ("sn1")
  // dth ltl_sceneNode sceneNode moType movable("sn2")
  EXPECT_TRUE(sgOgreConsole.execute("dth sceneNode en0"));
  EXPECT_TRUE(sgOgreConsole.execute("dth sceneNode en1 Entity"));
  EXPECT_TRUE(sgOgreConsole.execute("dth sceneNode light Light"));
  EXPECT_FALSE(mSceneMgr->hasMovableObject("light0", "Light"));
  EXPECT_TRUE(sgOgreConsole.execute("dth sceneNode camera Camera camera0"));
  EXPECT_FALSE(mSceneMgr->hasMovableObject("camera0", "Camera"));
}

TEST_F(TestOgreConsole, testDth2) {
  // dth ltl_tagPoint entity ("tag0")
  // dth ltl_tagPoint entity moType  ("tag1")
  // dth ltl_tagPoint entity bone ("tag2")
  // dth ltl_tagPoint entity bone moType ("tag3")
  // dth ltl_tagPoint entity bone moType movable("tag4")
  EXPECT_TRUE(
      sgOgreConsole.execute("dth tagPoint sinbad Hand.R Entity rightSword"));
  EXPECT_FALSE(mSceneMgr->hasMovableObject("rightSword", "Entity"));
  EXPECT_TRUE(sgOgreConsole.execute("dth tagPoint sinbad"));
  EXPECT_TRUE(sgOgreConsole.execute("dth tagPoint sinbad Light"));
  EXPECT_TRUE(sgOgreConsole.execute("dth tagPoint sinbad Hand.L"));
  EXPECT_TRUE(sgOgreConsole.execute("dth tagPoint sinbad Hand.L Camera"));
}

TEST_F(TestOgreConsole, testDthPrompt) {
  sgConsole.getUi()->setCmdLine("dth Entity o");
  sgConsole.prompt();
  EXPECT_STREQ("dth Entity ogrehead", getCmdLine().c_str());
  sgConsole.getUi()->setCmdLine("dth sceneNode en0 Entity o");
  sgConsole.prompt();
  EXPECT_STREQ("dth sceneNode en0 Entity ogrehead", getCmdLine().c_str());
}



TEST_F(TestOgreConsole, testEdmo) {
  EXPECT_TRUE(sgOgreConsole.execute("edmo Entity ogrehead"));
  EXPECT_STREQ("ogrehead", sgOgreConsole.getCwd()->getName().c_str());
  EXPECT_TRUE(sgOgreConsole.execute("edmo sceneNode en1 Entity sinbad"));
  EXPECT_STREQ("sinbad", sgOgreConsole.getCwd()->getName().c_str());
  EXPECT_TRUE(
      sgOgreConsole.execute("edmo tagPoint sinbad Hand.R Entity rightSword"));
  EXPECT_STREQ("rightSword", sgOgreConsole.getCwd()->getName().c_str());
}



}

#endif /* TESTOGRECONSOLE_H */
