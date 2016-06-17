#include "pacStable.h"
#include "pacIntrinsicCmd.h"
#include "pacConsole.h"
#include "pacAbsDir.h"
#include "pacArgHandler.h"
#include "pacStdUtil.h"
#include "pacIntrinsicArgHandler.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace pac {

//------------------------------------------------------------------------------
LsCmd::LsCmd() : Command("ls") {}

//------------------------------------------------------------------------------
bool LsCmd::doExecute() {
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  AbsDir* curDir = sgConsole.getCwd();

  if (handler->getMatchedBranch() == "1") {
    Node* pathNode = handler->getMatchedNode("path");
    std::for_each(pathNode->beginLoopValueIter(), pathNode->endLoopValueIter(),
        [&](const std::string& path) -> void {
          AbsDir* dir = AbsDirUtil::findPath(path, curDir);
          sgConsole.outputLine(dir->getName() + ":");
          outputChildren(dir);
        });
  } else {
    // output current dir
    outputChildren(curDir);
  }
  return true;
}

//------------------------------------------------------------------------------
bool LsCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  Node* root = handler->getRoot();
  root->eb("0");
  root->acn("path", "path", Node::NT_LOOP)->eb("1");
  this->mArgHandler = handler;
  return true;
}

//------------------------------------------------------------------------------
void LsCmd::outputChildren(AbsDir* dir) {
  RaiiConsoleBuffer raii;
  std::for_each(dir->beginChildIter(), dir->endChildIter(),
      [&](AbsDir* childDir) -> void { sgConsole.output(childDir->getName()); });
}

//------------------------------------------------------------------------------
PwdCmd::PwdCmd() : Command("pwd", "blank") {}

//------------------------------------------------------------------------------
bool PwdCmd::doExecute() {
  AbsDir* curDir = sgConsole.getCwd();
  sgConsole.outputLine(curDir->getFullPath());
  return true;
}

//------------------------------------------------------------------------------
CtdCmd::CtdCmd() : Command("ctd", "blank") {}

//------------------------------------------------------------------------------
bool CtdCmd::doExecute() {
  sgConsole.cleanTempDirs();
  return true;
}

//------------------------------------------------------------------------------
CdCmd::CdCmd() : Command("cd") {}

//------------------------------------------------------------------------------
bool CdCmd::doExecute() {
  TreeArgHandler* tree = static_cast<TreeArgHandler*>(mArgHandler);
  const std::string& branch = tree->getMatchedBranch();
  AbsDir* targetDir;
  if (branch == "0") {
    AbsDir* curDir = sgConsole.getCwd();
    targetDir = AbsDirUtil::findPath(mArgHandler->getValue(), curDir);
  } else {
    targetDir = sgConsole.getAlternateDir();
  }
  if (!targetDir) {
    sgConsole.outputLine("0 target dir");
    return false;
  }
  sgConsole.setCwd(targetDir);
  return true;
}

//------------------------------------------------------------------------------
bool CdCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  this->mArgHandler = handler;
  Node* root = handler->getRoot();
  root->acn("path")->eb("0");
  root->acn("ltl_-")->eb("1");
  return true;
}

//------------------------------------------------------------------------------
SetCmd::SetCmd() : Command("set") {}

//------------------------------------------------------------------------------
void SetCmd::outputErrMessage(const std::string& args) {
  TreeArgHandler* tree = static_cast<TreeArgHandler*>(mArgHandler);
  ArgHandler* valueHandler = tree->getNode("value")->getArgHandler();
  if (valueHandler->getName() == "value") {
    mArgHandler->outputErrMessage(args);
  } else {
    //@TODO , extract value component in args
    valueHandler->outputErrMessage("inputed value ");
  }
}

//------------------------------------------------------------------------------
bool SetCmd::doExecute() {
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  const std::string& branch = handler->getMatchedBranch();
  AbsDir* dir = 0;
  if (branch == "0") {
    // set param value
    dir = sgConsole.getCwd();
  } else if (branch == "1") {
    // set path param value
    PathArgHandler* pathHandler =
        static_cast<PathArgHandler*>(handler->getMatchedNodeHandler("path"));
    dir = pathHandler->getPathDir();
  } else {
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "illegal branch");
  }

  const std::string& param = handler->getMatchedNodeValue("param");
  ArgHandler* valueHandler = handler->getMatchedNodeHandler("value");
  dir->setParameter(param, valueHandler);

  return true;
}

