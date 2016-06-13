#ifndef TESTDIRSYSTEM_HPP
#define TESTDIRSYSTEM_HPP

#include "pacConsole.h"
#include "pacAbsDir.h"
#include "testAbsDir.h"
#include "pacConsoleUI.h"
#include "testConsoleUI.h"
#include "pacIntrinsicArgHandler.h"
#include "testConsoleUI.h"
#include <gtest/gtest.h>

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
class TestConsoleSystem : public ::testing::Test {
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
    pathDir0 = d + "dir0" + d;
    pathDir0_0 = pathDir0 + "dir0_0" + d;
    pathDir0_1 = pathDir0 + "dir0_1" + d;
    pathDir0_0_0 = pathDir0_0 + "dir0_0_0" + d;
    pathDir0_0_1 = pathDir0_0 + "dir0_0_1" + d;
    pathDir0_1_0 = pathDir0_1 + "dir0_1_0" + d;
    pathDir0_1_1 = pathDir0_1 + "dir0_1_1" + d;
    pathConsole = d + "consoleUi" + d;
    mUi = getImplUi();
    mUi->setLastOutput("");
    mUi->setCmdLine("");
  }

  void TearDown() {
    if (dir0) delete dir0;
    sgConsole.setCwd(&sgRootDir);
  }

  ImplConsoleUI* getImplUi() {
    return static_cast<ImplConsoleUI*>(sgConsole.getUi());
  }

  const std::string& getLastOutput() { return getImplUi()->getLastOutput(); }
  std::string getCwd() { return getImplUi()->getCwd(); }
  std::string getCmdLine() { return getImplUi()->getCmdLine(); }

  AbsDir* dir0, *dir0_0, *dir0_1, *dir0_0_0, *dir0_0_1, *dir0_1_0, *dir0_1_1;
  std::string d;
  std::string pathDir0, pathDir0_0, pathDir0_1, pathDir0_0_0, pathDir0_0_1,
      pathDir0_1_0, pathDir0_1_1, pathConsole;
  ImplConsoleUI* mUi;
};
}

#endif /* TESTDIRSYSTEM_HPP */
