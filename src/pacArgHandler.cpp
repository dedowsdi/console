#include "pacStable.h"
#include "pacArgHandler.h"
#include "pacIntrinsicArgHandler.h"
#include "pacStringUtil.h"
#include "pacStdUtil.h"
#include "pacConsole.h"

namespace pac {

//------------------------------------------------------------------------------
void Branch::recordNodeValue(Node* node, SVCIter f, SVCIter l) {
  PacAssert(node, "0 node");
  if (node->isLeaf() || node->isRoot())
    PAC_EXCEPT(Exception::ERR_INVALID_STATE,
        "you can not add node value to root or leaf.");

  NodeValues::iterator iter =
      std::find_if(this->nodeValues.begin(), this->nodeValues.end(),
          [&](NodeValues::value_type& v) -> bool { return v.first == node; });
  sgLogger.logMessage(
      "record node value " + node->getName() + " : " + StringUtil::join(f, l),
      SL_TRIVIAL);

  if (iter == this->nodeValues.end()) {
    this->nodeValues.push_back(
        std::make_pair(node, std::vector<SVCIterPair>()));
    this->nodeValues.rbegin()->second.push_back(std::make_pair(f, l));
  } else {
    if (!node->getLoopNode())
      PAC_EXCEPT(Exception::ERR_INVALID_STATE,
          "you can not record node value twice for the same normal node which "
          "has no loop type ancestor node");
    iter->second.push_back(std::make_pair(f, l));
  }
}

//------------------------------------------------------------------------------
void Branch::recordTreeLeafPair(TreeArgHandler* tree, Node* leaf) {
  sgLogger.logMessage("recored subtree " + tree->getName() +
                          " with matched branch " + leaf->getName(),
      SL_TRIVIAL);
  treeLeafPairs.push_back(std::make_pair(tree, leaf));
}

//------------------------------------------------------------------------------
void Branch::pushTree(TreeArgHandler* tree) {
  treeStartPairs.push(std::make_pair(tree, current));
}

//------------------------------------------------------------------------------
TreeStartPair& Branch::topTree() { return treeStartPairs.top(); }

//------------------------------------------------------------------------------
void Branch::popTree(Node* leaf) {
  TreeStartPair& tlp = topTree();
  TreeArgHandler* tree = tlp.first;
  if (tree != leaf->getTree())
    PAC_EXCEPT(Exception::ERR_INVALID_STATE,
        "tree of leaf(" + leaf->getTree()->getName() +
            ") is different from top tree:" + tree->getName());

  Node* treeNode = tree->getTreeNode();
  if (treeNode) {
    // record it's value and matched leaf if it's a sub tree
    this->recordNodeValue(treeNode, tlp.second, this->current);
    this->recordTreeLeafPair(tree, leaf);
  } else {
    // main tree, main tree has no parent node, and it's value is unique
    sgLogger.logMessage("Set main branch " + leaf->getName(), SL_TRIVIAL);
    tlp.first->setValue(StringUtil::join(tlp.second, this->current));
    tlp.first->setMatchedLeaf(leaf);
  }

  treeStartPairs.pop();
}

//------------------------------------------------------------------------------
void Branch::restoreBranch() {
  Node* node = getLastNode();
  if (node) {
    if (!node->getLeafChild())
      PAC_EXCEPT(Exception::ERR_INVALID_STATE,
          "illegal state, found no leaf child, invalid branch");
  }

  PacAssert(treeStartPairs.size() == 0, "there should no trees left in stack");

  // restore node values
  std::for_each(nodeValues.begin(), nodeValues.end(),
      [&](NodeValues::value_type& v) -> void {
        // clear loop value first
        v.first->clearLoopValue();
        std::for_each(v.second.begin(), v.second.end(),
            [&](SVCIterPair& iterPair) -> void {
              v.first->restoreValue(iterPair.first, iterPair.second);
            });
      });

  // restore tree leaf
  std::for_each(
      treeLeafPairs.begin(), treeLeafPairs.end(), [&](TreeLeafPair& v) -> void {
        sgLogger.logMessage("set tree:" + v.first->getName() + " matched leaf" +
                                v.second->getName(),
            SL_TRIVIAL);
        v.first->setMatchedLeaf(v.second);
      });
}

//------------------------------------------------------------------------------
Node* Branch::getLastNode() {
  // PacAssert(!nodeValues.empty(), "found no recorded node values");
  if (nodeValues.empty()) return 0;
  return std::get<0>(*nodeValues.rbegin());
}

//------------------------------------------------------------------------------
ArgHandler::ArgHandler(const std::string& name)
    : mArgHandlerType(AHT_PRIMITIVE),
      mPromptType(PT_PROMPTANDCOMPLETE),
      mNode(0),
      mName(name) {}

//------------------------------------------------------------------------------
ArgHandler::ArgHandler(const ArgHandler& rhs)
    : mArgHandlerType(rhs.mArgHandlerType),
      mPromptType(rhs.mPromptType),
      mNode(rhs.mNode),
      mName(rhs.mName) {
  // set tree node to 0
  mNode = 0;
}

//------------------------------------------------------------------------------
void ArgHandler::prompt(const std::string& s) {
  this->runtimeInit();
  this->populatePromptBuffer(s);
  this->applyPromptBuffer(s);
}

//------------------------------------------------------------------------------
bool ArgHandler::validate(const std::string& s) {
  this->runtimeInit();
  bool res = this->doValidate(s);
  if (res) this->setValue(s);
  return res;
}

//------------------------------------------------------------------------------
StringVector::iterator ArgHandler::beginPromptBuffer() {
  return mPromptBuffer.begin();
}

//------------------------------------------------------------------------------
StringVector::iterator ArgHandler::endPromptBuffer() {
  return mPromptBuffer.end();
}

//------------------------------------------------------------------------------
size_t ArgHandler::getPromptBufferSize() { return mPromptBuffer.size(); }

//------------------------------------------------------------------------------
void ArgHandler::getPromptArgHandlers(ArgHandlerVec& ahv) {
  ahv.push_back(this);
}

//------------------------------------------------------------------------------
void ArgHandler::validateBranch(
    Branches& branches, ArgHandlerVec& promptHandlers) {
  PacAssert(!branches.empty(), "empty branch");
  Branches::iterator iter =
      std::remove_if(branches.begin(), branches.end(), [&](Branch& v) -> bool {

        if (v.current == v.last) promptHandlers.push_back(this);

        if (v.current == v.last || !validate(*v.current))
          return true;
        else {
          v.recordNodeValue(this->getTreeNode(), v.current, v.current + 1);
          ++v.current;
          return false;
        }
      });

  branches.erase(iter, branches.end());
}

//------------------------------------------------------------------------------
void ArgHandler::applyPromptBuffer(const std::string& s, bool autoComplete) {
  if (mPromptBuffer.empty()) return;

  if (mPromptType == PT_PROMPTONLY) {
    std::for_each(mPromptBuffer.begin(), mPromptBuffer.end(),
        [&](const std::string& v) -> void { sgConsole.outputLine(v); });
  } else {
    if (mPromptBuffer.size() > 1 || !autoComplete) {
      RaiiConsoleBuffer raii;
      std::for_each(mPromptBuffer.begin(), mPromptBuffer.end(),
          [&](const std::string& v) -> void { sgConsole.output(v); });
    }
    if (autoComplete) this->completeTyping(s);
  }
}

//------------------------------------------------------------------------------
void ArgHandler::outputErrMessage(const std::string& s) {
  sgConsole.outputLine(s + " is not a valid " + getName());
}

//------------------------------------------------------------------------------
void ArgHandler::appendPromptBuffer(const std::string& buf) {
  mPromptBuffer.push_back(buf);
}

//------------------------------------------------------------------------------
void ArgHandler::completeTyping(const std::string& s) {
  const std::string&& iden =
      StdUtil::getIdenticalString(mPromptBuffer.begin(), mPromptBuffer.end());
  // just check again
  if (!s.empty() && !StringUtil::startsWith(iden, s))
    PAC_EXCEPT(Exception::ERR_INVALID_STATE,
        "complete string don't starts with typing!!!!");

  sgConsole.complete(iden.substr(s.size()));
}

//------------------------------------------------------------------------------
Node::Node(const std::string& name, const std::string& ahName, NodeType nt)
    : mNodeType(nt), mParent(0), mArgHandler(0), mTree(0), mName(name) {
  if (!isRoot() && !isLeaf()) {
    mArgHandler = sgArgLib.createArgHandler(ahName);
    PacAssert(mArgHandler, "0 arg handler");
    mArgHandler->setTreeNode(this);
  } else {
    if (!mAhName.empty())
      PAC_EXCEPT(Exception::ERR_INVALID_STATE, "not empty ahName");
  }
}

//------------------------------------------------------------------------------
Node::~Node() {
  std::for_each(
      mChildren.begin(), mChildren.end(), [&](Node* v) -> void { delete v; });
  mChildren.clear();
  if (mArgHandler) delete mArgHandler;
  mArgHandler = 0;
}

//------------------------------------------------------------------------------
Node::Node(const Node& rhs)
    : mNodeType(rhs.mNodeType),
      mParent(0),
      mArgHandler(0),
      mTree(0),
      mName(rhs.getName()) {
  if (!isRoot() && !isLeaf()) {
    mArgHandler = rhs.getArgHandler()->clone();
    mArgHandler->setTreeNode(this);
  }
  // deep copy children, take care of loop type.
  std::for_each(rhs.beginChildIter(), rhs.endChildIter(),
      [&](Node* v) -> void { this->addChildNode(new Node(*v)); });
}

//------------------------------------------------------------------------------
size_t Node::getDepth() {
  size_t d = 0;
  Node* node = this;
  while ((node = node->getParent())) ++d;

  return d;
}

//------------------------------------------------------------------------------
Node* Node::addChildNode(Node* child) {
  mChildren.push_back(child);
  if (child != this)  // loop type can add it self to child
  {
    child->setParent(this);
    child->setTree(mTree);
  }
  return child;
}

//------------------------------------------------------------------------------
Node* Node::addChildNode(const std::string& name,
    const std::string& ahName /*= ""*/, NodeType nt /*= NT_NORMAL*/) {
  Node* node = new Node(name, ahName.empty() ? name : ahName, nt);
  this->addChildNode(node);
  if (nt != NT_LEAF) node->getArgHandler()->onLinked();

  return node;
}

//------------------------------------------------------------------------------
Node* Node::getChildNode(const std::string& name, bool recursive /*= true*/) {
  for (NodeVector::iterator iter = mChildren.begin(); iter != mChildren.end();
       ++iter) {
    if (isLeaf()) continue;
    if ((*iter)->getName() == name) return *iter;
    if (recursive) {
      Node* node = (*iter)->getChildNode(name, recursive);
      if (node) return node;
    }
  }
  return 0;
}

//------------------------------------------------------------------------------
Node* Node::getAncestorNode(const std::string& name) {
  if (!mParent) return 0;

  if (mParent->getName() == name)
    return mParent;
  else
    return mParent->getAncestorNode(name);
}

//------------------------------------------------------------------------------
TreeArgHandler* Node::getSubTree() {
  if (isLeaf() || isRoot()) return 0;
  if (mArgHandler->getArgHandlerType() == ArgHandler::AHT_TREE)
    return static_cast<TreeArgHandler*>(mArgHandler);
  return 0;
}

//------------------------------------------------------------------------------
Node* Node::endBranch(const std::string& branchName) {
  Node* tail = new Node(branchName, "", Node::NT_LEAF);
  return this->addChildNode(tail);
}

//------------------------------------------------------------------------------
void Node::validateBranch(Branches& branches, ArgHandlerVec& promptHandlers) {
  PacAssert(!branches.empty(), "empty branch");
  sgLogger.logMessage("node " + this->mName + " validateBranch with " +
                          StringUtil::toString(branches.size()) + " branches",
      SL_TRIVIAL);

  if (isLeaf()) {
    // recored tree values
    sgLogger.logMessage("meet leaf of " + this->getTree()->getName() +
                            " with " + StringUtil::toString(branches.size()) +
                            " branches",
        SL_TRIVIAL);
    std::for_each(branches.begin(), branches.end(),
        [&](Branch& branch) -> void { branch.popTree(this); });
    return;
  }

  if (isRoot()) {
    // record tree start point
    std::for_each(branches.begin(), branches.end(),
        [&](Branch& v) -> void { v.pushTree(getTree()); });
  } else {
    // test current arghandler
    PacAssert(mArgHandler, "0 arghandler");
    if (mArgHandler->getTreeNode() != this)
      PAC_EXCEPT(Exception::ERR_INVALID_STATE, "wrong handler!!");

    this->mArgHandler->validateBranch(branches, promptHandlers);
    if (branches.empty()) return;
  }
  Branches origBranch(branches);
  branches.clear();

  // add branches of loop node
  if (isLoop()) {
    // recursive self. Be careful here, loop real3 validate matrix3 will
    // result in 3 branches(real3, real3 real3, real3 real3 real3).
    Branches tmp(origBranch);
    this->validateBranch(tmp, promptHandlers);
    branches.splice(branches.end(), tmp);
  }

  // add branches of child node
  std::for_each(mChildren.begin(), mChildren.end(), [&](Node* node) -> void {
    Branches tmp(origBranch);
    node->validateBranch(tmp, promptHandlers);
    branches.splice(branches.end(), tmp);
  });
}

//------------------------------------------------------------------------------
void Node::restoreValue(SVCIter first, SVCIter last) {
  if (last == first)
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "invalid 0 values for loop node");

