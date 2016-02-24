#include "pacStable.h"
#include "pacIntrinsicCmd.h"
#include "pacConsole.h"
#include "pacAbsDir.h"
#include "pacArgHandler.h"
#include "pacIntrinsicArgHandler.h"

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
  root->endBranch("0");
  root->addChildNode("path", "path", Node::NT_LOOP)->endBranch("1");
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
CdCmd::CdCmd() : Command("cd", "path") {}

//------------------------------------------------------------------------------
bool CdCmd::doExecute() {
  AbsDir* curDir = sgConsole.getCwd();
  AbsDir* targetDir = AbsDirUtil::findPath(mArgHandler->getValue(), curDir);
  sgConsole.setCwd(targetDir);
  return true;
}

//------------------------------------------------------------------------------
SetCmd::SetCmd() : Command("set") {}

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
  }else{
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "illegal branch");
  }

  const std::string& param = handler->getMatchedNodeValue("param");
  ValueArgHandler* valueHandler =
      static_cast<ValueArgHandler*>(handler->getMatchedNodeHandler("value"));
  dir->setParameter(param, valueHandler->getHandler());

  return true;
}

//------------------------------------------------------------------------------
bool SetCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  this->mArgHandler = handler;
  Node* root = handler->getRoot();
  root->addChildNode("param")->addChildNode("value")->endBranch("0");
  root->addChildNode("path")
      ->addChildNode("param")
      ->addChildNode("value")
      ->endBranch("1");
  return true;
}

//------------------------------------------------------------------------------
LpCmd::LpCmd() : Command("lp") {}

//------------------------------------------------------------------------------
bool LpCmd::doExecute() {
  TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
  AbsDir* curDir = sgConsole.getCwd();

  const std::string& branch = handler->getMatchedBranch();
  if (branch == "0") {
    // lp
    outputProperties(curDir);
  } else if (branch == "1") {
    // lp param
    const std::string& param = handler->getMatchedNode("param")->getValue();
    sgConsole.outputLine(param + " : " + curDir->getParameter(param));
  } else if (branch == "2") {
    // lp path
    PathArgHandler* pathHandler =
        static_cast<PathArgHandler*>(handler->getMatchedNodeHandler("path"));
    outputProperties(pathHandler->getPathDir());
  } else if (branch == "3") {
    // lp path param
    const std::string& param = handler->getMatchedNode("param")->getValue();
    PathArgHandler* pathHandler =
        static_cast<PathArgHandler*>(handler->getMatchedNodeHandler("path"));
    sgConsole.outputLine(
        param + " : " + pathHandler->getPathDir()->getParameter(param));
  }
  return true;
}
//------------------------------------------------------------------------------
bool LpCmd::buildArgHandler() {
  TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
  Node* root = handler->getRoot();
  root->endBranch("0");                                  // lp
  root->addChildNode("param", "param")->endBranch("1");  // lp param
  Node* pathNode = root->addChildNode("path", "path");
  pathNode->endBranch("1");                                  // lp path
  pathNode->addChildNode("param", "param")->endBranch("2");  // lp path param
  this->mArgHandler = handler;
  return true;
}

//------------------------------------------------------------------------------
void LpCmd::outputProperties(AbsDir* dir) {
  RaiiConsoleBuffer raii;
  StringVector&& params = dir->getParameters();
  std::for_each(params.begin(), params.end(),
      [&](const std::string& v)
          -> void { sgConsole.output(v + " : " + dir->getParameter(v)); });
}
}
