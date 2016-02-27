#ifndef TESTPACARGHANDLER_H
#define TESTPACARGHANDLER_H

#include "pacArgHandler.h"
#include "pacIntrinsicArgHandler.h"
#include "pacStringUtil.h"
#include <limits>
#include <gtest/gtest.h>

using namespace pac;

//test every arghandler except path, param, pparam, value, they will be tested
//at testConsole.hpp.
class TestArgHandler : public ::testing::Test {
protected:
  virtual void SetUp() {
    mBoolTrue = "true";
    mBoolFalse = "false";
    mShortMax = StringUtil::toString(std::numeric_limits<short>::max());
    mUshortMax =
        StringUtil::toString(std::numeric_limits<unsigned short>::max());
    mIntMax = StringUtil::toString(std::numeric_limits<int>::max());
    mUintMax = StringUtil::toString(std::numeric_limits<unsigned int>::max());
    mLongMax = StringUtil::toString(std::numeric_limits<long>::max());
    mUlongMax = StringUtil::toString(std::numeric_limits<unsigned long>::max());
    mShortMin = StringUtil::toString(std::numeric_limits<short>::min());
    mUshortMin =
        StringUtil::toString(std::numeric_limits<unsigned short>::min());
    mIntMin = StringUtil::toString(std::numeric_limits<int>::min());
    mUintMin = StringUtil::toString(std::numeric_limits<unsigned int>::min());
    mLongMin = StringUtil::toString(std::numeric_limits<long>::min());
    mUlongMin = StringUtil::toString(std::numeric_limits<unsigned long>::min());
    mReal = StringUtil::toString(3.1415926);
    mNreal = StringUtil::toString(-0.999);
    mNpreal = StringUtil::toString(0.999);
    mInt2 = "1 2";
    mInt3 = "1 2 3";
    mInt4 = "1 2 3 4";
    mInt5 = "1 2 3 4 5";
    mReal2 = "1.02 2.03";
    mReal3 = "1.02 2.03 3.03";
    mReal4 = "1.02 2.03 3.03 4.04";
    mReal5 = "1.02 2.03 3.03 4.04 5.05";
    mNreal2 = "0.02 0.93";
    mNreal3 = "0.82 0.03 -0.03";
    mNreal4 = "0.02 0.53 0.03 -0.04";
    mNreal5 = "0.02 0.83 0.63 0.94 -0.85";
    mMatrix2 = "2.02 -3.03 4.04 5.08";
    mMatrix3 = mMatrix2 + " " + mMatrix2 + " -9.09";
    mMatrix4 = mMatrix3 + " " + mReal5 + " " + mReal2;
    mNmatrix2 = "0.82 -0.73 0.04 -0.08";
    mNmatrix3 = mNmatrix2 + " " + mNmatrix2 + " -0.99";
    mNmatrix4 = mNmatrix3 + " " + mNreal5 + " " + mNreal2;
    mBlank = "";
    mCmdCd = "cd";
    mCmdLp = "lp";
    mCmdLs = "ls";
    mCmdPwd = "pwd";
    mCmdSet = "set";

    mMap["boolTrue"] = mBoolTrue;
    mMap["boolFalse"] = mBoolFalse;
    mMap["shortMax"] = mShortMax;
    mMap["ushortMax"] = mUshortMax;
    mMap["intMax"] = mIntMax;
    mMap["uintMax"] = mUintMax;
    mMap["longMax"] = mLongMax;
    mMap["ulongMax"] = mUlongMax;
    mMap["shortMin"] = mShortMin;
    mMap["ushortMin"] = mUshortMin;
    mMap["intMin"] = mIntMin;
    mMap["uintMin"] = mUintMin;
    mMap["longMin"] = mLongMin;
    mMap["ulongMin"] = mUlongMin;
    mMap["real"] = mReal;
    mMap["nreal"] = mNreal;
    mMap["npreal"] = mNpreal;
    mMap["int2"] = mInt2;
    mMap["int3"] = mInt3;
    mMap["int4"] = mInt4;
    mMap["int5"] = mInt5;
    mMap["real2"] = mReal2;
    mMap["real3"] = mReal3;
    mMap["real4"] = mReal4;
    mMap["real5"] = mReal5;
    mMap["nreal2"] = mNreal2;
    mMap["nreal3"] = mNreal3;
    mMap["nreal4"] = mNreal4;
    mMap["nreal5"] = mNreal5;
    mMap["matrix2"] = mMatrix2;
    mMap["matrix3"] = mMatrix3;
    mMap["matrix4"] = mMatrix4;
    mMap["nmatrix2"] = mNmatrix2;
    mMap["nmatrix3"] = mNmatrix3;
    mMap["nmatrix4"] = mNmatrix4;
    mMap["blank"] = "";
    mMap["cmdCd"] = mCmdCd;
    mMap["cmdLp"] = mCmdLp;
    mMap["cmdLs"] = mCmdLs;
    mMap["cmdPwd"] = mCmdPwd;
    mMap["cmdSet"] = mCmdSet;

    Node* root;
    mp2IntBool = new TreeArgHandler("IntBool");
    root = mp2IntBool->getRoot();
    root->addChildNode("intNode", "int")->endBranch("0");
    root->addChildNode("boolNode", "bool")->endBranch("1");
    mp2IntInt2 = new TreeArgHandler("IntInt2");
    root = mp2IntInt2->getRoot();
    root->addChildNode("intNode", "int")->endBranch("0");
    root->addChildNode("int2Node", "int2")->endBranch("1");
    mp3BoolReal3Matrix2 = new TreeArgHandler("BoolReal3Matrix2");
    root = mp3BoolReal3Matrix2->getRoot();
    root->addChildNode("boolNode", "bool")->endBranch("0");
    root->addChildNode("real3Node", "real3")->endBranch("1");
    root->addChildNode("matrix2Node", "matrix2")->endBranch("2");
    mp3Int3Nreal3Matrix3 = new TreeArgHandler("Int3Nreal3Matrix3");
    root = mp3Int3Nreal3Matrix3->getRoot();
    root->addChildNode("int3Node", "int3")->endBranch("0");
    root->addChildNode("nreal3Node", "nreal3")->endBranch("1");
    root->addChildNode("matrix3Node", "matrix3")->endBranch("2");
    mp4BoolInt2Nreal3Matrix4 = new TreeArgHandler("BoolInt2Nreal3Matrix4");
    root = mp4BoolInt2Nreal3Matrix4->getRoot();
    root->addChildNode("boolNode", "bool")->endBranch("0");
    root->addChildNode("int2Node", "int2")->endBranch("1");
    root->addChildNode("nreal3Node", "nreal3")->endBranch("2");
    root->addChildNode("matrix4Node", "matrix4")->endBranch("3");
    ms3BoolBoolBool = new TreeArgHandler("BoolBoolBool");
    root = ms3BoolBoolBool->getRoot();
    root->addChildNode("boolNode0", "bool")
        ->addChildNode("boolNode1", "bool")
        ->addChildNode("boolNode2", "bool")
        ->endBranch("0");
    ms2Real3Real3Real3 = new TreeArgHandler("Real3Real3Real3");
    root = ms2Real3Real3Real3->getRoot();
    root->addChildNode("real3Node0", "real3")
        ->addChildNode("real3Node1", "real3")
        ->addChildNode("real3Node2", "real3")
        ->endBranch("0");
    ml1real3 = new TreeArgHandler("LoopReal3");
    root = ml1real3->getRoot();
    root->addChildNode("real3LoopNode", "real3", Node::NT_LOOP)->endBranch("0");
    ml2real3bool = new TreeArgHandler("LoopReal3Bool");
    root = ml2real3bool->getRoot();
    root->addChildNode("real3LoopNode", "real3", Node::NT_LOOP)
        ->addChildNode("boolLoopNode", "bool", Node::NT_LOOP)
        ->endBranch("0");
  }