  if (isLeaf() || isRoot())
    PAC_EXCEPT(
        Exception::ERR_INVALID_STATE, "you can not set value for root or leaf");

  const std::string&& value = StringUtil::join(first, last);
  if (mArgHandler->getArgHandlerType() == ArgHandler::AHT_TREE) {
    mArgHandler->setValue(value);
  } else {
    // for primitive type, set it's value to last valid value
    mArgHandler->setValue(*(last - 1));
  }

  if (this->getLoopNode())
    // recored value under loop type node
    mValues.push_back(value);
}

//------------------------------------------------------------------------------
NodeVector Node::getLeaves() {
  NodeVector nv;
  if (isLeaf()) {
    nv.push_back(this);
    return nv;
  }

  std::for_each(mChildren.begin(), mChildren.end(), [&](Node* v) -> void {
    NodeVector&& childLeaves = v->getLeaves();
    nv.insert(nv.end(), childLeaves.begin(), childLeaves.end());
  });
  return nv;
}

//------------------------------------------------------------------------------
std::string Node::getArgPath() {
  // PacAssertS(isLeaf(), "You can not call getArgPath with type :" +
  // StringUtil::toString(static_cast<int>(mNodeType)));

  // get reverse path
  std::stringstream ss;
  ss << "[";
  Node* node = this;
  while (node) {
    if (node->isRoot()) {
      // output tree name
      std::string treeName = node->getTree()->getName();
      std::reverse(treeName.begin(), treeName.end());
      ss << "]" << treeName;
      // output [ of tree node if it exists
      node = node->getTree()->getTreeNode();
      if (node) ss << "[";
    } else {
      std::string nodeName = node->getName();
      std::reverse(nodeName.begin(), nodeName.end());
      ss << nodeName << "<-";
      node = node->getParent();
    }
  }
  // reverse again
  std::string&& path = ss.str();
  std::reverse(path.begin(), path.end());
  return path;
}

