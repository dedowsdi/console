#ifndef TESTCONSOLE_H
#define TESTCONSOLE_H
#include "pacConsole.h"
#include "testConsoleSystem.hpp"
#include "pacConsoleUI.h"
#include "pacIntrinsicArgHandler.h"
#include "testConsoleUI.hpp"

namespace pac {

TEST_F(TestConsoleSystem, pathArgHandler) {
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
  EXPECT_FALSE(handler->validate(" "));
  delete handler;
}
TEST_F(TestConsoleSystem, paramArgHandler) {
  sgConsole.setCwd(dir0);
  ParamArgHandler* handler =
      static_cast<ParamArgHandler*>(sgArgLib.createArgHandler("param"));
  EXPECT_TRUE(handler->validate("paramBool"));
  EXPECT_TRUE(handler->validate("paramInt"));
  EXPECT_TRUE(handler->validate("paramString"));
  delete handler;
}
// deprecated
// TEST_F(TestConsoleSystem, pparamArgHandler) {
// sgConsole.setCwd(dir0);
// PparamArgHandler* handler =
// static_cast<PparamArgHandler*>(sgArgLib.createArgHandler("pparam"));
// EXPECT_TRUE(handler->validate("paramBool"));
// EXPECT_TRUE(handler->validate("paramInt"));
// EXPECT_TRUE(handler->validate("paramString"));
// EXPECT_FALSE(handler->validate(d));
// EXPECT_FALSE(handler->validate(pathDir0_0));
// EXPECT_FALSE(handler->validate(pathDir0_0_0));
// EXPECT_FALSE(handler->validate(pathDir0 + " bool"));
// EXPECT_TRUE(handler->validate(pathDir0 + " paramBool"));
// EXPECT_TRUE(handler->validate(pathDir0 + " paramInt"));
// EXPECT_TRUE(handler->validate(pathDir0 + " paramString"));
// delete handler;
//}

// TEST_F(TestConsoleSystem, valueArgHandler) {
// sgConsole.setCwd(dir0);

// TreeArgHandler* handler =
// static_cast<TreeArgHandler*>(sgArgLib.createArgHandler("ahcmd_set"));
// EXPECT_TRUE(handler->validate(pathDir0 + " paramBool"));

//}

TEST_F(TestConsoleSystem, illigalCmdName) {
  sgConsole.getUi()->setCmdLine("adfsf");
  EXPECT_FALSE(sgConsole.execute());
}

TEST_F(TestConsoleSystem, promptCmd) {
  sgConsole.getUi()->setCmdLine("");
  sgConsole.prompt();
  EXPECT_EQ("", getCmdLine());
  sgConsole.getUi()->setCmdLine("p");
  sgConsole.prompt();
  EXPECT_EQ("pwd", getCmdLine());
  sgConsole.getUi()->setCmdLine("pwd");
  sgConsole.prompt();
  EXPECT_EQ("pwd", getCmdLine());
  sgConsole.getUi()->setCmdLine(" ct");
  sgConsole.prompt();
  EXPECT_EQ(" ctd", getCmdLine());
  sgConsole.getUi()->setCmdLine("  ");
  sgConsole.prompt();
  EXPECT_EQ("  ", getCmdLine());
  sgConsole.getUi()->setCmdLine("s");
  sgConsole.prompt();
  EXPECT_EQ("s", getCmdLine());
  sgConsole.getUi()->setCmdLine("se");
  sgConsole.prompt();
  EXPECT_EQ("set", getCmdLine());
}

TEST_F(TestConsoleSystem, executeCmdCd) {
  sgConsole.execute("cd " + d + "dir0");
  ASSERT_EQ(dir0, sgConsole.getCwd());
  sgConsole.execute("  cd " + pathDir0_0);
  ASSERT_EQ(dir0_0, sgConsole.getCwd());
  sgConsole.execute("cd " + pathDir0_0_0);
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
TEST_F(TestConsoleSystem, executeCmdCdAalternate) {
  EXPECT_TRUE(sgConsole.execute("cd " + d + "dir0"));
  EXPECT_TRUE(sgConsole.execute("cd -"));
  ASSERT_EQ(&sgRootDir, sgConsole.getCwd());
  EXPECT_TRUE(sgConsole.execute("cd -"));
  ASSERT_EQ(dir0, sgConsole.getCwd());
  EXPECT_TRUE(sgConsole.execute("cd " + pathDir0_0_0));
  EXPECT_TRUE(sgConsole.execute("cd " + pathDir0_0));
  delete dir0_0_0;
  EXPECT_EQ(0, sgConsole.getAlternateDir());
  delete dir0_0;
  EXPECT_EQ(&sgRootDir, sgConsole.getCwd());
}

TEST_F(TestConsoleSystem, promptCmdCd) {
  sgConsole.getUi()->setCmdLine("cd " + d + "con");
  sgConsole.prompt();
  EXPECT_EQ("cd " + d + "consoleUi", getCmdLine());
  sgConsole.getUi()->setCmdLine("cd " + d + "di");
  sgConsole.prompt();
  EXPECT_EQ("cd " + d + "dir0", getCmdLine());
  sgConsole.getUi()->setCmdLine("cd " + d + "dir0");
  sgConsole.prompt();
  EXPECT_EQ("cd " + d + "dir0", getCmdLine());
  sgConsole.getUi()->setCmdLine("cd " + pathDir0 + "dir0_0");
  sgConsole.prompt();
  EXPECT_EQ("cd " + pathDir0 + "dir0_0", getCmdLine());
  sgConsole.getUi()->setCmdLine("cd " + pathDir0_0);
  sgConsole.prompt();
  EXPECT_EQ("cd " + pathDir0_0 + "dir0_0_", getCmdLine());
}

TEST_F(TestConsoleSystem, executeCmdPwd) {
  sgConsole.execute("cd " + d + "dir0");
  sgConsole.execute("pwd");
  EXPECT_EQ(getSortedVector({pathDir0}), mUi->getItems());
  sgConsole.execute("cd dir0_0" + d);
  sgConsole.execute(" pwd ");
  EXPECT_EQ(getSortedVector({pathDir0_0}), mUi->getItems());
  sgConsole.execute("cd dir0_0_0" + d);
  sgConsole.execute(" pwd ");
  EXPECT_EQ((getSortedVector({pathDir0_0_0})), mUi->getItems());
}

TEST_F(TestConsoleSystem, promptCmdPwd) {
  sgConsole.getUi()->setCmdLine("pwd  ");
  sgConsole.prompt();
  EXPECT_EQ("pwd  ", getCmdLine());
}

TEST_F(TestConsoleSystem, executeCmdLs) {
  sgConsole.execute("ls");
  ASSERT_EQ(getSortedVector({"consoleUi", "dir0"}), mUi->getItems());
  sgConsole.execute("ls dir0");
  ASSERT_EQ(getSortedVector({"dir0_0", "dir0_1"}), mUi->getItems());
}

TEST_F(TestConsoleSystem, promptCmdLs) {
  sgConsole.getUi()->setCmdLine("ls " + d + "con");
  sgConsole.prompt();
  EXPECT_EQ("ls " + d + "consoleUi", getCmdLine());
  sgConsole.getUi()->setCmdLine("ls " + d + "di");
  sgConsole.prompt();
  EXPECT_EQ("ls " + d + "dir0", getCmdLine());
  sgConsole.getUi()->setCmdLine("ls " + d + "dir0");
  sgConsole.prompt();
  EXPECT_EQ("ls " + d + "dir0", getCmdLine());
  sgConsole.getUi()->setCmdLine("ls " + pathDir0);
  sgConsole.prompt();
  EXPECT_EQ("ls " + pathDir0 + "dir0_", getCmdLine());
  sgConsole.getUi()->setCmdLine("ls " + pathDir0+ "dir0_0");
  sgConsole.prompt();
  EXPECT_EQ("ls " + pathDir0 + "dir0_0", getCmdLine());
  sgConsole.getUi()->setCmdLine("ls " + pathDir0_0);
  sgConsole.prompt();
  EXPECT_EQ("ls " + pathDir0_0 + "dir0_0_", getCmdLine());
}
TEST_F(TestConsoleSystem, executeCmdSet) {
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

TEST_F(TestConsoleSystem, promptCmdSet) {
  sgConsole.setCwd(dir0);
  sgConsole.getUi()->setCmdLine("set paramString");
  sgConsole.prompt();
  EXPECT_EQ("set paramString", getCmdLine());
  sgConsole.getUi()->setCmdLine("set paramString ");
  sgConsole.prompt();

  EXPECT_EQ("set paramString ", getCmdLine());
  EXPECT_EQ("one  three  two  \n", getLastOutput());
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

TEST_F(TestConsoleSystem, executeCmdCtd) {
  sgConsole.execute("ctd");
  EXPECT_EQ(1, sgRootDir.getNumChildren());
  dir0 = 0;
}
}

#endif /* TESTCONSOLE_H */
