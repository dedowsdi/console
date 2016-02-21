#include "pacCommand.h"
#include "pacIntrinsicCmd.h"
#include "pacArgHandler.h"
#include "pacException.h"
#include "pacLogger.h"
#include <boost/regex.hpp>

namespace pac {

//------------------------------------------------------------------------------
Command::Command(const std::string& name) : mName(name), mArgHandler(0) {
  // change this in subclass ctor if you want to use existing arg handler
  mAhName = getDefAhName();
}

//------------------------------------------------------------------------------
Command::Command(const Command& rhs)
    : mName(rhs.getName()), mAhName(rhs.getAhName()), mArgHandler(0) {
  mArgHandler = sgArgLib.createArgHandler(mAhName);
}

//------------------------------------------------------------------------------
Command::~Command() {
  if (mArgHandler == 0)
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "0 arg handler");
  delete mArgHandler;
  mArgHandler = 0;
}

//------------------------------------------------------------------------------
void Command::prompt() { getArgHandler()->prompt(mArgs); }

//------------------------------------------------------------------------------
bool Command::execute() {
  if (getArgHandler()->validate(mArgs)) {
    bool res = this->doExecute();
    return res;
  } else {
    getArgHandler()->outputErrMessage(mArgs);
    return false;
  }
}

//------------------------------------------------------------------------------
void Command::setArgsAndOptions(const std::string& v) {
  boost::smatch m;
  // check if - after \S
  boost::regex reInvalid("\\S-");
  if (boost::regex_search(v.begin(), v.end(), m, reInvalid))
    PAC_EXCEPT(Exception::ERR_INVALIDPARAMS,
        v + " is illegal, - after nonspace character");

  mArgs.clear();
  mOptions.clear();
  // extract options
  boost::regex reOptions("-(\\w+)");
  // extract args,[^-]* does'nt work here. It's necessary to use (^[]*|[]+)
  // style
  boost::regex reArgs("(?:^[^-\\w]*|[^-\\w]+)\\<\\w+\\>\\s*");

  // get options
  std::string::const_iterator start = v.begin();
  while (boost::regex_search(start, v.end(), m, reOptions)) {
    mOptions += m[1];
    start = m[0].second;
  }

  // get args
  start = v.begin();
  while (boost::regex_search(start, v.end(), m, reArgs)) {
    mArgs += m[0];
    start = m[0].second;
  }
}

//------------------------------------------------------------------------------
ArgHandler* Command::getArgHandler() { return mArgHandler; }

//------------------------------------------------------------------------------
Command* Command::init() {
  buildArgHandler();
  if (mArgHandler->getName() == getDefAhName())
    sgArgLib.registerArgHandler(mArgHandler);

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
  PacAssert(!cmdProto->getName().empty(), "empty cmd name");
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
  this->registerCommand((new LpCmd())->init());
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