//------------------------------------------------------------------------------
ArgHandler* Node::getArgHandler() const {
  PacAssert(mArgHandler, "0 arghandler");
  return mArgHandler;
}

//------------------------------------------------------------------------------
const std::string& Node::getAhName() const {
  if (!mArgHandler) PAC_EXCEPT(Exception::ERR_INVALID_STATE, "0 arghandler");
  return mArgHandler->getName();
}

//------------------------------------------------------------------------------
Node* Node::getLeafChild() const {
  NodeVector::const_iterator iter =
      std::find_if(mChildren.begin(), mChildren.end(),
          [&](const NodeVector::value_type& v) -> bool { return v->isLeaf(); });

  return iter != mChildren.end() ? *iter : 0;
}

//------------------------------------------------------------------------------
Node* Node::getLoopNode() {
  if (isLoop()) return this;
  Node* treeNode = this->getTree()->getTreeNode();
  if (!treeNode) return 0;
  return treeNode->getLoopNode();
}

//------------------------------------------------------------------------------
void Node::setTree(TreeArgHandler* tree, bool recursive /*= true*/) {
  mTree = tree;
  if (recursive) {
    std::for_each(mChildren.begin(), mChildren.end(),
        [&](Node* v) -> void { v->setTree(tree, recursive); });
  }
}

