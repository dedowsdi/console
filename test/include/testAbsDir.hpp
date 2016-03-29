#ifndef TESTABSDIR_H
#define TESTABSDIR_H
#include "pacStringInterface.h"
#include "pacAbsDir.h"
#include "pacStringUtil.h"
#include "pacArgHandler.h"
#include "pacIntrinsicArgHandler.h"
#include "pacException.h"
#include "gtest/gtest.h"

namespace pac {

class TestSI : public StringInterface {
public:
  TestSI() : StringInterface("test", true) {
    if (createParamDict()) {
      ParamDictionary* dict = getParamDict();
      dict->addParameter("paramBool", &msParamBool);
      dict->addParameter("paramInt", &msParamInt);
      dict->addParameter("paramString", &msParamString);

      StringArgHandler* handler = new StringArgHandler("testString");
      handler->insert("one");
      handler->insert("two");
      handler->insert("three");
      sgArgLib.registerArgHandler(handler);
    }
  }

  bool getBool() const { return mBool; }
  void setBool(bool b) { mBool = b; }
  const std::string& getString() const { return mString; }
  void setString(const std::string& v) { mString = v; }
  int getInt() const { return mInt; }
  void setInt(int v) { mInt = v; }

  struct ParamBool : public ParamCmd {
    ParamBool() : ParamCmd("bool") {}
    virtual std::string doGet(const void* target) const {
      const TestSI* si = static_cast<const TestSI*>(target);
      return StringUtil::toString(si->getBool());
    }
    virtual void doSet(void* target, ArgHandler* handler) {
      TestSI* si = static_cast<TestSI*>(target);
      si->setBool(StringUtil::parseBool(handler->getValue()));
    }
  };

  struct ParamString : public ParamCmd {
    ParamString() : ParamCmd("testString") {}
    virtual std::string doGet(const void* target) const {
      const TestSI* si = static_cast<const TestSI*>(target);
      return si->getString();
    }
    virtual void doSet(void* target, ArgHandler* handler) {
      TestSI* si = static_cast<TestSI*>(target);
      si->setString(handler->getValue());
    }
  };

  struct ParamInt : public ParamCmd {
    ParamInt() : ParamCmd("int") {}
    virtual std::string doGet(const void* target) const {
      const TestSI* si = static_cast<const TestSI*>(target);
      return StringUtil::toString(si->getInt());
    }
    virtual void doSet(void* target, ArgHandler* handler) {
      TestSI* si = static_cast<TestSI*>(target);
      si->setInt(StringUtil::parseInt(handler->getValue()));
    }
  };

  static ParamBool msParamBool;
  static ParamString msParamString;
  static ParamInt msParamInt;
  bool mBool;
  std::string mString;
  int mInt;
};

TestSI::ParamBool TestSI::msParamBool;
TestSI::ParamString TestSI::msParamString;
TestSI::ParamInt TestSI::msParamInt;

class TestAbsDir : public ::testing::Test {
protected:
  void SetUp() {
    d = pac::delim;
    dir0 = new AbsDir("dir0", new TestSI());
    dir0_0 = new AbsDir("dir0_0", new TestSI());
    dir0_1 = new AbsDir("dir0_1", new TestSI());
    dir0_0_0 = new AbsDir("dir0_0_0", new TestSI());
    dir0_0_1 = new AbsDir("dir0_0_1", new TestSI());
    dir0_1_0 = new AbsDir("dir0_1_0", new TestSI());
    dir0_1_1 = new AbsDir("dir0_1_1", new TestSI());
    sgRootDir.addChild(dir0);
    dir0->addChild(dir0_0);
    dir0->addChild(dir0_1);
    dir0_0->addChild(dir0_0_0);
    dir0_0->addChild(dir0_0_1);
    dir0_1->addChild(dir0_1_0);
    dir0_1->addChild(dir0_1_1);
  }
  void TearDown() {
    delete dir0;
    // delete dir0_0;
    // delete dir0_1;
    // delete dir0_0_0;
    // delete dir0_0_1;
    // delete dir0_1_0;
    // delete dir0_1_1;
  }

