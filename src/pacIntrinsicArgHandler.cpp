#include "pacStable.h"
#include "pacCommand.h"
#include "pacIntrinsicArgHandler.h"
#include "pacAbsDir.h"
#include "pacConsole.h"

namespace pac {

//------------------------------------------------------------------------------
StringArgHandler::StringArgHandler(
    const std::string& name, const std::string& text)
    : ArgHandler(name) {
  mStrings.insert(text);
  setPromptType(PT_PROMPTANDCOMPLETE);
}

//------------------------------------------------------------------------------
StringArgHandler::StringArgHandler(const std::string& name) : ArgHandler(name) {
  setPromptType(PT_PROMPTANDCOMPLETE);
}

//------------------------------------------------------------------------------
StringArgHandler* StringArgHandler::insert(const std::string& s) {
  mStrings.insert(s);
  return this;
}

//------------------------------------------------------------------------------
void StringArgHandler::remove(const std::string& s) { mStrings.erase(s); }

//------------------------------------------------------------------------------
void StringArgHandler::populatePromptBuffer(const std::string& s) {
  RaiiConsoleBuffer raii;
  StringVector sv;
  std::for_each(
      mStrings.begin(), mStrings.end(), [&](const std::string& v) -> void {
        if (s.empty() || StringUtil::startsWith(v, s, true)) {
          appendPromptBuffer(v);
        }
      });
}

//------------------------------------------------------------------------------
bool StringArgHandler::doValidate(const std::string& s) { return exist(s); }

//------------------------------------------------------------------------------
bool StringArgHandler::exist(const std::string& value) {
  return mStrings.find(value) != mStrings.end();
}

//------------------------------------------------------------------------------
BlankArgHandler::BlankArgHandler() : ArgHandler("blank") {}

//------------------------------------------------------------------------------
void BlankArgHandler::populatePromptBuffer(const std::string& s) {
  appendPromptBuffer("blank");
}

//------------------------------------------------------------------------------
bool BlankArgHandler::doValidate(const std::string& s) { return s.empty(); }

//------------------------------------------------------------------------------
PathArgHandler::PathArgHandler() : ArgHandler("path"), mDir(0) {
  setPromptType(PT_PROMPTANDCOMPLETE);
}

//------------------------------------------------------------------------------
PathArgHandler::PathArgHandler(const PathArgHandler& rhs) : ArgHandler(rhs) {
  setDir(sgConsole.getCwd());
}

//------------------------------------------------------------------------------
void PathArgHandler::populatePromptBuffer(const std::string& s) {
  RaiiConsoleBuffer raii;

  const std::string&& head = StringUtil::getHead(s);
  const std::string&& tail = StringUtil::getTail(s);
  AbsDir* headDir = AbsDirUtil::findPath(head, mDir);
  std::for_each(headDir->beginChildIter(), headDir->endChildIter(),
      [&](AbsDir* v) -> void {
        if (tail.empty() || StringUtil::startsWith(v->getName(), tail, true)) {
          appendPromptBuffer(v->getName());
        }
      });
}

//------------------------------------------------------------------------------
bool PathArgHandler::doValidate(const std::string& s) {
  setPathDir(AbsDirUtil::findPath(s, mDir));
  return mPathDir != 0;
}

//------------------------------------------------------------------------------
void PathArgHandler::completeTyping(const std::string& s) {
  const std::string& tail = StringUtil::getTail(s);
  const std::string&& iden =
      StdUtil::getIdenticalString(mPromptBuffer.begin(), mPromptBuffer.end());
  // just check again
  if (!tail.empty() && !StringUtil::startsWith(iden, tail))
    PAC_EXCEPT(Exception::ERR_INVALID_STATE,
        "complete string don't starts with typing!!!!");

  sgConsole.complete(iden.substr(tail.size()));
}

//------------------------------------------------------------------------------
CmdArgHandler::CmdArgHandler() : StringArgHandler("cmd") {
  std::for_each(sgCmdLib.beginCmdMapIterator(), sgCmdLib.endCmdMapIterator(),
      [&](const CommandLib::CmdMap::value_type& v)
          -> void { mStrings.insert(v.first); });
}

//------------------------------------------------------------------------------
ParamArgHandler::ParamArgHandler() : StringArgHandler("param"), mDir(0) {
  setPromptType(PT_PROMPTANDCOMPLETE);
}

//------------------------------------------------------------------------------
void ParamArgHandler::runtimeInit() {
  setUpWd();
  if (!mDir) PAC_EXCEPT(Exception::ERR_INVALID_STATE, "0 dir");
  StringVector&& sv = mDir->getParameters();
  this->insert(sv.begin(), sv.end());
}

//------------------------------------------------------------------------------
void ParamArgHandler::setUpWd() {
  mDir = sgConsole.getCwd();
  Node* node = getTreeNode();
  if (node) {
    Node* parentNode = node->getParent();
    if (parentNode && !parentNode->isRoot()) {
      ArgHandler* handler = parentNode->getArgHandler();
      if (handler->getName() == "path") {
        mDir = AbsDirUtil::findPath(handler->getValue(), mDir);
      }
    }
  }
}

//------------------------------------------------------------------------------
PparamArgHandler::PparamArgHandler() : TreeArgHandler("pparam") {
  mRoot->addChildNode("param", "param")->endBranch("0");
  mRoot->addChildNode("path", "path")
      ->addChildNode("param", "param")
      ->endBranch("1");
}

//------------------------------------------------------------------------------
ParamArgHandler* PparamArgHandler::getParamHandler() {
  if (!mMatchedLeaf) PAC_EXCEPT(Exception::ERR_INVALID_STATE, "0 matched leaf");
  Node* node = mMatchedLeaf->getAncestorNode("param");
  return static_cast<ParamArgHandler*>(node->getArgHandler());
}

//------------------------------------------------------------------------------
ValueArgHandler::ValueArgHandler() : ArgHandler("value"), mHandler(0), mDir(0) {
  setPromptType(PT_PROMPTANDCOMPLETE);
}

//------------------------------------------------------------------------------
ValueArgHandler::ValueArgHandler(const ValueArgHandler& rhs)
    : ArgHandler(rhs), mHandler(0), mDir(0) {}

//------------------------------------------------------------------------------
void ValueArgHandler::runtimeInit() {
  // get param name and working dir form param handler
  ParamArgHandler* paramHandler = getParamHandler();
  setDir(paramHandler->getDir());
  const std::string& param = paramHandler->getValue();
  const std::string& ahName = mDir->getValueArgHandler(param);
  setHandler(sgArgLib.createArgHandler(ahName));
}

//------------------------------------------------------------------------------
void ValueArgHandler::populatePromptBuffer(const std::string& s) {
  PacAssert(mHandler, "0 handler in value handler");
  mHandler->populatePromptBuffer(s);
  mPromptBuffer.assign(
      mHandler->beginPromptBuffer(), mHandler->endPromptBuffer());
}

//------------------------------------------------------------------------------
bool ValueArgHandler::doValidate(const std::string& s) {
  PacAssert(mHandler, "0 handler in value handler");
  return mHandler->validate(s);
}

//------------------------------------------------------------------------------
ParamArgHandler* ValueArgHandler::getParamHandler() {
  Node* node = this->getTreeNode();
  if (!node) PAC_EXCEPT(Exception::ERR_INVALID_STATE, "0 node");
  node = node->getParent();
  if (!node) PAC_EXCEPT(Exception::ERR_INVALID_STATE, "0 node");
  if (node->isRoot()) PAC_EXCEPT(Exception::ERR_INVALID_STATE, "root node");
  ArgHandler* handler = node->getArgHandler();
  if (handler->getName() == "param")
    return static_cast<ParamArgHandler*>(handler);
  if (handler->getName() == "pparam")
    return static_cast<PparamArgHandler*>(handler)->getParamHandler();
  PAC_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "param not found");
}
}