//------------------------------------------------------------------------------
NodeVector::const_iterator Node::beginChildIter() const {
  return mChildren.begin();
}

//------------------------------------------------------------------------------
NodeVector::const_iterator Node::endChildIter() const {
  return mChildren.end();
}

//------------------------------------------------------------------------------
size_t Node::getNumChildren() const { return mChildren.size(); }

//------------------------------------------------------------------------------
StringVector::const_iterator Node::beginLoopValueIter() const {
  return mValues.begin();
}

//------------------------------------------------------------------------------
StringVector::const_iterator Node::endLoopValueIter() const {
  return mValues.end();
}

//------------------------------------------------------------------------------
void Node::clearLoopValue() { mValues.clear(); }

//------------------------------------------------------------------------------
void Node::getPromptArgHandlers(ArgHandlerVec& ahv) {
  if (isLeaf()) return;

  std::for_each(mChildren.begin(), mChildren.end(), [&](Node* v) -> void {
    if (!v->isLeaf()) v->getArgHandler()->getPromptArgHandlers(ahv);
  });
  if (isLoop()) mArgHandler->getPromptArgHandlers(ahv);
}

//------------------------------------------------------------------------------
TreeArgHandler::TreeArgHandler(const std::string& name)
    : ArgHandler(name), mMatchedLeaf(0) {
  mRoot = new Node(name + "_root", "", Node::NT_ROOT);
  mRoot->setTree(this);
  setArgHandlerType(AHT_TREE);
}

