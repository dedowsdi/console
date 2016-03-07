#include "pacStable.h"
#include "pacCommand.h"
#include "pacIntrinsicArgHandler.h"
#include "pacAbsDir.h"
#include "pacConsole.h"
#include <boost/regex.hpp>
#include <cmath>

namespace pac {

//------------------------------------------------------------------------------
StringArgHandler::StringArgHandler(const std::string& name)
    : ArgHandler(name) {}

//------------------------------------------------------------------------------
StringArgHandler* StringArgHandler::insert(const std::string& s) {
  mStrings.insert(s);
  return this;
}

//------------------------------------------------------------------------------
void StringArgHandler::remove(const std::string& s) { mStrings.erase(s); }

//------------------------------------------------------------------------------
void StringArgHandler::populatePromptBuffer(const std::string& s) {
  std::for_each(
      mStrings.begin(), mStrings.end(), [&](const std::string& v) -> void {
        if (s.empty() || StringUtil::startsWith(v, s)) {
          appendPromptBuffer(v);
        }
      });
}

//------------------------------------------------------------------------------
bool StringArgHandler::doValidate(const std::string& s) {
  return mStrings.find(s) != mStrings.end();
}

//------------------------------------------------------------------------------
LiteralArgHandler::LiteralArgHandler(const std::string& text)
    : ArgHandler("ltl_" + text), mText(text) {}

//------------------------------------------------------------------------------
void LiteralArgHandler::populatePromptBuffer(const std::string& s) {
  if (StringUtil::startsWith(mText, s)) appendPromptBuffer(s);
}

//------------------------------------------------------------------------------
bool LiteralArgHandler::doValidate(const std::string& s) { return mText == s; }

//------------------------------------------------------------------------------
BlankArgHandler::BlankArgHandler() : ArgHandler("blank") {
  setPromptType(PT_PROMPTONLY);
}

//------------------------------------------------------------------------------
void BlankArgHandler::populatePromptBuffer(const std::string& s) {
  (void)s;
  appendPromptBuffer("blank");
}

//------------------------------------------------------------------------------
bool BlankArgHandler::doValidate(const std::string& s) { return s.empty(); }

//------------------------------------------------------------------------------
PathArgHandler::PathArgHandler() : ArgHandler("path"), mDir(0) {}

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
        if (tail.empty() || StringUtil::startsWith(v->getName(), tail)) {
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
ParamArgHandler::ParamArgHandler()
    : StringArgHandler("param"), mDir(0), mPathNode(0) {}

//------------------------------------------------------------------------------
void ParamArgHandler::runtimeInit() {
  mDir = sgConsole.getCwd();
  if (mPathNode) {
    mDir = AbsDirUtil::findPath(mPathNode->getValue(), mDir);
  }
  if (!mDir) PAC_EXCEPT(Exception::ERR_INVALID_STATE, "0 dir");

  //@TODO, might cause problem if dir has tons of params
  StringVector&& sv = mDir->getParameters();
  this->insert(sv.begin(), sv.end());
}

//------------------------------------------------------------------------------
void ParamArgHandler::onLinked(Node* grandNode) {
  if (!grandNode->isRoot()) {
    if (grandNode->getArgHandler()->getName() == "path") mPathNode = grandNode;
    if (mPathNode->isLoop())
      PAC_EXCEPT(
          Exception::ERR_INVALID_STATE, "unexcepted loop path before param");
  }
}

//------------------------------------------------------------------------------
PparamArgHandler::PparamArgHandler() : TreeArgHandler("pparam") {
  mRoot->acn("param", "param")->endBranch("0");
  mRoot->acn("path", "path")->acn("param", "param")->endBranch("1");
}

//------------------------------------------------------------------------------
ParamArgHandler* PparamArgHandler::getParamHandler() {
  if (!mMatchedLeaf) PAC_EXCEPT(Exception::ERR_INVALID_STATE, "0 matched leaf");
  Node* node = mMatchedLeaf->getAncestorNode("param");
  return static_cast<ParamArgHandler*>(node->getArgHandler());
}

//------------------------------------------------------------------------------
ValueArgHandler::ValueArgHandler()
    : ArgHandler("value"), mHandler(0), mDir(0) {}

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
IdArgHandler::IdArgHandler() : ArgHandler("id") {}

//------------------------------------------------------------------------------
bool IdArgHandler::doValidate(const std::string& s) {
  static boost::regex regex("[_a-zA-z]\\w*");
  return boost::regex_match(s, regex);
}

//------------------------------------------------------------------------------
void RegexArgHandler::populatePromptBuffer(const std::string& s) {
  appendPromptBuffer("pls input regex expression");
}

//------------------------------------------------------------------------------
ReadonlyArgHandler::ReadonlyArgHandler() : ArgHandler("readonly") {}

//------------------------------------------------------------------------------
void ReadonlyArgHandler::populatePromptBuffer(const std::string& s) {
  appendPromptBuffer("it's readonly, don't edit");
}

//------------------------------------------------------------------------------
QuaternionArgHandler::QuaternionArgHandler() : TreeArgHandler("quaternion") {
  Node* root = getRoot();
  root->acn("real0", "real")
      ->acn("real1", "real")
      ->acn("real2", "real")
      ->acn("real3", "real")
      ->eb("0");
  root->acn("ltl_angleAxis")
      ->acn("real0", "real")
      ->acn("real1", "real")
      ->acn("real2", "real")
      ->acn("real3", "real")
      ->eb("1");
}

//------------------------------------------------------------------------------
  std::string QuaternionArgHandler::getUniformValue() const
{
  static Real pi = std::acos(-1);
  static Real toAngle = pi / 180;
  Real r0 = StringUtil::parsePrimitiveDecimal<Real>(
      this->getMatchedNodeValue("real0"));
  Real r1 = StringUtil::parsePrimitiveDecimal<Real>(
      this->getMatchedNodeValue("real1"));
  Real r2 = StringUtil::parsePrimitiveDecimal<Real>(
      this->getMatchedNodeValue("real2"));
  Real r3 = StringUtil::parsePrimitiveDecimal<Real>(
      this->getMatchedNodeValue("real3"));
  if (getMatchedBranch() == "1") {
    Real halfAngle = r0 * toAngle * 0.5f;
    r0 = std::cos(halfAngle);
    Real sinHalfAngle = std::sin(halfAngle);
    r1 = sinHalfAngle * r1;
    r2 = sinHalfAngle * r2;
    r3 = sinHalfAngle * r3;
  }
  std::stringstream ss;
  ss << r0 << " " << r1 << " " << r2 << " " << r3 << " ";
  return ss.str();
}
}