//------------------------------------------------------------------------------
bool SetCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  this->mArgHandler = handler;
  Node* root = handler->getRoot();
  root->acn("param")->acn("value")->eb("0");
  root->acn("path")->acn("param")->acn("value")->eb("1");
  return true;
}

//------------------------------------------------------------------------------
GetCmd::GetCmd() : Command("get") {}

//------------------------------------------------------------------------------
bool GetCmd::doExecute() {
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);

  const std::string& branch = handler->getMatchedBranch();
  if (branch != "0" && branch != "1" && branch != "2" && branch != "3" &&
      branch != "4" && branch != "5")
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "invalid branch:" + branch);

  AbsDir* curDir = sgConsole.getCwd();

  std::string param;

  // get path param ("4")
  // get path ltl_regex regex("5")
  if (branch == "0" || branch == "1" || branch == "2") {
    if (branch == "0") {
      // get ("0")
      outputProperties(curDir);
    } else if (branch == "1") {
      // get param ("1")
      outputProperties(curDir, handler->getMatchedNodeValue("param"));
    } else {
      // get ltl_regex regex("2")
      outputProperties(curDir, "", handler->getMatchedNodeValue("regex"));
    }
  } else if (branch == "3" || branch == "4" || branch == "5") {
    AbsDir* dir =
        AbsDirUtil::findPath(handler->getMatchedNodeValue("path"), curDir);
    if (branch == "3") {
      // get path ("3")
      outputProperties(dir);
    } else if (branch == "4") {
      // get param ("1")
      outputProperties(dir, handler->getMatchedNodeValue("param"));
    } else {
      // get ltl_regex regex("2")
      outputProperties(dir, "", handler->getMatchedNodeValue("regex"));
    }
  } else {
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "invalid branch:" + branch);
  }
  return true;
}
//------------------------------------------------------------------------------
bool GetCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  Node* root = handler->getRoot();
  this->mArgHandler = handler;

  // get ("0")
  root->eb("0");
  // get param ("1")
  root->acn("param")->eb("1");
  // get ltl_regex regex("2")
  root->acn("ltl_regex")->acn("regex")->eb("2");

  Node* pathNode = root->acn("path");
  // get path ("3")
  pathNode->eb("3");
  // get path param ("4")
  pathNode->acn("param")->eb("4");
  // get path ltl_regex regex("5")
  pathNode->acn("ltl_regex")->acn("regex")->eb("5");
  return true;
}

//------------------------------------------------------------------------------
void GetCmd::outputProperties(AbsDir* dir, const std::string& param /*= ""*/,
    const std::string& reExp /*= ""*/) {
  RaiiConsoleBuffer raii;
  if (!param.empty()) {
    sgConsole.output(param + " : " + dir->getParameter(param));
    return;
  }

  boost::regex regex(reExp);
  StringVector&& sv = dir->getParameters();
  std::for_each(sv.begin(), sv.end(), [&](const std::string& v) -> void {
    if (reExp.empty() || boost::regex_match(v, regex))
      sgConsole.output(v + " : " + dir->getParameter(v));
  });
}

//------------------------------------------------------------------------------
SzCmd::SzCmd() : Command("sz") {}

//------------------------------------------------------------------------------
bool SzCmd::doExecute() {
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  const std::string& branch = handler->getMatchedBranch();
  bool recursive = !hasOption('R');
  const std::string& fileName = handler->getMatchedNodeValue("id");

  boost::filesystem::ofstream ofs(fileName, std::fstream::trunc);
  AbsDir* curDir = sgConsole.getCwd();

  if (branch == "0") {
    // sz id
    curDir->serialize(ofs, recursive);
  } else if (branch == "1") {
    // sz id path
    AbsDir* dir =
        AbsDirUtil::findPath(handler->getMatchedNodeValue("path"), curDir);
    dir->serialize(ofs, recursive);
  } else {
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "unknown branch");
  }

  return true;
}

//------------------------------------------------------------------------------
bool SzCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  Node* root = handler->getRoot();
  root->eb("0");               // sz
  root->acn("path")->eb("1");  // sz path
  this->mArgHandler = handler;
  return true;
}

//------------------------------------------------------------------------------
EchoCmd::EchoCmd() : ManualCommand("echo") {}

//------------------------------------------------------------------------------
bool EchoCmd::doExecute() {
  sgConsole.outputLine(mArgHandler->getValue());
  return true;
}
}