//------------------------------------------------------------------------------
TreeArgHandler::TreeArgHandler(const TreeArgHandler& rhs)
    : ArgHandler(rhs), mMatchedLeaf(0) {
  mRoot = new Node(*rhs.getRoot());
  mRoot->setTree(this);
  // make sure no duplicate branch exists
  NodeVector&& leaves = mRoot->getLeaves();
  std::sort(leaves.begin(), leaves.end());
  NodeVector::iterator iter = std::unique(leaves.begin(), leaves.end());
  if (iter != leaves.end())
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "duplicate tree branches");
}

//------------------------------------------------------------------------------
void TreeArgHandler::prompt(const std::string& s) {
  this->runtimeInit();
  // split by space
  StringVector sv;
  if (!s.empty()) sv = StringUtil::split(s);

  if (s.empty() || s[s.size() - 1] == ' ') sv.push_back("");

  Branches branches;
  ArgHandlerVec ahv;

  branches.push_back(Branch(sv.begin(), sv.end() - 1, sv.begin()));
  this->validateBranch(branches, ahv);

  // mutiple candidates
  ArgHandlerVec candidates;
  std::for_each(ahv.begin(), ahv.end(), [&](ArgHandler* handler) -> void {
    handler->runtimeInit();
    handler->populatePromptBuffer(*sv.rbegin());
    if (handler->getPromptBufferSize() > 0) {
      candidates.push_back(handler);
    }
  });

  if (candidates.size() == 1)
    candidates[0]->applyPromptBuffer(*sv.rbegin(), true);
  else if (candidates.size() > 1) {
    std::for_each(
        candidates.begin(), candidates.end(), [&](ArgHandler* v) -> void {
          // output head
          sgConsole.outputLine(v->getTreeNode()->getArgPath() + ":");
          v->applyPromptBuffer(*sv.rbegin(), false);
        });
  }
}

