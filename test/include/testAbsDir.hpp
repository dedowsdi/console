#ifndef TESTABSDIR_H
#define TESTABSDIR_H
#include "testConsoleSystem.hpp"
#include "pacArgHandler.h"
#include "pacIntrinsicArgHandler.h"
#include "pacException.h"
#include "gtest/gtest.h"

namespace pac {

TEST_F(TestConsoleSystem, addChildWithIlligalName) {
  AbsDir ldir0("ab c", 0);
  EXPECT_THROW(sgRootDir.addChild(&ldir0), InvalidParametersException);
  AbsDir ldir1("abc" + pac::delim, 0);
  EXPECT_THROW(sgRootDir.addChild(&ldir1), InvalidParametersException);
}

TEST_F(TestConsoleSystem, addDuplicateChild) {
  EXPECT_THROW(sgRootDir.addChild(dir0), ItemIdentityException);
}

TEST_F(TestConsoleSystem, getParameters) {
  StringVector&& sv = dir0->getParameters();
  EXPECT_EQ(3, sv.size());
  EXPECT_STREQ("paramBool", sv[0].c_str());
  EXPECT_STREQ("paramInt", sv[1].c_str());
  EXPECT_STREQ("paramString", sv[2].c_str());
}

TEST_F(TestConsoleSystem, getsetParameter) {
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

TEST_F(TestConsoleSystem, getFullPath) {
  EXPECT_EQ(d, sgRootDir.getFullPath());
  EXPECT_EQ(pathDir0, dir0->getFullPath());
  EXPECT_EQ(pathDir0_0, dir0_0->getFullPath());
  EXPECT_EQ(pathDir0_1, dir0_1->getFullPath());
  EXPECT_EQ(pathDir0_0_0, dir0_0_0->getFullPath());
  EXPECT_EQ(pathDir0_0_1, dir0_0_1->getFullPath());
  EXPECT_EQ(pathDir0_1_0, dir0_1_0->getFullPath());
  EXPECT_EQ(pathDir0_1_1, dir0_1_1->getFullPath());
}

TEST_F(TestConsoleSystem, findRelativePath) {
  EXPECT_EQ(&sgRootDir, AbsDirUtil::findPath(".", &sgRootDir));
  EXPECT_EQ(0, AbsDirUtil::findPath("..", &sgRootDir));
  EXPECT_EQ(
      &sgRootDir, AbsDirUtil::findPath("." + pathDir0 + ".." + d, &sgRootDir));
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

TEST_F(TestConsoleSystem, findAbsolutePath) {
  EXPECT_EQ(dir0, AbsDirUtil::findPath(pathDir0, &sgRootDir));
  EXPECT_EQ(dir0_0, AbsDirUtil::findPath(pathDir0_0, &sgRootDir));
  EXPECT_EQ(dir0_1, AbsDirUtil::findPath(pathDir0_1, &sgRootDir));
  EXPECT_EQ(dir0_0_0, AbsDirUtil::findPath(pathDir0_0_0, &sgRootDir));
  EXPECT_EQ(dir0_0_1, AbsDirUtil::findPath(pathDir0_0_1, &sgRootDir));
  EXPECT_EQ(dir0_1_0, AbsDirUtil::findPath(pathDir0_1_0, &sgRootDir));
  EXPECT_EQ(dir0_1_1, AbsDirUtil::findPath(pathDir0_1_1, &sgRootDir));
}
}

#endif /* TESTABSDIR_H */
