#include "pacCommand.h"
#include "pacIntrinsicCmd.h"
#include "pacIntrinsicArgHandler.h"
#include "pacArgHandler.h"
#include "pacException.h"
#include "pacLogger.h"
#include "pacStringUtil.h"
#include <boost/regex.hpp>

namespace pac {

//------------------------------------------------------------------------------
Command::Command(const std::string& name, const std::string& ahName /* = ""*/)
    : mName(name), mArgHandler(0) {
  boost::regex re("\\W");
  if (boost::regex_search(mName, re))
    PAC_EXCEPT(
        Exception::ERR_INVALIDPARAMS, "illegal character in\"" + mName + "\" ");

  if (!ahName.empty()) mArgHandler = sgArgLib.createArgHandler(ahName);
}

//------------------------------------------------------------------------------
Command::Command(const Command& rhs)
    : mName(rhs.getName()), mArgHandler(rhs.mArgHandler->clone()) {}

//------------------------------------------------------------------------------
Command::~Command() {
  if (mArgHandler == 0)
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "0 arg handler");
  delete mArgHandler;
  mArgHandler = 0;
}

//------------------------------------------------------------------------------
void Command::prompt() { mArgHandler->prompt(mArgs); }

//------------------------------------------------------------------------------
bool Command::execute() {
  // right trim
  std::string args = mArgs;
  StringUtil::trim(args, false, true);
  if (mArgHandler->validate(args)) {
    bool res = this->doExecute();
    return res;
  } else {
    mArgHandler->outputErrMessage(args);
    return false;
  }
}

//------------------------------------------------------------------------------
void Command::setArgsAndOptions(const std::string& v) {
  boost::smatch m;
  // check if - after nonspace
  boost::regex reInvalid("\\S-");
  if (boost::regex_search(v, reInvalid))
    PAC_EXCEPT(Exception::ERR_INVALIDPARAMS,
        v + " is illegal, - after nonspace character");

  mArgs.clear();
  mOptions.clear();
  // extract options
  boost::regex reOptions("-(\\S*)\\s*");

  // get options
  std::string::const_iterator start = v.begin();
  std::string::const_iterator end = v.end();
  while (boost::regex_search(start, end, m, reOptions)) {
    mOptions += m[1];
    start = m[0].second;
  }
  // remove options to get args
  mArgs = boost::regex_replace(v, reOptions, "");
  StringUtil::trim(mArgs, true, false);
}

//------------------------------------------------------------------------------
ArgHandler* Command::getArgHandler() const { return mArgHandler; }

//------------------------------------------------------------------------------
Command* Command::init() {
  if (buildArgHandler()) sgArgLib.registerArgHandler(mArgHandler->clone());

  if (!mArgHandler)
    PAC_EXCEPT(Exception::ERR_INVALID_STATE,
        "0 handler, you need to pass ahName to Command::Command() or override "
        "Command::buildArgHandler()");

  return this;
}

//------------------------------------------------------------------------------
CommandLib::~CommandLib() {
  // clean command
  std::for_each(mCmdMap.begin(), mCmdMap.end(),
      [&](CmdMap::value_type& v) -> void { delete v.second; });
  mCmdMap.clear();
}

//------------------------------------------------------------------------------
Command* CommandLib::createCommand(const std::string& cmdName) {
  CmdMap::iterator iter = mCmdMap.find(cmdName);
  if (iter != mCmdMap.end()) {
    return iter->second->clone();
  } else {
    PAC_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, cmdName + " not found");
  }
}

//------------------------------------------------------------------------------
void CommandLib::registerCommand(Command* cmdProto) {
  sgLogger.logMessage("register command " + cmdProto->getName());
  // check if it's already registerd
  CmdMap::iterator iter = std::find_if(mCmdMap.begin(), mCmdMap.end(),
      [&](CmdMap::value_type& v)
          -> bool { return v.first == cmdProto->getName(); });

  if (iter == mCmdMap.end()) {
    mCmdMap[cmdProto->getName()] = cmdProto;
  } else {
    PAC_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
        cmdProto->getName() + " already registed!");
  }
}

//------------------------------------------------------------------------------
void CommandLib::init() {
  // register intrinsic commands
  this->registerCommand((new LsCmd())->init());
  this->registerCommand((new PwdCmd())->init());
  this->registerCommand((new CdCmd())->init());
  this->registerCommand((new SetCmd())->init());
  this->registerCommand((new GetCmd())->init());
  this->registerCommand((new SzCmd())->init());

  sgArgLib.registerArgHandler(new CmdArgHandler());
}
//------------------------------------------------------------------------------
CommandLib::CmdMap::const_iterator CommandLib::beginCmdMapIterator() const {
  return mCmdMap.begin();
}

//------------------------------------------------------------------------------
CommandLib::CmdMap::const_iterator CommandLib::endCmdMapIterator() const {
  return mCmdMap.end();
}

template <>
CommandLib* Singleton<CommandLib>::msSingleton = 0;
}
