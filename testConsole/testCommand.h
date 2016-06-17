#ifndef TESTCOMMAND_HPP
#define TESTCOMMAND_HPP

#include "pacConsole.h"
#include "pacCommand.h"
#include "pacException.h"
#include <gtest/gtest.h>

namespace pac {

class TestCommand : public ::testing::Test {
protected:
  virtual void SetUp() {
    mCmd = sgCmdLib.createCommand("ls");
    mManualCmd = sgCmdLib.createCommand("echo");
  }

  virtual void TearDown() {
    delete mCmd;
    delete mManualCmd;
  }

  Command* mCmd;
  Command* mManualCmd;
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
  mCmd->setArgsAndOptions("abc - ");
  EXPECT_STREQ("abc - ", mCmd->getArgs().c_str());

  EXPECT_THROW(mCmd->setArgsAndOptions("d-"), InvalidParametersException);
  EXPECT_THROW(mCmd->setArgsAndOptions(" abc d-"), InvalidParametersException);
  EXPECT_THROW(
      mCmd->setArgsAndOptions(" abc d- abc"), InvalidParametersException);
}

TEST_F(TestCommand, setArgsAndOptionsManual){
  mManualCmd->setArgsAndOptions("abc");
  EXPECT_STREQ("abc", mManualCmd->getArgs().c_str());
  EXPECT_STREQ("", mManualCmd->getOptions().c_str());

  mManualCmd->setArgsAndOptions(" -a abc -bc def ");
  EXPECT_STREQ("abc -bc def ", mManualCmd->getArgs().c_str());
  EXPECT_STREQ("a", mManualCmd->getOptions().c_str());

}
}

#endif /* TESTCOMMAND_HPP */
