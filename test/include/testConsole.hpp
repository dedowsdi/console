#ifndef TESTCONSOLE_H
#define TESTCONSOLE_H
#include "pacConsole.h"
#include "testAbsDir.hpp"
#include "pacUiConsole.h"
#include "pacIntrinsicArgHandler.h"
#include "testUiConsole.hpp"

namespace pac {
class TestConsole : public ::testing::Test {
protected:
  void SetUp() {
    d = pac::delim;
    testSI = new TestSI();
    dir0 = new AbsDir("dir0", testSI);
    dir0_0 = new AbsDir("dir0_0", testSI);
    dir0_0_0 = new AbsDir("dir0_0_0", testSI);
    sgRootDir.addChild(dir0);
    dir0->addChild(dir0_0);
    dir0_0->addChild(dir0_0_0);
    sgConsole.setCwd(&sgRootDir);
    pathDir0 = d + "dir0" + d;
    pathDir0_0 = pathDir0 + "dir0_0" + d;
    pathDir0_0_0 = pathDir0_0 + "dir0_0_0" + d;
    pathConsole = d + "uiConsole" + d;
    getImplUi()->setLastOutput("");
    getImplUi()->setCmdLine("");
  }

  void TearDown() {
    delete dir0;
    delete testSI;
  }

  ImplUiConsole* getImplUi() {
    return static_cast<ImplUiConsole*>(sgConsole.getUi());
  }

  const std::string& getLastOutput() { return getImplUi()->getLastOutput(); }
  std::string getCwd() { return getImplUi()->getCwd(); }
  std::string getCmdLine() { return getImplUi()->getCmdLine(); }

