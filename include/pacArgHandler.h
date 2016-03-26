#ifndef PACARGHANDLER_H
#define PACARGHANDLER_H

#include "pacSingleton.h"
#include "pacEnumUtil.h"

namespace pac {

typedef std::pair<SVCIter, SVCIter> SVCIterPair;
typedef std::pair<Node*, std::vector<SVCIterPair>> NodeValue;
typedef std::vector<NodeValue> NodeValues;
typedef std::list<Branch> Branches;
typedef std::pair<TreeArgHandler*, Node*> TreeLeafPair;
typedef std::pair<TreeArgHandler*, SVCIter> TreeStartPair;
typedef std::stack<TreeStartPair> TreeStartPairs;
typedef std::vector<TreeLeafPair> TreeLeafPairs;
typedef std::vector<ArgHandler*> ArgHandlerVec;
typedef std::vector<Node*> NodeVector;

/**
 * In order to support tree type handler, it's necessary to recorded every
 * candidate branch and node values in these branches.  All string value is
 * referenced by a StringVector::const_iterator.
 */
struct Branch {
  /**
   * ctor
   * @param f : beginning iter of args
   * @param l : end iter of args
   * @param c : current testing arg iter
   */
  Branch(SVCIter f, SVCIter l, SVCIter c) : first(f), last(l), current(c) {}

  /**
   * Record node value
   * @param node : tested node.
   * @param f : begin of value iter
   * @param l : end of of value iter
   * @return :
   */
  void recordNodeValue(Node* node, SVCIter f, SVCIter l);

  /**
   * Reccord tree and it's matched leaf .
   * @param tree : tree handler
   * @param leaf : leaf node
   */
  void recordTreeLeafPair(TreeArgHandler* tree, Node* leaf);

  /**
   * push new tree into stack, record it's starting arg point
   * @param tree : main or sub tree
   */
  void pushTree(TreeArgHandler* tree);

  /**
   * top tree in stack
   * @return : top tree in stack
   */
  TreeStartPair& topTree();

  /**
   * Meet leaf node, end tree handler. Record tree value , pop tree.
   * @param leaf : leaf node
   */
  void popTree(Node* leaf);

  /**
   * Copy node values from nodeValues to nodes and their underlying arghandler.
   * Handler will set their value to the tested string or strings(tree type) in
   * this branch. Loop node or node with loop type ancestor will also get it's
   * full values in this branch.
   */
  void restoreBranch();

  /**
   * Get last tested node in this branch. It's last element in nodeValues.
   */
  Node* getLastNode();

  SVCIter first, last, current;  // first last current string reference
  NodeValues nodeValues;
  TreeLeafPairs treeLeafPairs;
  TreeStartPairs treeStartPairs;
};

/**
 * Base class of all Argument handler. Arghandler such as int2, real2 only have
 * object prototype, so i choose prototype to create them. Some handler such as
 * "parameter" , "value", "path" need to be initialized at run time, for
 * this handlers most job is done at copy ctor.
 */
class ArgHandler {
public:
  friend class Node;

  enum PromptType {
    PT_PROMPTONLY,        // prompt only , no auto complete
    PT_PROMPTANDCOMPLETE  // prompt and auto complete
  };

  enum ArgHandlerType {
    AHT_PRIMITIVE,  // primitive type, such as int, bool, Real...
    AHT_TREE        // tree type. composites of primitive types. such as int2,
  };

  /**
   * ctor
   * @param name : handler name, will be used if you register it in arg
   * handler lib.
   */
  ArgHandler(const std::string& name);
  ArgHandler(const ArgHandler& rhs);

  virtual ~ArgHandler() {}

  /**
   * prompt, complete for current typing.
   * @param s : current typing word
   */
  virtual void prompt(const std::string& s);

  /**
   * check if s is valid. Set mValue to s if it's valid. Don't override this,
   * over doValidate instead.
   * @param s : test string
   */
  virtual bool validate(const std::string& s);
  virtual ArgHandler* clone() = 0;

  const std::string& getName() const { return mName; }
  void setName(const std::string& v) { mName = v; }

  virtual const std::string& getValue() const { return mValue; }
  virtual void setValue(const std::string& v) { mValue = v; }
  /**
   * sometimes tree arghandler with multiple branch has the same abstract value
   */
  virtual std::string getUniformValue() const { return mValue; }

  /**
   * Some handler need context node to decide it's behavier. This function
   * should  be called at validate, validateBranch, prompt.
   */
  virtual void runtimeInit(){};