  AbsDir* dir0, *dir0_0, *dir0_1, *dir0_0_0, *dir0_0_1, *dir0_1_0, *dir0_1_1;
  std::string d;  // delim
};

TEST_F(TestAbsDir, addChildWithIlligalName) {
  AbsDir ldir0("ab c", 0);
  EXPECT_THROW(sgRootDir.addChild(&ldir0), InvalidParametersException);
  AbsDir ldir1("abc" + pac::delim, 0);
  EXPECT_THROW(sgRootDir.addChild(&ldir1), InvalidParametersException);
}

TEST_F(TestAbsDir, addDuplicateChild) {
  EXPECT_THROW(sgRootDir.addChild(dir0), ItemIdentityException);
}

TEST_F(TestAbsDir, getParameters) {
  StringVector&& sv = dir0->getParameters();
  EXPECT_EQ(3, sv.size());
  EXPECT_STREQ("paramBool", sv[0].c_str());
  EXPECT_STREQ("paramInt", sv[1].c_str());
  EXPECT_STREQ("paramString", sv[2].c_str());
}

TEST_F(TestAbsDir, getsetParameter) {
  dir0->setParameter("paramInt", "1");
  EXPECT_EQ("1", dir0->getParameter("paramInt"));
  dir0->setParameter("paramBool", "true");
  EXPECT_EQ("true", dir0->getParameter("paramBool"));
  dir0->setParameter("paramString", "one");
  EXPECT_EQ("one", dir0->getParameter("paramString"));

  EXPECT_THROW(
      dir0->setParameter("paramInt", "1.0"), InvalidParametersException);
  EXPECT_THROW(
      dir0->setParameter("paramInt", "true"), InvalidParametersException);
  EXPECT_THROW(
      dir0->setParameter("paramInt", "false"), InvalidParametersException);
  EXPECT_THROW(
      dir0->setParameter("paramBool", "1"), InvalidParametersException);
  EXPECT_THROW(
      dir0->setParameter("paramString", "1"), InvalidParametersException);
  EXPECT_THROW(
      dir0->setParameter("paramString", "true"), InvalidParametersException);
}

TEST_F(TestAbsDir, getFullPath) {
  EXPECT_EQ(d, sgRootDir.getFullPath());
  EXPECT_EQ(d + "dir0" + d, dir0->getFullPath());
  EXPECT_EQ(d + "dir0" + d + "dir0_0" + d, dir0_0->getFullPath());
  EXPECT_EQ(d + "dir0" + d + "dir0_1" + d, dir0_1->getFullPath());
  EXPECT_EQ(
      d + "dir0" + d + "dir0_0" + d + "dir0_0_0" + d, dir0_0_0->getFullPath());
  EXPECT_EQ(
      d + "dir0" + d + "dir0_0" + d + "dir0_0_1" + d, dir0_0_1->getFullPath());
  EXPECT_EQ(
      d + "dir0" + d + "dir0_1" + d + "dir0_1_0" + d, dir0_1_0->getFullPath());
  EXPECT_EQ(
      d + "dir0" + d + "dir0_1" + d + "dir0_1_1" + d, dir0_1_1->getFullPath());
}

TEST_F(TestAbsDir, findPathThrow) {
  EXPECT_THROW(AbsDirUtil::findPath(pac::delim + " ", &sgRootDir),
      InvalidParametersException);
  EXPECT_THROW(AbsDirUtil::findPath("abc", 0), InvalidParametersException);
}

TEST_F(TestAbsDir, findRelativePath) {
  EXPECT_EQ(&sgRootDir, AbsDirUtil::findPath(".", &sgRootDir));
  EXPECT_EQ(0, AbsDirUtil::findPath("..", &sgRootDir));
  EXPECT_EQ(&sgRootDir,
      AbsDirUtil::findPath("." + d + "dir0" + d + ".." + d, &sgRootDir));
  EXPECT_EQ(dir0, AbsDirUtil::findPath("dir0", &sgRootDir));
  EXPECT_EQ(dir0_0, AbsDirUtil::findPath("dir0" + d + "dir0_0", &sgRootDir));
  EXPECT_EQ(dir0_1, AbsDirUtil::findPath("dir0" + d + "dir0_1", &sgRootDir));
  EXPECT_EQ(dir0_0_0,
      AbsDirUtil::findPath("dir0" + d + "dir0_0" + d + "dir0_0_0", &sgRootDir));
  EXPECT_EQ(dir0_0_1,
      AbsDirUtil::findPath("dir0" + d + "dir0_0" + d + "dir0_0_1", &sgRootDir));
  EXPECT_EQ(dir0_1_0,
      AbsDirUtil::findPath("dir0" + d + "dir0_1" + d + "dir0_1_0", &sgRootDir));
  EXPECT_EQ(dir0_1_1,
      AbsDirUtil::findPath("dir0" + d + "dir0_1" + d + "dir0_1_1", &sgRootDir));
  EXPECT_EQ(dir0_0_0, AbsDirUtil::findPath("dir0_0" + d + "dir0_0_0", dir0));
  EXPECT_EQ(dir0_0_1, AbsDirUtil::findPath("dir0_0" + d + "dir0_0_1", dir0));
  EXPECT_EQ(dir0_1_0, AbsDirUtil::findPath("dir0_1" + d + "dir0_1_0", dir0));
  EXPECT_EQ(dir0_1_1, AbsDirUtil::findPath("dir0_1" + d + "dir0_1_1", dir0));
  EXPECT_EQ(dir0_0_0, AbsDirUtil::findPath("dir0_0_0", dir0_0));
  EXPECT_EQ(dir0_0_1, AbsDirUtil::findPath("dir0_0_1", dir0_0));
  EXPECT_EQ(dir0_1_0, AbsDirUtil::findPath("dir0_1_0", dir0_1));
  EXPECT_EQ(dir0_1_1, AbsDirUtil::findPath("dir0_1_1", dir0_1));
  EXPECT_EQ(0, AbsDirUtil::findPath("abcd", dir0_1));
}

TEST_F(TestAbsDir, findAbsolutePath) {
  EXPECT_EQ(dir0, AbsDirUtil::findPath(d + "dir0", &sgRootDir));
  EXPECT_EQ(
      dir0_0, AbsDirUtil::findPath(d + "dir0" + d + "dir0_0", &sgRootDir));
  EXPECT_EQ(
      dir0_1, AbsDirUtil::findPath(d + "dir0" + d + "dir0_1", &sgRootDir));
  EXPECT_EQ(
      dir0_0_0, AbsDirUtil::findPath(
                    d + "dir0" + d + "dir0_0" + d + "dir0_0_0", &sgRootDir));
  EXPECT_EQ(
      dir0_0_1, AbsDirUtil::findPath(
                    d + "dir0" + d + "dir0_0" + d + "dir0_0_1", &sgRootDir));
  EXPECT_EQ(
      dir0_1_0, AbsDirUtil::findPath(
                    d + "dir0" + d + "dir0_1" + d + "dir0_1_0", &sgRootDir));
  EXPECT_EQ(
      dir0_1_1, AbsDirUtil::findPath(
                    d + "dir0" + d + "dir0_1" + d + "dir0_1_1", &sgRootDir));
}
}

#endif /* TESTABSDIR_H */
