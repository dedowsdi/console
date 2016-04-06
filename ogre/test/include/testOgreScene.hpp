#ifndef TESTOGRESCENE_HPP
#define TESTOGRESCENE_HPP

#include "pacConsolePreRequisite.h"
#include "pacAbsDir.h"
#include "pacArgHandler.h"
#include "../test/include/testConsoleUI.hpp"
#include "OgreConsole.h"
#include <gtest/gtest.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreItem.h>
#include <OgreCamera.h>
#include <OgreParticleSystem.h>
#include <OgreUtil.h>
#include <limits>

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
Ogre::Vector3 randUnitVector3() {
  return Ogre::Vector3(Ogre::Math::UnitRandom(), Ogre::Math::UnitRandom(),
      Ogre::Math::UnitRandom());
}
Ogre::Vector4 randVector4() {
  return Ogre::Vector4(randReal(), randReal(), randReal(), randReal());
}
Ogre::Vector4 randUnitVector4() {
  return Ogre::Vector4(Ogre::Math::UnitRandom(), Ogre::Math::UnitRandom(),
      Ogre::Math::UnitRandom(), Ogre::Math::UnitRandom());
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
std::string randUnitVector4Str() {
  return Ogre::StringConverter::toString(randUnitVector4());
}

Ogre::Quaternion randQuaternion(bool normalise = false) {
  auto q = Ogre::Quaternion(randReal(), randReal(), randReal(), randReal());
  if (normalise) q.normalise();
  return q;
}

std::string randQuaternionStr(bool normalise = false) {
  return Ogre::StringConverter::toString(randQuaternion(normalise));
}

static const Real epsilon = 0.01;
bool cmpReal(Real lhs, Real rhs) {
  Real d = lhs - rhs;
  return d >= -epsilon && d <= epsilon;
}

bool cmpRealStrings(const std::string& lhs, const std::string& rhs) {
  if (lhs.empty() || rhs.empty())
    PAC_EXCEPT(Exception::ERR_INVALIDPARAMS, "empty arg");
  auto l = StringUtil::split(lhs);
  auto r = StringUtil::split(lhs);
  if (l.size() != r.size())
    PAC_EXCEPT(Exception::ERR_INVALIDPARAMS,
        "wrong size, lhs:" + lhs + ", rhs:" + rhs);

  for (size_t i = 0; i < l.size(); ++i) {
    if (!cmpReal(StringUtil::parseReal(l[i]), StringUtil::parseReal(r[i])))
      return false;
  }

  return true;
}

class TestOgreScene : public ::testing::Test {
protected:
  /**
   * build a simple scene to test ogre command
   * root->light<-light0
   *     ->camera<-camera0
   *     ->entRoot->en0<-ogrehead
   *              ->en1<-sinbad
   *     ->particle<-particle0
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
    mParticleNode = mRootNode->createChildSceneNode();
    mParticleNode->setName("particle");
    mLight0 = mSceneMgr->createLight();
    mLight0->setName("light0");
    mCamera0 = mSceneMgr->createCamera("camera0");
    mOgreHead = mSceneMgr->createItem("ogrehead.mesh");
    mOgreHead->setName("ogrehead");
    mSinbad = mSceneMgr->createItem("sinbad.mesh");
    mSinbad->setName("sinbad");
    mLeftSword = mSceneMgr->createItem("sword.mesh");
    mLeftSword->setName("leftSword");
    mRightSword = mSceneMgr->createItem("sword.mesh");
    mRightSword->setName("rightSword");
    mParticle0 = mSceneMgr->createParticleSystem("fireball");
    mParticle0->setName("particle0");

    mCamera0->detachFromParent();
    mCameraNode->attachObject(mCamera0);
    mLightNode->attachObject(mLight0);
    mEntNode0->attachObject(mOgreHead);
    mEntNode1->attachObject(mSinbad);
    //mSinbad->attachObjectToBone("Hand.R", mRightSword);
    //mSinbad->attachObjectToBone("Hand.L", mLeftSword);

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
    mParticleNodeNameid = OgreUtil::createNameid(mParticleNode);

    mOgreHeadNameid = OgreUtil::createNameid(mOgreHead);
    mSinbadNameid = OgreUtil::createNameid(mSinbad);
    mLeftSwordNameid = OgreUtil::createNameid(mLeftSword);
    mRightSwordNameid = OgreUtil::createNameid(mRightSword);
    mLight0Nameid = OgreUtil::createNameid(mLight0);
    mCamera0Nameid = OgreUtil::createNameid(mCamera0);
    mParticle0Nameid = OgreUtil::createNameid(mParticle0);
    srand(static_cast<unsigned>(time(0)));
    mUi = static_cast<ImplConsoleUI*>(sgOgreConsole.getUi());
  }

  void TearDown() {
    mSceneMgr->destroyAllMovableObjects();
    mSceneMgr->destroyAllCameras();
    sgConsole.cleanTempDirs();
    OgreUtil::destroySceneNodeTotally(mRootNode);
    sgConsole.setCwd(&sgRootDir);
  }

  ImplConsoleUI* getImplUi() {
    return static_cast<ImplConsoleUI*>(sgConsole.getUi());
  }
  const std::string& getLastOutput() { return getImplUi()->getLastOutput(); }
  std::string getCwd() { return getImplUi()->getCwd(); }
  std::string getCmdLine() { return getImplUi()->getCmdLine(); }

  Ogre::SceneManager* mSceneMgr;
  Ogre::SceneNode* mRootNode, *mLightNode, *mCameraNode, *mEntRootNode,
      *mEntNode0, *mEntNode1, *mBlankNode0, *mBlankNode1, *mBlankNode2,
      *mParticleNode;
  Ogre::Light* mLight0;
  Ogre::Camera* mCamera0;
  Ogre::Item* mOgreHead, *mSinbad, *mLeftSword, *mRightSword;
  Ogre::ParticleSystem* mParticle0;
  ImplConsoleUI* mUi;
  std::string d;
  std::string mDynamicRootNodeNameid, mStaticRootNodeNameid, mRootNodeNameid,
      mLightNodeNameid, mCameraNodeNameid, mEntRootNodeNameid, mEntNode0Nameid,
      mEntNode1Nameid, mBlankNode0Nameid, mBlankNode1Nameid, mBlankNode2Nameid,
      mParticleNodeNameid, mOgreHeadNameid, mSinbadNameid, mLeftSwordNameid,
      mRightSwordNameid, mLight0Nameid, mCamera0Nameid, mParticle0Nameid;
};
}

#endif /* TESTOGRESCENE_HPP */