  /**
   * Populate prompt buffer, to be used later in applyPromptBuffer.
   * @param s : buffer
   */
  virtual void populatePromptBuffer(const std::string& s) { (void)s; };

  /**
   * Apply prompt buffer, if it's note buffer, output it line by line, if it's
   * complete buffer and autoComplete is true, complete current typing as much
   * as possible, if size of complete buffer is not 1, output them in console
   * format.
   * @param autoComplete : auto complete current typing
   */
  virtual void applyPromptBuffer(
      const std::string& s, bool autoComplete = true);

  /**
   * Test next item in each branches with this handler, remove it if it's
   * invalid or args already consumed. Increment the testing string iterator if
   * it's valid.
   * @param branches : candidate branches
   */
  virtual void validateBranch(
      Branches& branches, ArgHandlerVec& promptHandlers);

  /**
   * output error message to console
   * @param s : test string
   */
  virtual void outputErrMessage(const std::string& s);

  virtual std::string getArgPath() const;

  Node* getTreeNode() const { return mNode; }
  void setTreeNode(Node* v) { mNode = v; }

  StringVector::iterator beginPromptBuffer();
  StringVector::iterator endPromptBuffer();
  size_t getPromptBufferSize();

  PromptType getPromptType() const { return mPromptType; }
  void setPromptType(PromptType v) { mPromptType = v; }

  ArgHandlerType getArgHandlerType() const { return mArgHandlerType; }
  void setArgHandlerType(ArgHandlerType v) { mArgHandlerType = v; }

  /**
   * Deprecated. push arg handlers to prompt current typing.
   * @param{out} ahv : target vector to be pushed into
   */
  void getPromptArgHandlers(ArgHandlerVec& ahv);

protected:
  /**
   * check if s is valid. Derived primitive handler class must override this
   */
  virtual bool doValidate(const std::string& s) {
    (void)s;
    return false;
  }
  /**
   * Append prompt buffer.
   * @param buf : buf
   */
  void appendPromptBuffer(const std::string& buf);

  virtual void completeTyping(const std::string& s);

  /**
   * This will be called when tree node of this handler be added as a child of
   * another node.
   * @grandNode : parent node of tree node of this handler, not tree node,
   * becareful!!!!!!!.
   */
  virtual void onLinked(Node* grandNode) { (void)grandNode; }

protected:
  ArgHandlerType mArgHandlerType;
  PromptType mPromptType;
  Node* mNode;  // only used when this arghandler is inside a node arghandler
  std::string mName;
  std::string mValue;
  StringVector mPromptBuffer;
};

/**
 * Used in TreeHandler, represent a node in tree.  Node handler need an
 * arghandler to populatePromptBuffer and validte.
 */
class Node {
public:
  /**
   * Every treeArgHandler starts with root, plus n normal, loop, ends with n
   * leaves.  Normal and loop node do the real job, root and leaf here is only
   * used to mark start and end. You can specify branch id at leaf node.
   */
  enum NodeType {
    NT_ROOT,
    NT_NORMAL,  // appear only once
    NT_LOOP,    // appear multiple times, you can only loop promitive type
                // handler.
    NT_LEAF
  };

  /**
   * Ctor
   * @param name : node name
   * @param ahName : underlying argument handler name.
   * @param NodeType : node type
   */
  Node(const std::string& name, const std::string& ahName,
      NodeType nt = NT_NORMAL);
  ~Node();

  /**
   * deep copy
   */
  Node(const Node& rhs);

  /**
   * Get depth of this node
   */
  size_t getDepth();

  /**
   * Add node to children.
   * @param child : child node
   * @return : added child node
   */
  Node* addChildNode(Node* child);

  /**
   * Create child node, add it to children.
   * @param name : child node name
   * @param ahName : child node arg handler name, it will be set to child node
   * name if it's empty
   * @param NodeType : child node type
   * @return : added child node
   */
  Node* addChildNode(const std::string& name, const std::string& ahName = "",
      NodeType nt = NT_NORMAL);

  /**
   * abbreviation of addChildNode, to save some space.
   */
  Node* acn(const std::string& name, const std::string& ahName = "",
      NodeType nt = NT_NORMAL) {
    return addChildNode(name, ahName, nt);
  }

  /**
   * Get child node by name.
   * @param name : child node name
   * @param recursive : recursive child of child, set this to false if you just
   * want to search in direct child.
   * @return : child node with specified name
   */
  Node* getChildNode(const std::string& name, bool recursive = true) const;

