#include "pacStable.h"
#include "pacConsole.h"
#include "pacCommand.h"
#include "pacArgHandler.h"
#include "pacUiConsole.h"
#include "pacAbsDir.h"
#include "pacConsolePattern.h"
#include "pacCmdHistory.h"
#include "pacAbsDir.h"
#include <boost/regex.hpp>

namespace pac {

template <>
Console* Singleton<Console>::msSingleton = 0;

//------------------------------------------------------------------------------
Console::Console(UiConsole* ui)
    : StringInterface("console"),
      mIsBuffering(false),
      mDir(0),
      mUi(ui),
      mPattern(0),
      mCmdHistory(0) {
  if (!ui) PAC_EXCEPT(Exception::ERR_INVALIDPARAMS, "0 ui");
}

//------------------------------------------------------------------------------
Console::~Console() {
  delete &sgCmdLib;

  mCmdHistory = 0;

  // clean arg lib
  delete &sgArgLib;
  delete &sgLogger;
  delete &sgRootDir;
  delete mCmdHistory;
}

//------------------------------------------------------------------------------
void Console::init() {
  new Logger();
  initCmdHistory();
  initDir();

  new CommandLib();
  new ArgHandlerLib();

  sgArgLib.init();
  sgCmdLib.init();

  initConoslePattern();
}

//------------------------------------------------------------------------------
void Console::initConoslePattern() {
  this->mPattern = new DefaultPattern(mUi->getTextWidth());
}

//------------------------------------------------------------------------------
void Console::initCmdHistory() { mCmdHistory = new CmdHistory(); }

//------------------------------------------------------------------------------
void Console::initDir() {
  new RootDir();
  setCwd(&sgRootDir);
  AbsDir* uiDir = new AbsDir("uiConsole", mUi);
  sgRootDir.addChild(uiDir);
}

//------------------------------------------------------------------------------
bool Console::execute(const std::string& cmdLine /*= ""*/) {
  std::string line = cmdLine.empty() ? mUi->getCmdLine() : cmdLine;
  StringUtil::trim(line);
  if (line.empty()) return false;

  fakeOutputDirAndCmd(line);
  mCmdHistory->push(line);

  mUi->setCmdLine("");

  boost::regex reCmd2("^\\s*(\\w+)(\\s*.*)$");
  boost::smatch m;
  if (boost::regex_match(line, m, reCmd2)) {
    Command* cmd = sgCmdLib.createCommand(m[1]);
    if (cmd) {
      cmd->setArgsAndOptions(m[2]);
      return cmd->execute();
    }
  }
  return true;
}

//------------------------------------------------------------------------------
void Console::prompt() {
  std::string&& cmdLine = mUi->getCmdLine();
  StringUtil::trim(cmdLine, true, false);
  if (cmdLine.empty()) return;

  fakeOutputDirAndCmd(cmdLine);

  boost::regex reCmd("^\\s*(\\w*)$");
  boost::smatch m;
  if (boost::regex_match(cmdLine, m, reCmd)) {
    // prompt command name
    this->promptCommandName(m[1]);
  } else {
    // prompt argument
    // extract command name, args and options
    boost::regex reCmd2("^\\s*(\\w+)(\\s*.*)$");
    if (boost::regex_match(cmdLine, m, reCmd2)) {
      Command* cmd = sgCmdLib.createCommand(m[1]);
      if (cmd) {
        cmd->setArgsAndOptions(m[2]);
        cmd->prompt();
      }
    } else {
      PAC_EXCEPT(Exception::ERR_INVALID_STATE,
          "invalid state caused by command line :" + cmdLine);
    }
  }
}

//------------------------------------------------------------------------------
Console& Console::output(const std::string& s, int type /*= 1*/) {
  if (type == 1 && mIsBuffering)
    this->appendBuffer(s);
  else
    mUi->output(s, type);

  return *this;
}

//------------------------------------------------------------------------------
Console& Console::outputLine(const std::string& s, int type /*= 1*/) {
  if (mIsBuffering)
    PAC_EXCEPT(
        Exception::ERR_INVALID_STATE, "Can not output line while buffering.");
  mUi->outputLine(s, type);
  return *this;
}

//------------------------------------------------------------------------------
Console& Console::complete(const std::string& s) {
  mUi->complete(s);
  return *this;
}

//------------------------------------------------------------------------------
void Console::setCwd(AbsDir* dir) { mDir = dir; }

//------------------------------------------------------------------------------
void Console::startBuffer() {
  if (mIsBuffering)
    PAC_EXCEPT(
        Exception::ERR_INVALID_STATE, "It's wrong to start buffer twice");
  if (!mBuffer.empty())
    PAC_EXCEPT(Exception::ERR_INVALID_STATE,
        "It'w wrong to start buffer when buffer is not empty");
  mIsBuffering = true;
}

//------------------------------------------------------------------------------
void Console::endBuffer() {
  PacAssert(mIsBuffering, "It'w wrong to end buffer without start it");
  mIsBuffering = false;
  if (!mBuffer.empty())
    mUi->outputNoAutoWrap(
        mPattern->applyPattern(mBuffer.begin(), mBuffer.end()));

  mBuffer.clear();
}

//------------------------------------------------------------------------------
void Console::rollCommand(bool backWard /*= true*/) {
  if (backWard)
    mUi->setCmdLine(mCmdHistory->previous());
  else
    mUi->setCmdLine(mCmdHistory->next());
}

//------------------------------------------------------------------------------
void Console::promptCommandName(const std::string& cmdName) {
  ArgHandler* handler = sgArgLib.createArgHandler("cmd");
  handler->prompt(cmdName);
  delete handler;
}

//------------------------------------------------------------------------------
void Console::fakeOutputDirAndCmd(const std::string& cmdLine) {
  output(mDir->getFullPath() + ":" + cmdLine);
}

//------------------------------------------------------------------------------
void Console::appendBuffer(const std::string& v) { mBuffer.push_back(v); }

//------------------------------------------------------------------------------
RaiiConsoleBuffer::RaiiConsoleBuffer() { sgConsole.startBuffer(); }

//------------------------------------------------------------------------------
RaiiConsoleBuffer::~RaiiConsoleBuffer() { sgConsole.endBuffer(); }
}
