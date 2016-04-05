#include "pacStable.h"
#include "pacConsole.h"
#include "pacCommand.h"
#include "pacArgHandler.h"
#include "pacConsoleUI.h"
#include "pacAbsDir.h"
#include "pacConsolePattern.h"
#include "pacCmdHistory.h"
#include "pacAbsDir.h"
#include <boost/regex.hpp>

namespace pac {

template <>
Console* Singleton<Console>::msSingleton = 0;

//------------------------------------------------------------------------------
Console::Console(ConsoleUI* ui)
    : StringInterface("console", false),
      mIsBuffering(false),
      mDir(0),
      mAlternateDir(0),
      mRootDir(0),
      mUi(ui),
      mPattern(0),
      mCmdHistory(0) {
  if (!ui) PAC_EXCEPT(Exception::ERR_INVALIDPARAMS, "0 ui");
}

//------------------------------------------------------------------------------
Console::~Console() {
  delete &sgCmdLib;
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
  initArghandler();
  initCommand();
  initDir();
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
  //build root
  mRootDir = new AbsDir(pac::delim);
  setCwd(&sgRootDir);
  AbsDir* uiDir = new AbsDir("consoleUi", mUi);
  mRootDir->addChild(uiDir, false);
}

//------------------------------------------------------------------------------
void Console::initArghandler() {
  new ArgHandlerLib();
  sgArgLib.init();
}

//------------------------------------------------------------------------------
void Console::initCommand() {
  new CommandLib();
  sgCmdLib.init();
}

//------------------------------------------------------------------------------
bool Console::execute(const std::string& cmdLine /*= ""*/) {
  std::string line = cmdLine.empty() ? mUi->getCmdLine() : cmdLine;
  StringUtil::trim(line);
  if (line.empty()) return false;

  sgLogger.logMessage(
      "************************************************************");
  sgLogger.logMessage("executing command \"" + line + "\"");

  fakeOutputDirAndCmd(line);
  mCmdHistory->push(line);

  mUi->setCmdLine("");

  boost::regex reCmd2("^\\s*(\\w+)(\\s*.*)$");
  boost::smatch m;
  if (boost::regex_match(line, m, reCmd2)) {
    Command* cmd = sgCmdLib.createCommand(m[1]);
    if (cmd) {
      cmd->setArgsAndOptions(m[2]);
      if (cmd->execute()) {
        sgLogger.logMessage("finished executing command \"" + line + "\"");
        sgLogger.logMessage(
            "************************************************************");
        return true;
      }
    } else {
      outputLine("unknown command : " + m[1]);
    }
  } else {
    outputLine("unknown input");
  }
  sgLogger.logMessage("failed executing command \"" + line + "\"");
  sgLogger.logMessage(
      "************************************************************");
  return false;
}

//------------------------------------------------------------------------------
void Console::prompt() {
  std::string&& cmdLine = mUi->getCmdLine();
  StringUtil::trim(cmdLine, true, false);
  // fakeOutputDirAndCmd(cmdLine);

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
void Console::setCwd(AbsDir* dir) {
  mAlternateDir = mDir;
  mDir = dir;
  std::string&& cwd = dir->getFullPath();
  if (cwd.size() > 1) {
    // replace trailing / with " "
    *cwd.rbegin() = ' ';
  } else {
    // add trailing " " for root
    cwd.append(" ");
  }
  mUi->setCwd(cwd);
  sgLogger.logMessage("set cwd to \"" + dir->getName() + "\"");
}

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
    mUi->output(mPattern->applyPattern(mBuffer.begin(), mBuffer.end()));

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
void Console::deleteDir(AbsDir* dir) {
  if (dir == mRootDir) {
    mDir = 0;
    mAlternateDir = 0;
    return;
  }
  if (mDir == dir) mDir = &sgRootDir;
  if (mAlternateDir == dir) mAlternateDir = 0;
}

//------------------------------------------------------------------------------
void Console::cleanTempDirs() { cleanTempDir(&sgRootDir); }

//------------------------------------------------------------------------------
bool Console::isActive() { return mUi->getVisible(); }

//------------------------------------------------------------------------------
void Console::setActive(bool b) {
  mUi->setVisible(b);
  mUi->setFocus(b);
}

//------------------------------------------------------------------------------
void Console::toggleActive() { return setActive(!isActive()); }

//------------------------------------------------------------------------------
void Console::resize() { mPattern->setTextWidth(mUi->getTextWidth()); }

//------------------------------------------------------------------------------
void Console::promptCommandName(const std::string& cmdName) {
  ArgHandler* handler = sgArgLib.createArgHandler("cmd");
  handler->prompt(cmdName);
  delete handler;
}

//------------------------------------------------------------------------------
void Console::fakeOutputDirAndCmd(const std::string& cmdLine) {
  outputLine(mDir->getFullPath() + " " + cmdLine);
}

//------------------------------------------------------------------------------
void Console::appendBuffer(const std::string& v) { mBuffer.push_back(v); }

//------------------------------------------------------------------------------
void Console::cleanTempDir(AbsDir* dir) {
  if (dir->getTemp()) {
    delete dir;
    return;
  }
  // operator on copy
  AbsDirs dirs(dir->beginChildIter(), dir->endChildIter());
  std::for_each(
      dirs.begin(), dirs.end(), [&](AbsDir* d) -> void { cleanTempDir(d); });
}

//------------------------------------------------------------------------------
RaiiConsoleBuffer::RaiiConsoleBuffer() { sgConsole.startBuffer(); }

//------------------------------------------------------------------------------
RaiiConsoleBuffer::~RaiiConsoleBuffer() { sgConsole.endBuffer(); }
}