  /**
   * Get parent node by name. This is always a recursive operation.
   * @param name : parent node name
   * @param maxLvl : max level , 1 for direct parent
   * @return : ancestor node with specified name or 0
   */
  Node* getAncestorNode(const std::string& name, size_t maxLvl = 9999) const;

  Node* getAncestorNode(int lvl);

  Node* getAncestorNodeAhName(int lvl);

  /**
   * Get tree handler attached to this node.
   * @return : tree handler or 0
   */
  TreeArgHandler* getSubTree() const;

  /**
   * Add leaf node, end current branch.
   * @param branchName : leaf node name, also used as branch name.
   */
  Node* endBranch(const std::string& branchName);

  /**
   * abbrivation of endBranch, save some space
   */
  Node* eb(const std::string& branchName) { return endBranch(branchName); }

  Node* getParent() const { return mParent; }
  void setParent(Node* v) { mParent = v; }

  void onLinked();

  bool isRoot() const { return mNodeType == NT_ROOT; }
  bool isNormal() const { return mNodeType == NT_NORMAL; }
  bool isLeaf() const { return mNodeType == NT_LEAF; }
  bool isLoop() const { return mNodeType == NT_LOOP; }

  /**
   * Recursively validate branches, accumulate result branches.
   * leaf:  pop current tree
   * root:  test each child node, accumulate result branches
   * normal: test underlying arg handler 1st, if passed, test each child node,
   *         accumulate result branches.
   * loop: test underlying arghandler , if passed, test self again and each
   * child node , accumulate result branches
   * @param branches{in out} : branch reach leaf of main tree
   * @param promptHandlers{out} : if handler > num args, push it inti
   * promptHandlers, will be used in prompt of main tree
   */
  void validateBranch(Branches& branches, ArgHandlerVec& promptHandlers);

  /**
   * see Branch::restoreValue
   */
  void restoreValue(SVCIter first, SVCIter last);

  const std::string& getValue() const { return mArgHandler->getValue(); }

  /**
   * Recursively descendent leaves
   * @return : node vector of leaves
   */
  void getLeaves(NodeVector& nv);

  /**
   * If it's not attached to a true node, just return it's name, otherwise Join
   * argument handler type until root, it looks like:
   * [tree->ah0->ah1->[subtree->ah0->ah1]->ah2->ah3 ]
   */
  std::string getArgPath() const;

  const std::string& getAhName() const;

  /**
   * Get direct leaf child.
   * @return : direct leaf child or 0
   */
  Node* getLeafChild() const;

  /**
   * get 1st loop type node (including self). This operation will be done
   * accross trees. It will search until meet 1st loop node or main tree.
   * @return : pointer of 1st loop type ancestor or 0
   */
  Node* getLoopNode();

  ArgHandler* getArgHandler() const { return mArgHandler; }
  void setArgHandler(ArgHandler* v) {
    mArgHandler = v;
    mArgHandler->setTreeNode(this);
  }

  NodeType getNodeType() const { return mNodeType; }
  void setNodeType(NodeType v) { mNodeType = v; }

  const std::string& getName() const { return mName; }
  void setName(const std::string& v) { mName = v; }

  TreeArgHandler* getTree() const { return mTree; }
  void setTree(TreeArgHandler* tree, bool recursive = true);

  NodeVector::const_iterator beginChildIter() const;
  NodeVector::const_iterator endChildIter() const;
  size_t getNumChildren() const;

  StringVector::const_iterator beginLoopValueIter() const;
  StringVector::const_iterator endLoopValueIter() const;
  void clearLoopValue();

  /**
   * Get arg handler of next node and current node if it's loop type.
   * @param ahv : arg handler vec
   */
  void getPromptArgHandlers(ArgHandlerVec& ahv);

private:
  NodeType mNodeType;       // node type
  Node* mParent;            // parent node
  ArgHandler* mArgHandler;  // underlying arghandler
  TreeArgHandler* mTree;    // tree pointer
  std::string mName;        // node name in current tree
  std::string mAhName;      // argument handler name
  StringVector mValues;     // for loop node only
  NodeVector mChildren;     // children
};

/**
 * Tree type arg handler. Used to deal with cmd which take multiple formats.
 * Every treeArgHandler starts with root, plus n normal, ends with n leaves.
 * You can get result route when you execute command.
 */
class TreeArgHandler : public ArgHandler {
public:
  friend class ArgHandlerLib;

  virtual ArgHandler* clone() { return new TreeArgHandler(*this); }

  TreeArgHandler(const std::string& name);
  /**
   * deep copy
   */
  TreeArgHandler(const TreeArgHandler& rhs);