  AbsDir* dir0, *dir0_0, *dir0_0_0;
  TestSI* testSI;
  std::string d;
  std::string pathDir0, pathDir0_0, pathDir0_0_0, pathConsole;
};

TEST_F(TestConsole, pathArgHandler) {
  PathArgHandler* handler =
      static_cast<PathArgHandler*>(sgArgLib.createArgHandler("path"));
  EXPECT_TRUE(handler->validate(d));
  EXPECT_EQ(&sgRootDir, handler->getPathDir());
  EXPECT_TRUE(handler->validate(pathDir0));
  EXPECT_EQ(dir0, handler->getPathDir());
  EXPECT_TRUE(handler->validate(pathDir0_0));
  EXPECT_EQ(dir0_0, handler->getPathDir());
  EXPECT_TRUE(handler->validate(pathDir0_0_0));
  EXPECT_EQ(dir0_0_0, handler->getPathDir());
  EXPECT_TRUE(handler->validate(pathConsole));
  EXPECT_EQ(pathConsole, handler->getPathDir()->getFullPath());
  delete handler;
}
TEST_F(TestConsole, paramArgHandler) {
  sgConsole.setCwd(dir0);
  ParamArgHandler* handler =
      static_cast<ParamArgHandler*>(sgArgLib.createArgHandler("param"));
  EXPECT_TRUE(handler->validate("paramBool"));
  EXPECT_TRUE(handler->validate("paramInt"));
  EXPECT_TRUE(handler->validate("paramString"));
  delete handler;
}
TEST_F(TestConsole, pparamArgHandler) {
  sgConsole.setCwd(dir0);
  PparamArgHandler* handler =
      static_cast<PparamArgHandler*>(sgArgLib.createArgHandler("pparam"));
  EXPECT_TRUE(handler->validate("paramBool"));
  EXPECT_TRUE(handler->validate("paramInt"));
  EXPECT_TRUE(handler->validate("paramString"));
  EXPECT_FALSE(handler->validate(d));
  EXPECT_FALSE(handler->validate(pathDir0_0));
  EXPECT_FALSE(handler->validate(pathDir0_0_0));
  EXPECT_FALSE(handler->validate(pathDir0 + " bool"));
  EXPECT_TRUE(handler->validate(pathDir0 + " paramBool"));
  EXPECT_TRUE(handler->validate(pathDir0 + " paramInt"));
  EXPECT_TRUE(handler->validate(pathDir0 + " paramString"));
  delete handler;
}

// TEST_F(TestConsole, valueArgHandler) {
// sgConsole.setCwd(dir0);

// TreeArgHandler* handler =
// static_cast<TreeArgHandler*>(sgArgLib.createArgHandler("ahcmd_set"));
// EXPECT_TRUE(handler->validate(pathDir0 + " paramBool"));

//}

TEST_F(TestConsole, promptCmd) {
  sgConsole.getUi()->setCmdLine("");
  sgConsole.prompt();
  EXPECT_EQ("", getCmdLine());
  sgConsole.getUi()->setCmdLine("p");
  sgConsole.prompt();
  EXPECT_EQ("pwd", getCmdLine());
  sgConsole.getUi()->setCmdLine("pwd");
  sgConsole.prompt();
  EXPECT_EQ("pwd", getCmdLine());
  sgConsole.getUi()->setCmdLine(" c");
  sgConsole.prompt();
  EXPECT_EQ(" cd", getCmdLine());
  sgConsole.getUi()->setCmdLine("  ");
  sgConsole.prompt();
  EXPECT_EQ("  ", getCmdLine());
  sgConsole.getUi()->setCmdLine("s");
  sgConsole.prompt();
  EXPECT_EQ("set", getCmdLine());
  sgConsole.getUi()->setCmdLine("se");
  sgConsole.prompt();
  EXPECT_EQ("set", getCmdLine());
}

TEST_F(TestConsole, executeCmdCd) {
  sgConsole.execute("cd " + d + "dir0");
  ASSERT_EQ(dir0, sgConsole.getCwd());
  sgConsole.execute("  cd " + d + "dir0" + d + "dir0_0" + d);
  ASSERT_EQ(dir0_0, sgConsole.getCwd());
  sgConsole.execute("cd " + d + "dir0" + d + "dir0_0" + d + "dir0_0_0");
  ASSERT_EQ(dir0_0_0, sgConsole.getCwd());
  sgConsole.execute("cd " + d);
  ASSERT_EQ(&sgRootDir, sgConsole.getCwd());
  sgConsole.execute("  cd dir0");
  ASSERT_EQ(dir0, sgConsole.getCwd());
  sgConsole.execute("cd dir0_0" + d);
  ASSERT_EQ(dir0_0, sgConsole.getCwd());
  sgConsole.execute(" cd dir0_0_0  ");
  ASSERT_EQ(dir0_0_0, sgConsole.getCwd());
  sgConsole.execute("cd .");
  ASSERT_EQ(dir0_0_0, sgConsole.getCwd());
  sgConsole.execute("cd ..");
  ASSERT_EQ(dir0_0, sgConsole.getCwd());
  sgConsole.execute("cd .." + d + "..");
  ASSERT_EQ(&sgRootDir, sgConsole.getCwd());
}

TEST_F(TestConsole, promptCmdCd) {
  sgConsole.getUi()->setCmdLine("cd " + d + "ui");
  sgConsole.prompt();
  EXPECT_EQ("cd " + d + "uiConsole", getCmdLine());
  sgConsole.getUi()->setCmdLine("cd " + d + "di");
  sgConsole.prompt();
  EXPECT_EQ("cd " + d + "dir0", getCmdLine());
  sgConsole.getUi()->setCmdLine("cd " + d + "dir0");
  sgConsole.prompt();
  EXPECT_EQ("cd " + d + "dir0", getCmdLine());
  sgConsole.getUi()->setCmdLine("cd " + d + "dir0" + d);
  sgConsole.prompt();
  EXPECT_EQ("cd " + d + "dir0" + d + "dir0_0", getCmdLine());
  sgConsole.getUi()->setCmdLine("cd " + d + "dir0" + d + "dir0_0");
  sgConsole.prompt();
  EXPECT_EQ("cd " + d + "dir0" + d + "dir0_0", getCmdLine());
  sgConsole.getUi()->setCmdLine("cd " + d + "dir0" + d + "dir0_0" + d);
  sgConsole.prompt();
  EXPECT_EQ("cd " + d + "dir0" + d + "dir0_0" + d + "dir0_0_0", getCmdLine());
}

TEST_F(TestConsole, executeCmdPwd) {
  sgConsole.execute("cd " + d + "dir0");
  sgConsole.execute("pwd");
  ASSERT_EQ(d + "dir0" + d, getLastOutput());
  sgConsole.execute("cd dir0_0" + d);
  sgConsole.execute(" pwd ");
  ASSERT_EQ(d + "dir0" + d + "dir0_0" + d, getLastOutput());
  sgConsole.execute("cd dir0_0_0" + d);
  sgConsole.execute(" pwd ");
  ASSERT_EQ(d + "dir0" + d + "dir0_0" + d + "dir0_0_0" + d, getLastOutput());
}

TEST_F(TestConsole, promptCmdPwd) {
  sgConsole.getUi()->setCmdLine("pwd  ");
  sgConsole.prompt();
  EXPECT_EQ("pwd  ", getCmdLine());
}

TEST_F(TestConsole, executeCmdLs) {
  sgConsole.execute("ls");
  ASSERT_EQ("\ndir0  uiConsole  ", getLastOutput());
  sgConsole.execute("ls dir0");
  ASSERT_EQ("\ndir0_0  ", getLastOutput());
}

TEST_F(TestConsole, promptCmdLs) {
  sgConsole.getUi()->setCmdLine("ls " + d + "ui");
  sgConsole.prompt();
  EXPECT_EQ("ls " + d + "uiConsole", getCmdLine());
  sgConsole.getUi()->setCmdLine("ls " + d + "di");
  sgConsole.prompt();
  EXPECT_EQ("ls " + d + "dir0", getCmdLine());
  sgConsole.getUi()->setCmdLine("ls " + d + "dir0");
  sgConsole.prompt();
  EXPECT_EQ("ls " + d + "dir0", getCmdLine());
  sgConsole.getUi()->setCmdLine("ls " + d + "dir0" + d);
  sgConsole.prompt();
  EXPECT_EQ("ls " + d + "dir0" + d + "dir0_0", getCmdLine());
  sgConsole.getUi()->setCmdLine("ls " + d + "dir0" + d + "dir0_0");
  sgConsole.prompt();
  EXPECT_EQ("ls " + d + "dir0" + d + "dir0_0", getCmdLine());
  sgConsole.getUi()->setCmdLine("ls " + d + "dir0" + d + "dir0_0" + d);
  sgConsole.prompt();
  EXPECT_EQ("ls " + d + "dir0" + d + "dir0_0" + d + "dir0_0_0", getCmdLine());
  sgConsole.getUi()->setCmdLine("ls " + d + "uiConsole " + d + "di");
  sgConsole.prompt();
  EXPECT_EQ("ls " + d + "uiConsole " + d + "dir0", getCmdLine());
}
TEST_F(TestConsole, executeCmdSet) {
  sgConsole.setCwd(dir0);
  EXPECT_TRUE(sgConsole.execute("set paramInt 0"));
  EXPECT_STREQ("0", dir0->getParameter("paramInt").c_str());
  EXPECT_TRUE(sgConsole.execute("set paramInt 1"));
  EXPECT_STREQ("1", dir0->getParameter("paramInt").c_str());
  EXPECT_TRUE(sgConsole.execute("set . paramInt 2"));
  EXPECT_STREQ("2", dir0->getParameter("paramInt").c_str());
  EXPECT_TRUE(sgConsole.execute("set " + pathDir0 + " paramInt 3"));
  EXPECT_STREQ("3", dir0->getParameter("paramInt").c_str());
  EXPECT_TRUE(sgConsole.execute("set " + pathDir0 + " paramString one"));
  EXPECT_STREQ("one", dir0->getParameter("paramString").c_str());
  EXPECT_FALSE(sgConsole.execute("set " + pathDir0 + " paramString x"));
}

TEST_F(TestConsole, promptCmdSet) {
  sgConsole.setCwd(dir0);
  sgConsole.getUi()->setCmdLine("set paramString");
  sgConsole.prompt();
  EXPECT_EQ("set paramString", getCmdLine());
  sgConsole.getUi()->setCmdLine("set paramString ");
  sgConsole.prompt();

  EXPECT_EQ("set paramString ", getCmdLine());
  EXPECT_EQ("\none  three  two  ", getLastOutput());
  sgConsole.getUi()->setCmdLine("set paramString o");
  sgConsole.prompt();
  EXPECT_EQ("set paramString one", getCmdLine());
  sgConsole.getUi()->setCmdLine("set paramString tw");
  sgConsole.prompt();
  EXPECT_EQ("set paramString two", getCmdLine());
  sgConsole.getUi()->setCmdLine("set " + d + "di");
  sgConsole.prompt();
  EXPECT_EQ("set " + d + "dir0", getCmdLine());
  sgConsole.getUi()->setCmdLine("set " + pathDir0_0 + " paramString tw");
  sgConsole.prompt();
  EXPECT_EQ("set " + pathDir0_0 + " paramString two", getCmdLine());
  sgConsole.getUi()->setCmdLine("set " + pathDir0_0_0 + " paramBool f");
  sgConsole.prompt();
  EXPECT_EQ("set " + pathDir0_0_0 + " paramBool false", getCmdLine());
}
}

#endif /* TESTCONSOLE_H */