//------------------------------------------------------------------------------
bool TreeArgHandler::validate(const std::string& s) {
  sgLogger.logMessage("Tree " + mName + " start validate " + s, SL_TRIVIAL);
  this->checkWholeness();
  this->setMatchedLeaf(0);

  this->runtimeInit();
  // split by space
  StringVector sv;
  if (!s.empty()) sv = StringUtil::split(s);

  Branches branches;
  ArgHandlerVec ahv;
  branches.push_back(Branch(sv.begin(), sv.end(), sv.begin()));
  this->validateBranch(branches, ahv);

  // filter result, arg must be totally consumed and last node must belongs to
  // main tree and has leaf child.
  Branches::iterator iter = std::remove_if(
      branches.begin(), branches.end(), [&](Branches::value_type& v) -> bool {
        if (v.current != v.last) return true;
        Node* n = v.getLastNode();
        if (n && (n->getTree() != this || !n->getLeafChild())) return true;
        return false;
      });
  branches.erase(iter, branches.end());

  sgLogger.logMessage(
      "found " + StringUtil::toString(branches.size()) + " branches",
      SL_TRIVIAL);

  // make sure only 1 matched branch exists
  if (branches.size() > 1) {
    if (sv.empty())
      PAC_EXCEPT(
          Exception::ERR_INVALID_STATE, "empty value found multiple branch!");

    sgConsole.outputLine("found multiple valid branches:");
    std::for_each(branches.begin(), branches.end(), [&](Branch& arg) -> void {
      // if multiple branches exists, at least 1 node value is recorded
      sgConsole.outputLine(arg.getLastNode()->getLeafChild()->getArgPath());
    });
    return false;
  } else if (branches.size() == 0) {
    return false;
  } else {
    branches.begin()->restoreBranch();
    return true;
  }
}

//------------------------------------------------------------------------------
void TreeArgHandler::getPromptArgHandlers(ArgHandlerVec& ahv) {
  return getRoot()->getPromptArgHandlers(ahv);
}

//------------------------------------------------------------------------------
Node* TreeArgHandler::getNode(const std::string& name) {
  Node* node = mRoot->getChildNode(name, true);
  if (node == 0)
    PAC_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
        name + " not found in tree handler" + getName());
  return node;
}

//------------------------------------------------------------------------------
const std::string& TreeArgHandler::getMatchedNodeValue(
    const std::string& name) {
  return getMatchedNode(name)->getValue();
}

//------------------------------------------------------------------------------
const std::string& TreeArgHandler::getMatchedNodeValue(
    const std::string& name, std::initializer_list<const char*> branches) {
  static std::string blank;
  auto iter = std::find(branches.begin(), branches.end(), name);
  return iter == branches.end() ? blank : getMatchedNodeValue("name");
}

//------------------------------------------------------------------------------
NodeVector TreeArgHandler::getLeaves() { return mRoot->getLeaves(); }

//------------------------------------------------------------------------------
const std::string& TreeArgHandler::getMatchedBranch() {
  PacAssertS(mMatchedLeaf, "Havn't found matched leaf for " + getName());
  return mMatchedLeaf->getName();
}