  virtual void prompt(const std::string& s);

  virtual bool validate(const std::string& s);

  virtual void getPromptArgHandlers(ArgHandlerVec& ahv);

  /**
   * see Node::validateBranch
   */
  virtual void validateBranch(
      Branches& branches, ArgHandlerVec& promptHandlers);

  virtual void outputErrMessage(const std::string& s);

  virtual std::string getArgPath() const;

  Node* getRoot() { return mRoot; }
  const Node* getRoot() const { return mRoot; }

  /**
   * Get node by name, depth first search.  Throw if not found
   * @param name : child node name
   * @return : Child node pointer
   */
  Node* getNode(const std::string& name);

  /**
   * Get string value catched by specified node.
   * @param name : node name
   * @return : catched value
   */
  const std::string& getMatchedNodeValue(const std::string& name) const;
  /**
   * get matcheed node value if matched branch is in branches
   * @param name : node name
   */
  const std::string& getMatchedNodeValue(const std::string& name,
      std::initializer_list<const char*> branches) const;

  /**
   * get all leaves
   * @return : leaves vector
   */
  void getLeaves(NodeVector& nv);

  /**
   * Get id of matched branch.
   */
  const std::string& getMatchedBranch() const;

  Node* getMatchedLeaf() const { return mMatchedLeaf; }
  void setMatchedLeaf(Node* v) { mMatchedLeaf = v; }

  /**
   * Get node at matched branch
   * @param name : node name
   * @return : node in matched branch with specified name
   */
  Node* getMatchedNode(const std::string& name) const;

  bool hasMatchedNode(const std::string& name) const;

  /**
   * Get arg handler of matched node
   * @param nodeName : node name
   * @return : arg handler of matched node
   */
  ArgHandler* getMatchedNodeHandler(const std::string& nodeName) const;

  // cast return type
  template <typename T>
  T* getMatchedNodeHandler(const std::string& nodeName) const {
    return static_cast<T*>(getMatchedNodeHandler(nodeName));
  }

  /**
   * Get sub tree under specified node
   * @param nodeName : node name, not sub tree name!.
   * @return :
   */
  TreeArgHandler* getSubTree(const std::string& nodeName) const;

private:
  /**
   * make sure every branch ends with a leaf
   */
  void checkWholeness();
  void checkWholeness(Node* n);

protected:
  Node* mRoot;
  Node* mMatchedLeaf;
};

/**
 * Registered with following type:
 *
 * primitive:
 *  blank
 *  bool
 *  short
 *  ushort
 *  int
 *  uint
 *  long
 *  ulong
 *  real
 *  nreal
 *  npreal
 *  int2
 *  int3
 *  int4
 *  int5
 *  real2
 *  real3
 *  real4
 *  real5
 *  nreal2
 *  nreal3
 *  nreal4
 *  nreal5
 *  matrix2
 *  matrix3
 *  matrix4
 *  nmatrix2
 *  nmatrix3
 *  nmatrix4
 *  quaternion
 * special:
 *  regex
 *  readonly
 *  path
 *  cmd
 *  param
 *  value
 * literal:
 *  ltl_regex
 *  ltl_angleAxis
 */
class ArgHandlerLib : public Singleton<ArgHandlerLib> {
public:
  typedef std::map<std::string, ArgHandler*> ArgHandlerMap;

  ~ArgHandlerLib();
  /**
   * Regsiter new arument handler
   * @param handler : Argument handler
   * @return :
   */
  void registerArgHandler(ArgHandler* proto);

  /**
   * register proto->getName() and "re_"+proto->getName()
   * @param proto : prototype
   */
  void registerStringAH(StringArgHandler* proto);

  /**
   * register intrinsic arg handler
   */
  void init();

  /**
   * Create arg handler.
   * @param protoName : Previously registered prototype name
   * @return : newly created arg handler
   */
  ArgHandler* createArgHandler(const std::string& protoName);

  /**
   * Create 1 branch only tree with sequence of the 1 type argument
   * handler. This can be used to create int2 int3, real2, real3.......  Child
   * node will be named as name_0, name_1.... name_n
   * @param name : tree name
   * @param ahName : item argument handler name
   * @param num : number of items
   * @return : newly created tree argument handler
   */
  TreeArgHandler* createMonoTree(
      const std::string& name, const std::string& ahName, int num);

  /**
   * check if handler exists
   * @param name : handler name
   * @return : true if exists
   */
  bool exists(const std::string& name);

private:
  ArgHandlerMap mArgHandlerMap;
};
}

#endif /* PACARGHANDLER_H */