  virtual void TearDown() {
    delete mp2IntBool;
    delete mp2IntInt2;
    delete mp3BoolReal3Matrix2;
    delete mp3Int3Nreal3Matrix3;
    delete mp4BoolInt2Nreal3Matrix4;
    delete ms3BoolBoolBool;
    delete ms2Real3Real3Real3;
    delete ml1real3;
    delete ml2real3bool;
  }

  /**
   * return true if should use EXCEPT_TRUE
   * @param beg : begin item
   * @param end : end item
   * @param maskPass : mask pass or fail
   * @param itemName : key name in mMap
   * @return :
   */
  template <typename _InputIterator>
  bool ifUseTrue(const std::string& itemName, _InputIterator beg,
      _InputIterator end, bool maskPass) {
    _InputIterator iter = std::find(beg, end, itemName);
    return (maskPass && iter != end) || (!maskPass && iter == end);
  }

  template <typename _InputIterator>
  void test(const std::string& ahName, _InputIterator beg, _InputIterator end,
      bool maskPass = true) {
    ArgHandler* handler = sgArgLib.createArgHandler(ahName);
    test(handler, beg, end, maskPass);
    delete handler;
  }

  /**
   * Only item between beg and end should pass or fail
   * @param beg : begin item
   * @param end : end item
   * @param maskPass : mask pass or fail.
   */
  template <typename _InputIterator>
  void test(ArgHandler* handler, _InputIterator beg, _InputIterator end,
      bool maskPass = true) {
    // it's expanded on purpose, it's clear to find out which item is
    // wrong this way.

    // make no typo
    std::for_each(beg, end, [&](const std::string& s) -> void {
      if (mMap.find(s) == mMap.end())
        PAC_EXCEPT(Exception::ERR_INVALIDPARAMS, s + " not found in data map");
    });

    std::string itemName;

    itemName = "boolTrue";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "boolFalse";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "shortMax";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "ushortMax";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "intMax";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "uintMax";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "longMax";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "ulongMax";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "shortMin";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "ushortMin";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "intMin";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "uintMin";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "longMin";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "ulongMin";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "real";  // make it not nreal
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "nreal";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "npreal";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "int2";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "int3";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "int4";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "int5";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "real2";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "real3";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "real4";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "real5";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "nreal2";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "nreal3";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "nreal4";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "nreal5";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "matrix2";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "matrix3";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "matrix4";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "nmatrix2";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "nmatrix3";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "blank";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "cmdCd";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "cmdLs";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "cmdLp";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "cmdPwd";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
    itemName = "cmdSet";
    if (ifUseTrue(itemName, beg, end, maskPass))
      EXPECT_TRUE(handler->validate(mMap[itemName]));
    else
      EXPECT_FALSE(handler->validate(mMap[itemName]));
  }