//------------------------------------------------------------------------------
Node* TreeArgHandler::getMatchedNode(const std::string& name) {
  return getMatchedLeaf()->getAncestorNode(name);
}

//------------------------------------------------------------------------------
ArgHandler* TreeArgHandler::getMatchedNodeHandler(const std::string& nodeName) {
  return getMatchedNode(nodeName)->getArgHandler();
}

//------------------------------------------------------------------------------
TreeArgHandler* TreeArgHandler::getSubTree(const std::string& nodeName) {
  Node* node = this->getRoot()->getChildNode(nodeName);
  if (node->isLeaf() || node->isRoot() ||
      node->getArgHandler()->getArgHandlerType() != ArgHandler::AHT_TREE)
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "no tree under " + nodeName);

  return static_cast<TreeArgHandler*>(node->getArgHandler());
}

//------------------------------------------------------------------------------
void TreeArgHandler::validateBranch(
    Branches& branches, ArgHandlerVec& promptHandlers) {
  this->checkWholeness();
  this->runtimeInit();
  getRoot()->validateBranch(branches, promptHandlers);
}

//------------------------------------------------------------------------------
void TreeArgHandler::outputErrMessage(const std::string& s) {
  sgConsole.outputLine(
      s + " is not a valid " + getName() + " which takes following formats:");
  NodeVector&& nv = getLeaves();
  std::for_each(nv.begin(), nv.end(),
      [&](Node* v) -> void { sgConsole.outputLine(v->getArgPath()); });
}

//------------------------------------------------------------------------------
void TreeArgHandler::checkWholeness() { pac::checkWholeness(getRoot()); }

//------------------------------------------------------------------------------
ArgHandlerLib::~ArgHandlerLib() {
  // release prototypes
  std::for_each(mArgHandlerMap.begin(), mArgHandlerMap.end(),
      [&](ArgHandlerMap::value_type& v) -> void { delete v.second; });
  mArgHandlerMap.clear();
}

//------------------------------------------------------------------------------
void ArgHandlerLib::registerArgHandler(ArgHandler* handler) {
  PacAssert(!handler->getName().empty(), "empty handler name");
  sgLogger.logMessage("register handler " + handler->getName(), SL_NORMAL);
  // check if it's already registerd
  ArgHandlerMap::iterator iter = std::find_if(mArgHandlerMap.begin(),
      mArgHandlerMap.end(), [&](ArgHandlerMap::value_type& v) -> bool {
        return v.first == handler->getName();
      });

  if (iter == mArgHandlerMap.end())
    mArgHandlerMap[handler->getName()] = handler;
  else
    PAC_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
        handler->getName() + " already registed!");
}

