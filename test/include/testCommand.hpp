#ifndef TESTCOMMAND_HPP
#define TESTCOMMAND_HPP

#include "pacConsole.h"
#include "pacCommand.h"
#include "pacException.h"
#include <gtest/gtest.h>

namespace pac {

class TestCommand : public ::testing::Test {
protected:
  virtual void SetUp() { mCmd = sgCmdLib.createCommand("ls"); }

  virtual void TearDown() { delete mCmd; }

  Command* mCmd;
};

TEST_F(TestCommand, setArgsAndOptions) {
  mCmd->setArgsAndOptions("abc");
  EXPECT_STREQ("abc", mCmd->getArgs().c_str());
  EXPECT_STREQ("", mCmd->getOptions().c_str());
  mCmd->setArgsAndOptions(" abc");
  EXPECT_STREQ("abc", mCmd->getArgs().c_str());
  EXPECT_STREQ("", mCmd->getOptions().c_str());
  mCmd->setArgsAndOptions(" abc ");
  EXPECT_STREQ("abc ", mCmd->getArgs().c_str());
  EXPECT_STREQ("", mCmd->getOptions().c_str());
  mCmd->setArgsAndOptions("-a abc");
  EXPECT_STREQ("abc", mCmd->getArgs().c_str());
  EXPECT_STREQ("a", mCmd->getOptions().c_str());
  mCmd->setArgsAndOptions(" -a abc");
  EXPECT_STREQ("abc", mCmd->getArgs().c_str());
  EXPECT_STREQ("a", mCmd->getOptions().c_str());
  mCmd->setArgsAndOptions(" -a abc ");
  EXPECT_STREQ("abc ", mCmd->getArgs().c_str());
  EXPECT_STREQ("a", mCmd->getOptions().c_str());
  mCmd->setArgsAndOptions(" -a abc ");
  EXPECT_STREQ("abc ", mCmd->getArgs().c_str());
  EXPECT_STREQ("a", mCmd->getOptions().c_str());
  mCmd->setArgsAndOptions(" -a abc -b ");
  EXPECT_STREQ("abc ", mCmd->getArgs().c_str());
  EXPECT_STREQ("ab", mCmd->getOptions().c_str());
  mCmd->setArgsAndOptions(" -a abc -bc def ");
  EXPECT_STREQ("abc def ", mCmd->getArgs().c_str());
  EXPECT_STREQ("abc", mCmd->getOptions().c_str());
  mCmd->setArgsAndOptions(" /abc -bc def ");
  EXPECT_STREQ("/abc def ", mCmd->getArgs().c_str());
  EXPECT_STREQ("bc", mCmd->getOptions().c_str());

  EXPECT_THROW(mCmd->setArgsAndOptions("d-"), InvalidParametersException);
  EXPECT_THROW(mCmd->setArgsAndOptions(" abc d-"), InvalidParametersException);
  EXPECT_THROW(
      mCmd->setArgsAndOptions(" abc d- abc"), InvalidParametersException);
}
}

#endif /* TESTCOMMAND_HPP */