  std::string mBoolTrue;
  std::string mBoolFalse;
  std::string mShortMax;
  std::string mUshortMax;
  std::string mIntMax;
  std::string mUintMax;
  std::string mLongMax;
  std::string mUlongMax;
  std::string mShortMin;
  std::string mUshortMin;
  std::string mIntMin;
  std::string mUintMin;
  std::string mLongMin;
  std::string mUlongMin;
  std::string mReal;  // make it not nreal
  std::string mNreal;
  std::string mNpreal;
  std::string mInt2;
  std::string mInt3;
  std::string mInt4;
  std::string mInt5;
  std::string mReal2;
  std::string mReal3;
  std::string mReal4;
  std::string mReal5;
  std::string mNreal2;
  std::string mNreal3;
  std::string mNreal4;
  std::string mNreal5;
  std::string mMatrix2;
  std::string mMatrix3;
  std::string mMatrix4;
  std::string mNmatrix2;
  std::string mNmatrix3;
  std::string mNmatrix4;
  std::string mBlank;
  std::string mCmdCd;
  std::string mCmdLp;
  std::string mCmdLs;
  std::string mCmdPwd;
  std::string mCmdSet;

  StrStrMap mMap;

  // p as parallel, s as sequence, l as loop
  TreeArgHandler* mp2IntBool;
  TreeArgHandler* mp2IntInt2;
  TreeArgHandler* mp3BoolReal3Matrix2;
  TreeArgHandler* mp3Int3Nreal3Matrix3;
  TreeArgHandler* mp4BoolInt2Nreal3Matrix4;
  TreeArgHandler* ms3BoolBoolBool;
  TreeArgHandler* ms2Real3Real3Real3;
  TreeArgHandler* ml1real3;
  TreeArgHandler* ml2real3bool;
};