//------------------------------------------------------------------------------
void ArgHandlerLib::init() {
  this->registerArgHandler(new BlankArgHandler());
  // bool
  StringArgHandler* boolHandler = new StringArgHandler("bool");
  boolHandler->insert("true");
  boolHandler->insert("false");
  this->registerArgHandler(boolHandler);
  // primitive decimal arg handlers
  this->registerArgHandler(new PriDeciArgHandler<short>("short"));
  this->registerArgHandler(new PriDeciArgHandler<unsigned short>("ushort"));
  this->registerArgHandler(new PriDeciArgHandler<int>("int"));
  this->registerArgHandler(new PriDeciArgHandler<unsigned int>("uint"));
  this->registerArgHandler(new PriDeciArgHandler<long>("long"));
  this->registerArgHandler(new PriDeciArgHandler<unsigned long>("ulong"));
  this->registerArgHandler(new PriDeciArgHandler<Real>("real"));
  // normalized real
  this->registerArgHandler(
      new PriDeciRangeArgHandler<Real>("nreal", -1.0, 1.0));
  // normalized positive real
  this->registerArgHandler(new PriDeciRangeArgHandler<Real>("npreal", 0, 1.0));
  // int 2..int5, real2..real5, matrix2, matrix3, matrix4
  this->registerArgHandler(sgArgLib.createMonoTree("int2", "int", 2));
  this->registerArgHandler(sgArgLib.createMonoTree("int3", "int", 3));
  this->registerArgHandler(sgArgLib.createMonoTree("int4", "int", 4));
  this->registerArgHandler(sgArgLib.createMonoTree("int5", "int", 5));
  this->registerArgHandler(sgArgLib.createMonoTree("real2", "real", 2));
  this->registerArgHandler(sgArgLib.createMonoTree("real3", "real", 3));
  this->registerArgHandler(sgArgLib.createMonoTree("real4", "real", 4));
  this->registerArgHandler(sgArgLib.createMonoTree("real5", "real", 5));
  this->registerArgHandler(sgArgLib.createMonoTree("nreal2", "nreal", 2));
  this->registerArgHandler(sgArgLib.createMonoTree("nreal3", "nreal", 3));
  this->registerArgHandler(sgArgLib.createMonoTree("nreal4", "nreal", 4));
  this->registerArgHandler(sgArgLib.createMonoTree("nreal5", "nreal", 5));
  this->registerArgHandler(sgArgLib.createMonoTree("matrix2", "real", 4));
  this->registerArgHandler(sgArgLib.createMonoTree("matrix3", "real", 9));
  this->registerArgHandler(sgArgLib.createMonoTree("matrix4", "real", 16));
  this->registerArgHandler(sgArgLib.createMonoTree("nmatrix2", "nreal", 4));
  this->registerArgHandler(sgArgLib.createMonoTree("nmatrix3", "nreal", 9));
  this->registerArgHandler(sgArgLib.createMonoTree("nmatrix4", "nreal", 16));
  this->registerArgHandler(new IdArgHandler());
  this->registerArgHandler(new RegexArgHandler());

  // special handlers
  this->registerArgHandler(new PathArgHandler());
  // arg lib is inited before command lib, moved to CommandLib::init()
  // this->registerArgHandler(new CmdArgHandler());
  this->registerArgHandler(new ParamArgHandler());
  this->registerArgHandler(new ValueArgHandler());

  // iteral
  this->registerArgHandler(new LiteralArgHandler("regex"));
  this->registerArgHandler(new LiteralArgHandler("angleAxis"));
}

//------------------------------------------------------------------------------
ArgHandler* ArgHandlerLib::createArgHandler(const std::string& protoName) {
  ArgHandlerMap::iterator iter = mArgHandlerMap.find(protoName);
  if (iter == mArgHandlerMap.end())
    PAC_EXCEPT(
        Exception::ERR_ITEM_NOT_FOUND, protoName + " not found in arg libs");
  return iter->second->clone();
}

//------------------------------------------------------------------------------
TreeArgHandler* ArgHandlerLib::createMonoTree(
    const std::string& name, const std::string& ahName, int num) {
  TreeArgHandler* tree = new TreeArgHandler(name);

  Node* node = tree->getRoot();
  for (int i = 0; i < num; ++i) {
    node = node->addChildNode(name + "_" + StringUtil::toString(i), ahName);
  }
  node->endBranch("branch0");
  return tree;
}

//------------------------------------------------------------------------------
bool ArgHandlerLib::exists(const std::string& name) {
  return mArgHandlerMap.find(name) != mArgHandlerMap.end();
}

void checkWholeness(Node* n) {
  if (n->isLeaf()) return;

  if (n->getNumChildren() == 0)
    PAC_EXCEPT(Exception::ERR_INVALID_STATE, "do you  forget to endbranch for" +
                                                 n->getTree()->getName() +
                                                 " at " + n->getName());
  std::for_each(n->beginChildIter(), n->endChildIter(),
      [&](Node* v) -> void { checkWholeness(v); });
}

template <>
ArgHandlerLib* Singleton<ArgHandlerLib>::msSingleton = 0;
}