TEST_F(TestArgHandler, bool) {
  StringVector sv;
  sv.push_back("boolTrue");
  sv.push_back("boolFalse");
  test("bool", sv.begin(), sv.end(), 1);
}
TEST_F(TestArgHandler, short) {
  StringVector sv;
  sv.push_back("shortMax");
  sv.push_back("shortMin");
  sv.push_back("ushortMin");
  sv.push_back("uintMin");
  sv.push_back("ulongMin");
  test("short", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, ushort) {
  StringVector sv;
  sv.push_back("shortMax");
  sv.push_back("shortMin");
  sv.push_back("ushortMax");
  sv.push_back("ushortMin");
  sv.push_back("uintMin");
  sv.push_back("ulongMin");
  test("ushort", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, int) {
  StringVector sv;
  sv.push_back("shortMax");
  sv.push_back("shortMin");
  sv.push_back("ushortMax");
  sv.push_back("ushortMin");
  sv.push_back("intMax");
  sv.push_back("intMin");
  sv.push_back("uintMin");
  sv.push_back("ulongMin");
  if (mIntMax == mLongMax)  // might depend on machine ?
  {
    sv.push_back("longMin");
    sv.push_back("longMax");
  }
  test("int", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, uint) {
  StringVector sv;
  sv.push_back("shortMax");
  sv.push_back("shortMin");
  sv.push_back("ushortMax");
  sv.push_back("ushortMin");
  sv.push_back("intMax");
  sv.push_back("intMin");
  sv.push_back("uintMin");
  sv.push_back("uintMax");
  sv.push_back("ulongMin");
  if (mIntMax == mLongMax)  // depend on machine
  {
    sv.push_back("longMin");
    sv.push_back("longMax");
    sv.push_back("ulongMax");
  }
  test("uint", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, long) {
  StringVector sv;
  sv.push_back("shortMax");
  sv.push_back("shortMin");
  sv.push_back("ushortMax");
  sv.push_back("ushortMin");
  sv.push_back("intMax");
  sv.push_back("intMin");
  sv.push_back("uintMin");
  sv.push_back("longMin");
  sv.push_back("longMax");
  sv.push_back("ulongMin");
  if (mIntMax != mLongMax)  // depend on machine
  {
    sv.push_back("uintMax");
  }
  test("long", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, ulong) {
  StringVector sv;
  sv.push_back("shortMax");
  sv.push_back("shortMin");
  sv.push_back("ushortMax");
  sv.push_back("ushortMin");
  sv.push_back("intMax");
  sv.push_back("intMin");
  sv.push_back("uintMin");
  sv.push_back("uintMax");
  sv.push_back("longMax");
  sv.push_back("longMin");
  sv.push_back("ulongMin");
  sv.push_back("ulongMax");
  test("ulong", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, real) {
  StringVector sv;
  sv.push_back("shortMax");
  sv.push_back("shortMin");
  sv.push_back("ushortMax");
  sv.push_back("ushortMin");
  sv.push_back("intMax");
  sv.push_back("intMin");
  sv.push_back("uintMin");
  sv.push_back("uintMax");
  sv.push_back("longMax");
  sv.push_back("longMin");
  sv.push_back("ulongMin");
  sv.push_back("ulongMax");
  sv.push_back("real");
  sv.push_back("nreal");
  sv.push_back("npreal");
  test("real", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, nreal) {
  StringVector sv;
  sv.push_back("nreal");
  sv.push_back("npreal");
  sv.push_back("ushortMin");
  sv.push_back("uintMin");
  sv.push_back("ulongMin");
  test("nreal", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, npreal) {
  StringVector sv;
  sv.push_back("npreal");
  sv.push_back("ushortMin");
  sv.push_back("uintMin");
  sv.push_back("ulongMin");
  test("npreal", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, int2) {
  StringVector sv;
  sv.push_back("int2");
  test("int2", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, int3) {
  StringVector sv;
  sv.push_back("int3");
  test("int3", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, int4) {
  StringVector sv;
  sv.push_back("int4");
  test("int4", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, int5) {
  StringVector sv;
  sv.push_back("int5");
  test("int5", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, real2) {
  StringVector sv;
  sv.push_back("int2");
  sv.push_back("real2");
  sv.push_back("nreal2");
  test("real2", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, real3) {
  StringVector sv;
  sv.push_back("int3");
  sv.push_back("real3");
  sv.push_back("nreal3");
  test("real3", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, real4) {
  StringVector sv;
  sv.push_back("int4");
  sv.push_back("real4");
  sv.push_back("nreal4");
  sv.push_back("matrix2");
  sv.push_back("nmatrix2");
  test("real4", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, real5) {
  StringVector sv;
  sv.push_back("int5");
  sv.push_back("real5");
  sv.push_back("nreal5");
  test("real5", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, nreal2) {
  StringVector sv;
  sv.push_back("nreal2");
  test("nreal2", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, nreal3) {
  StringVector sv;
  sv.push_back("nreal3");
  test("nreal3", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, nreal4) {
  StringVector sv;
  sv.push_back("nreal4");
  sv.push_back("nmatrix2");
  test("nreal4", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, nreal5) {
  StringVector sv;
  sv.push_back("nreal5");
  test("nreal5", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, matrix2) {
  StringVector sv;
  sv.push_back("int4");
  sv.push_back("real4");
  sv.push_back("nreal4");
  sv.push_back("matrix2");
  sv.push_back("nmatrix2");
  test("matrix2", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, matrix3) {
  StringVector sv;
  sv.push_back("matrix3");
  sv.push_back("nmatrix3");
  test("matrix3", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, matrix4) {
  StringVector sv;
  sv.push_back("matrix4");
  sv.push_back("nmatrix4");
  test("matrix4", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, nmatrix2) {
  StringVector sv;
  sv.push_back("nreal4");
  sv.push_back("nmatrix2");
  test("nmatrix2", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, nmatrix3) {
  StringVector sv;
  sv.push_back("nmatrix3");
  test("nmatrix3", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, nmatrix4) {
  StringVector sv;
  sv.push_back("nmatrix4");
  test("nmatrix4", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, blank) {
  StringVector sv;
  sv.push_back("blank");
  test("blank", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, cmd) {
  StringVector sv;
  sv.push_back("cmdCd");
  sv.push_back("cmdLp");
  sv.push_back("cmdPwd");
  sv.push_back("cmdLs");
  sv.push_back("cmdSet");
  test("cmd", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, ParallelIntBool) {
  StringVector sv;
  sv.push_back("shortMax");
  sv.push_back("shortMin");
  sv.push_back("ushortMax");
  sv.push_back("ushortMin");
  sv.push_back("intMax");
  sv.push_back("intMin");
  sv.push_back("uintMin");
  sv.push_back("ulongMin");
  if (mIntMax == mLongMax)  // might depend on machine ?
  {
    sv.push_back("longMin");
    sv.push_back("longMax");
  }
  sv.push_back("boolTrue");
  sv.push_back("boolFalse");
  test(mp2IntBool, sv.begin(), sv.end(), 1);
}
TEST_F(TestArgHandler, ParallelIntInt2) {
  StringVector sv;
  sv.push_back("shortMax");
  sv.push_back("shortMin");
  sv.push_back("ushortMax");
  sv.push_back("ushortMin");
  sv.push_back("intMax");
  sv.push_back("intMin");
  sv.push_back("uintMin");
  sv.push_back("ulongMin");
  if (mIntMax == mLongMax)  // might depend on machine ?
  {
    sv.push_back("longMin");
    sv.push_back("longMax");
  }
  sv.push_back("int2");
  test(mp2IntInt2, sv.begin(), sv.end(), 1);
}
TEST_F(TestArgHandler, ParallelBoolReal3Matrix2) {
  StringVector sv;
  sv.push_back("boolTrue");
  sv.push_back("boolFalse");
  sv.push_back("int3");
  sv.push_back("real3");
  sv.push_back("nreal3");
  sv.push_back("int4");
  sv.push_back("real4");
  sv.push_back("nreal4");
  sv.push_back("matrix2");
  sv.push_back("nmatrix2");
  test(mp3BoolReal3Matrix2, sv.begin(), sv.end(), 1);
}
TEST_F(TestArgHandler, ParallelInt3Nreal3Matrix3) {
  StringVector sv;
  sv.push_back("int3");
  sv.push_back("nreal3");
  sv.push_back("matrix3");
  sv.push_back("nmatrix3");
  test(mp3Int3Nreal3Matrix3, sv.begin(), sv.end(), 1);
}
TEST_F(TestArgHandler, ParallelBoolInt2Nreal3Matrix4) {
  StringVector sv;
  sv.push_back("boolTrue");
  sv.push_back("boolFalse");
  sv.push_back("int2");
  sv.push_back("nreal3");
  sv.push_back("matrix4");
  sv.push_back("nmatrix4");
  test(mp4BoolInt2Nreal3Matrix4, sv.begin(), sv.end(), 1);
}
TEST_F(TestArgHandler, SequenceBoolBoolBool) {
  StringVector sv;
  test(ms3BoolBoolBool, sv.begin(), sv.end(), 1);
  ASSERT_TRUE(ms3BoolBoolBool->validate("true true true"));
  ;
}
TEST_F(TestArgHandler, SequenceReal3Real3Real3) {
  StringVector sv;
  sv.push_back("matrix3");
  sv.push_back("nmatrix3");
  test(ms2Real3Real3Real3, sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandler, LoopReal3) {
  StringVector sv;
  sv.push_back("int3");
  sv.push_back("real3");
  sv.push_back("nreal3");
  sv.push_back("matrix3");
  sv.push_back("nmatrix3");
  test(ml1real3, sv.begin(), sv.end(), 1);
  if (ml1real3->validate(mReal3))
    ASSERT_EQ(mReal3, ml1real3->getMatchedNodeValue("real3LoopNode"));
  if (ml1real3->validate(mMatrix3)) {
    Node* node = ml1real3->getNode("real3LoopNode");
    const std::string&& s =
        StringUtil::join(node->beginLoopValueIter(), node->endLoopValueIter());
    ASSERT_EQ(mMatrix3, s);
  }
}

TEST_F(TestArgHandler, LoopReal3Bool) {
  // StringVector sv;
  // test(ml2real3bool, sv.begin(), sv.end(), 1);
  ASSERT_TRUE(ml2real3bool->validate(mReal3 + " " + mBoolTrue));
  ASSERT_EQ(mReal3, ml2real3bool->getMatchedNodeValue("real3LoopNode"));
  ASSERT_EQ(mBoolTrue, ml2real3bool->getMatchedNodeValue("boolLoopNode"));

  ASSERT_TRUE(
      ml2real3bool->validate(mMatrix3 + " " + mBoolFalse + " " + mBoolFalse));
  Node* node = ml2real3bool->getNode("real3LoopNode");
  const std::string&& s =
      StringUtil::join(node->beginLoopValueIter(), node->endLoopValueIter());
  EXPECT_EQ(mMatrix3, s);
  node = ml2real3bool->getNode("boolLoopNode");
  const std::string&& s1 =
      StringUtil::join(node->beginLoopValueIter(), node->endLoopValueIter());
  EXPECT_EQ(mBoolFalse + " " + mBoolFalse, s1);
}

TEST(TestLoop, loopInt) {
  TreeArgHandler handler("loopInt");
  handler.getRoot()
      ->addChildNode("intNode", "int", Node::NT_LOOP)
      ->endBranch("0");

  for (int i = 0; i < 100; ++i) {
    size_t n = rand() % 20 + 1;
    std::stringstream ss;
    while (n-- > 0) ss << rand() % 10000 << " ";

    ASSERT_TRUE(handler.validate(ss.str()));
  }
}

TEST(TestLoop, loopBool) {
  TreeArgHandler handler("loopBool");
  handler.getRoot()
      ->addChildNode("boolNode", "bool", Node::NT_LOOP)
      ->endBranch("0");

  for (int i = 0; i < 100; ++i) {
    size_t n = rand() % 20 + 1;
    std::stringstream ss;
    while (n-- > 0) ss << (rand() % 2 == 0 ? "true" : "false") << " ";
    ASSERT_TRUE(handler.validate(ss.str()));
  }
}

class TestLivingThing : public ::testing::Test {
protected:
  virtual void SetUp() {
    mBirds.push_back("bird0");
    mBirds.push_back("bird1");
    mBirds.push_back("bird2");
    mFishes.push_back("fish0");
    mFishes.push_back("fish1");
    mFishes.push_back("fish2");
    mDogs.push_back("dog0");
    mDogs.push_back("dog1");
    mDogs.push_back("dog2");
    mCats.push_back("cat0");
    mCats.push_back("cat1");
    mCats.push_back("cat2");
    mVegetations.push_back("vegetation0");
    mVegetations.push_back("vegetation1");
    mVegetations.push_back("vegetation2");

    mMap["bird"] = &mBirds;
    mMap["fish"] = &mFishes;
    mMap["dog"] = &mDogs;
    mMap["cat"] = &mCats;
    mMap["vegetation"] = &mVegetations;

    Node* root;
    StringArgHandler* handler;
    TreeArgHandler* tree;
    if (!sgArgLib.exists("bird")) {
      handler = new StringArgHandler("bird");
      handler->insert(mBirds.begin(), mBirds.end());
      sgArgLib.registerArgHandler(handler);
    }
    if (!sgArgLib.exists("fish")) {
      handler = new StringArgHandler("fish");
      handler->insert(mFishes.begin(), mFishes.end());
      sgArgLib.registerArgHandler(handler);
    }
    if (!sgArgLib.exists("dog")) {
      handler = new StringArgHandler("dog");
      handler->insert(mDogs.begin(), mDogs.end());
      sgArgLib.registerArgHandler(handler);
    }
    if (!sgArgLib.exists("cat")) {
      handler = new StringArgHandler("cat");
      handler->insert(mCats.begin(), mCats.end());
      sgArgLib.registerArgHandler(handler);
    }
    if (!sgArgLib.exists("vegetation")) {
      handler = new StringArgHandler("vegetation");
      handler->insert(mVegetations.begin(), mVegetations.end());
      sgArgLib.registerArgHandler(handler);
    }
    if (!sgArgLib.exists("mammal")) {
      tree = new TreeArgHandler("mammal");
      root = tree->getRoot();
      root->addChildNode("dogNode", "dog")->endBranch("dogBranch");
      root->addChildNode("catNode", "cat")->endBranch("catBranch");
      sgArgLib.registerArgHandler(tree);
    }
    if (!sgArgLib.exists("animal")) {
      tree = new TreeArgHandler("animal");
      root = tree->getRoot();
      root->addChildNode("mammalNode", "mammal")->endBranch("mammalBranch");
      root->addChildNode("birdNode", "bird")->endBranch("birdBranch");
      root->addChildNode("fishNode", "fish")->endBranch("fishBranch");
      sgArgLib.registerArgHandler(tree);
    }
    if (!sgArgLib.exists("livingThing")) {
      tree = new TreeArgHandler("livingThing");
      root = tree->getRoot();
      root->addChildNode("animalNode", "animal")->endBranch("animalBranch");
      root->addChildNode("vegetationNode", "vegetation")
          ->endBranch("vegetationBranch");
      sgArgLib.registerArgHandler(tree);
    }
  }
  virtual void TearDown() {}
  template <typename _InputIterator>
  bool ifUseTrue(const std::string& itemName, _InputIterator beg,
      _InputIterator end, bool maskPass) {
    _InputIterator iter = std::find(beg, end, itemName);
    return (maskPass && iter != end) || (!maskPass && iter == end);
  }

  template <typename _InputIterator>
  void test(const std::string& ahName, _InputIterator beg, _InputIterator end,
      bool maskPass = true) {
    ArgHandler* handler = sgArgLib.createArgHandler(ahName);
    test(handler, beg, end, maskPass);
    delete handler;
  }

  /**
   * Only item between beg and end should pass or fail
   * @param beg : begin item
   * @param end : end item
   * @param maskPass : mask pass or fail.
   */
  template <typename _InputIterator>
  void test(ArgHandler* handler, _InputIterator beg, _InputIterator end,
      bool maskPass = true) {
    // it's expanded on purpose, it's clear to find out which item is
    // wrong this way.
    std::string itemName;

    itemName = "bird";
    std::for_each(mMap[itemName]->begin(), mMap[itemName]->end(),
        [&](const std::string& v) -> void {
          if (ifUseTrue(itemName, beg, end, maskPass))
            EXPECT_TRUE(handler->validate(v));
          else
            EXPECT_FALSE(handler->validate(v));
        });

    itemName = "fish";
    std::for_each(mMap[itemName]->begin(), mMap[itemName]->end(),
        [&](const std::string& v) -> void {
          if (ifUseTrue(itemName, beg, end, maskPass))
            EXPECT_TRUE(handler->validate(v));
          else
            EXPECT_FALSE(handler->validate(v));
        });

    itemName = "dog";
    std::for_each(mMap[itemName]->begin(), mMap[itemName]->end(),
        [&](const std::string& v) -> void {
          if (ifUseTrue(itemName, beg, end, maskPass))
            EXPECT_TRUE(handler->validate(v));
          else
            EXPECT_FALSE(handler->validate(v));
        });

    itemName = "cat";
    std::for_each(mMap[itemName]->begin(), mMap[itemName]->end(),
        [&](const std::string& v) -> void {
          if (ifUseTrue(itemName, beg, end, maskPass))
            EXPECT_TRUE(handler->validate(v));
          else
            EXPECT_FALSE(handler->validate(v));
        });

    itemName = "vegetation";
    std::for_each(mMap[itemName]->begin(), mMap[itemName]->end(),
        [&](const std::string& v) -> void {
          if (ifUseTrue(itemName, beg, end, maskPass))
            EXPECT_TRUE(handler->validate(v));
          else
            EXPECT_FALSE(handler->validate(v));
        });
  }

  std::string mBird0, mBird1, mBird2;
  std::string mFish0, mFish1, mFish2;
  std::string mDog0, mDog1, mDog2;
  std::string mCat0, mCat1, mCat2;
  std::string mVegetation0, mVegetation1, mVegetation2;

  StringVector mBirds, mFishes, mDogs, mCats, mVegetations;

  // StringArgHandler* mBird;
  // StringArgHandler* mFish;
  // StringArgHandler* mDog;
  // StringArgHandler* mCat;
  // StringArgHandler* mVegetation;
  // TreeArgHandler* mMammal;
  // TreeArgHandler* mAnimal;
  // TreeArgHandler* mLivingThing;

  std::map<std::string, StringVector*> mMap;
};

TEST_F(TestLivingThing, bird) {
  StringVector sv;
  sv.push_back("bird");
  test("bird", sv.begin(), sv.end(), 1);
}
TEST_F(TestLivingThing, fish) {
  StringVector sv;
  sv.push_back("fish");
  test("fish", sv.begin(), sv.end(), 1);
}
TEST_F(TestLivingThing, dog) {
  StringVector sv;
  sv.push_back("dog");
  test("dog", sv.begin(), sv.end(), 1);
}
TEST_F(TestLivingThing, cat) {
  StringVector sv;
  sv.push_back("cat");
  test("cat", sv.begin(), sv.end(), 1);
}
TEST_F(TestLivingThing, vegetation) {
  StringVector sv;
  sv.push_back("vegetation");
  test("vegetation", sv.begin(), sv.end(), 1);
}
TEST_F(TestLivingThing, mammal) {
  StringVector sv;
  sv.push_back("dog");
  sv.push_back("cat");
  test("mammal", sv.begin(), sv.end(), 1);
}
TEST_F(TestLivingThing, animal) {
  StringVector sv;
  sv.push_back("bird");
  sv.push_back("fish");
  sv.push_back("dog");
  sv.push_back("cat");
  test("animal", sv.begin(), sv.end(), 1);
}
TEST_F(TestLivingThing, livingThing) {
  StringVector sv;
  sv.push_back("bird");
  sv.push_back("fish");
  sv.push_back("dog");
  sv.push_back("cat");
  sv.push_back("vegetation");
  test("livingThing", sv.begin(), sv.end(), 1);

  TreeArgHandler* handler =
      static_cast<TreeArgHandler*>(sgArgLib.createArgHandler("livingThing"));

  ASSERT_TRUE(handler->validate("cat0"));
  ASSERT_STREQ("animalBranch", handler->getMatchedBranch().c_str());
  Node* animalNode = handler->getMatchedNode("animalNode");
  ASSERT_STREQ("cat0", animalNode->getValue().c_str());
  TreeArgHandler* animalHandler =
      static_cast<TreeArgHandler*>(animalNode->getArgHandler());
  ASSERT_STREQ("mammalBranch", animalHandler->getMatchedBranch().c_str());
  TreeArgHandler* mammalHandler = animalHandler->getSubTree("mammalNode");
  ASSERT_STREQ("catBranch", mammalHandler->getMatchedBranch().c_str());
  ASSERT_STREQ("cat0", mammalHandler->getValue().c_str());
  Node* catNode = mammalHandler->getMatchedNode("catNode");
  ASSERT_STREQ("cat0", catNode->getValue().c_str());
}

#endif /* TESTPACARGHANDLER_H */
