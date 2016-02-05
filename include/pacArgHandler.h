#ifndef PACARGHANDLER_H
#define PACARGHANDLER_H 

#include "pacSingleton.h"

namespace pac
{

#define defArgCom(type)\
virtual ArgHandler* clone(){return new type(*this);}
/**
 * Base class of all Argument handler. Arghandler such as int2, real2 don't have
 * real class prototype, what they have is object prototype, so i choose
 * prototype to create them. Some handler such as "parameter" , "value", "path",
 * they need to be initialized at run time, for this handlers most job is done
 * at copy ctor.
 */
class ArgHandler 
{
public:
	
	ArgHandler(const std::string& name);
	virtual ~ArgHandler(){}

	/**
	 * complete, prompt for current typing.
	 */
	void prompt(const std::string& s);
	/**
	 * check if s is valid. Set mValue to s if it's valid. Don't override this,
	 * over doValidate instead.
	 */
	bool validate(const std::string& s);

	/**
	 * Populate prompt buffer, to be used later in applyPromptBuffer. There are
	 * two kinds of buffer: note buffer and complete buffer. See
	 * appendNoteBuffer and appendCompleteBuffer.
	 * @remark : 
	 * @param s : 
	 * @return : 
	 */
	virtual void populatePromptBuffer(const std::string& s){};
	/**
	 * Apply prompt buffer, if it's note buffer, output it line by line, if it's
	 * complete buffer and autoComplete is true, complete current typing as much as possible, of size of
	 * complete buffer is not 1, output then in console format.
	 * @param autoComplete : auto complete current typing
	 */
	virtual void applyPromptBuffer(const std::string& s, bool autoComplete = true); 

	/**
	 * check if s is valid. 
	 */
	virtual bool doValidate(const std::string& s) = 0;
	virtual void outputErrMessage(const std::string& s);

	/**
	 * This name will be used at ArgHanderLib to reference this handler.
	 */
	const std::string& getName() const { return mName; }
	void setName( const std::string& v){mName = v;}

	virtual const std::string& getValue() const { return mValue; }
	virtual void setValue( const std::string& v){mValue = v;}

	virtual ArgHandler* clone() = 0;

	Node* getNode() const { return mNode; }
	void setNode( Node* v){mNode = v;}

	StringVector::iterator beginPromptBuffer();
	StringVector::iterator endPromptBuffer();
	size_t getPromptBufferSize();
	
protected:

	/**
	 * Append note buffer. This kind buffer will be output line by line. Don't
	 * add line break in buf. Don't mix it with complete buffer
	 * @param buf : buf 
	 */
	void appendNoteBuffer(const std::string& buf);
	/**
	 * Append complete buffer. This kind buffer will be used to complete current
	 * typing. Don't mix it with note buffer.
	 * @param buf : buf
	 */
	void appendCompleteBuffer(const std::string& buf);

	/**
	 * Check if buf is note buffer, note buf should start with @@.
	 * @return : true or false
	 */
	bool isNoteBuffer(const std::string& buf);
	/**
	 * Check if buf is complete buffer, complete buf should not start with @@
	 * @return : true or false
	 */
	bool isCompleteBuf(const std::string& buf);

private:
	std::string mName;
	std::string mValue;
	Node* mNode; //only used when this arghandler is inside a node arghandler
	StringVector mPromptBuffer;

};

typedef std::vector<Node*> NodeVector;
/**
 * Used in TreeHandler, represent a node in tree. Normal node handler need
 * another arghandler to do the populatePromptBuffer and validte.
 */
class Node
{
public:

	/**
	 * Every treeArgHandler starts with root, plus n normal, loop, ends with n
	 * leaves.  Normal and loop node do the real job, root and leaf here is only
	 * used to mark start and end. You can specify branch id at leaf node.
	 * @remark : 
	 * @return : 
	 */
	enum NodeType
	{
		NT_ROOT,
		NT_NORMAL,		//appear only once
		NT_LOOP,		//appear multiple times
		NT_LEAF
	};

	/**
	 * Ctor
	 * @param ahName : underlying argument handler name.
	 * @param NodeType : node type 
	 */
	Node(const std::string& name, const std::string& ahName, NodeType nt = NT_NORMAL);
	~Node();
	/**
	 * deep copy
	 */
	Node(const Node& rhs);

	/**
	 * Every node can have n child nodes
	 * @param child : child node
	 * @return : added child node
	 */
	Node* addChildNode(Node* child);

	Node* addChildNode(const std::string& name, const std::string& ahName, 
			NodeType nt = NT_NORMAL);

	/**
	 * Create child node, add it to children. 
	 * @param name child node name
	 * @param ahName : child node arg handler name 
	 * @param NodeType : child node type 
	 * @return : 
	 */

	/**
	 * Get child node by name. 
	 * @param name : child node name
	 * @param recursive : search child of child until leaf
	 * @return : child node with specified name 
	 */
	Node* getChildNode(const std::string& name, bool recursive = 1);

	/**
	 * Get parent node by name. This is always a recursive operation.
	 * @param name : parent node name
	 * @return : ancestor node with specified name 
	 */
	Node* getAncestorNode(const std::string& name);

	/**
	 * Add leaf node, end current buranch.
	 */
	Node* endBranch(int branch = 0);

	Node* getParent(){ return mParent; }
	void setParent( Node* v){mParent = v;}
	
	bool isRoot() const{ return  mNodeType == NT_ROOT; }
	bool isNormal() const{ return  mNodeType == NT_NORMAL; }
	bool isLeaf() const{ return  mNodeType == NT_LEAF; }
	bool isLoop() const{ return  mNodeType == NT_LOOP; }
	
	/**
	 * test s with underlying arg handler. Add value if it's NT_LOOP
	 * @param s : 
	 * @return : 
	 */
	bool validate(const std::string& s);

	/**
	 * add value if it's NT_LOOP.
	 * @param v : value
	 */
	void addValue(const std::string& v);

	const std::string& getValue(){return mArgHandler->getValue();}

	/**
	 * Recursively get child leaves 
	 * @return : node vector
	 */
	NodeVector getLeaves();

	/**
	 *  Join argument handler type until root. Only for leaf type.
	 */
	std::string getArgPath();

	ArgHandler* getArgHandler() const; 

	NodeType getNodeType() const { return mNodeType; }
	void setNodeType( NodeType v){mNodeType = v;}

	const std::string& getName() const { return mName; }
	void setName( const std::string& v){mName = v;}

	const std::string& getAhName() const { return mAhName; }
	void setAhName( const std::string& v){mAhName = v;}

	int getBranch();
	void setBranch(int v);

	NodeVector::const_iterator beginChildIter()const;
	NodeVector::const_iterator endChildIter()const;
	
	StringVector::const_iterator beginValueIter()const;
	StringVector::const_iterator endValueIter()const;

private:
	NodeType mNodeType;			//node type
	int mBranch;				//only for leaf node
	Node* mParent;				//parent node
	ArgHandler* mArgHandler;	//underlying arghandler
	std::string mName;			//node name in current tree
	std::string mAhName;		//argument handler name
	StringVector mValues;		//for loop node only
	NodeVector mChildren;		//children
};

/** 
 * Tree type arg handler. Used to deal with cmd which take multiple formats.
 * Every treeArgHandler starts with root, plus n normal, ends with n leaves.
 * You can get result route when you execute command.
 */
class TreeArgHandler: public ArgHandler
{
public:
	
	defArgCom(TreeArgHandler)

	TreeArgHandler(const std::string& name);
	/**
	 * deep copy
	 */
	TreeArgHandler(const TreeArgHandler& rhs);
	
	virtual void prompt(const std::string& s);
	virtual bool doValidate(const std::string& s);
	virtual void outputErrMessage(const std::string& s);

	Node* getRoot(){ return mRoot; }
	const Node* getRoot()const { return mRoot; }

	/**
	 * Get child node by name recursively. Throw if not found
	 * @param name : child node name
	 * @return : Child node pointer 
	 */
	Node* getNode(const std::string& name);

	/**
	 * Get string value catched by specified node.
	 * @param name : node name
	 * @return : catched value
	 */
	const std::string& getNodeValue(const std::string& name);

	/**
	 * get all leaves 
	 * @return : leaves vector
	 */
	NodeVector getLeaves();

	/**
	 * Get id of matched branch.
	 */
	int getMatchedBranch();

	Node* getMatchedLeaf() const { return mMatchedLeaf; }
	void setMatchedLeaf( Node* v){mMatchedLeaf = v;}

	/**
	 * Get node at matched branch 
	 * @param name : node name
	 * @return : node in matched branch with specified name 
	 */
	Node* getMatchedNode(const std::string& name);

private:

	/**
	 * Loop nv to valid s. Add child nodes to result if it's valid. 
	 * @param nv : candidate nodes
	 * @param s : test string
	 * @param noLeaf : filter leaf node
	 * @return : new candidate nodes 
	 */
	NodeVector validAndGetCandidate(const NodeVector& nv, const std::string& s, bool noLeaf);


private:
	Node* mRoot;
	Node* mMatchedLeaf;

};

/**
 * Registered with following type:
 *
 * blank
 * bool
 * short
 * ushort
 * int
 * uint
 * long
 * ulong
 * real
 * nreal
 * int2
 * int3
 * int4
 * int5
 * real2
 * real3
 * real4
 * real5
 * nreal2
 * nreal3
 * nreal4
 * nreal5
 * matrix2
 * matrix3
 * matrix4
 * nmatrix2
 * nmatrix3
 * nmatrix4
 * path
 * cmd
 * parameter
 * value
 *
 */
class ArgHandlerLib: public Singleton<ArgHandlerLib>
{
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
	 * register intrinsic arg handler 
	 */
	void init();

	/**
	 * Create arg handler.
	 * @param protoName : Previously registered prototype name
	 * @return : newly created arg handler 
	 */
	ArgHandler* createArgHandler(const std::string& protoName);

	Node* createRootNode(const std::string& name = "");
	Node* createLeafNode(int branch, const std::string& name = "");

	/**
	 * Create 1 branch only tree with sequence of the 1 type argument
	 * handler. This can be used to create int2 int3, real2, real3.......  Child
	 * node will be named as name_0, name_1.... name_n
	 * @param name : tree name
	 * @param ahName : item argument handler name 
	 * @param num : number of items 
	 * @return : newly created tree argument handler
	 */
	TreeArgHandler* createMonoTree(const std::string& name, const std::string& ahName, int num);

private:
	ArgHandlerMap mArgHandlerMap;
};

}


#endif /* PACARGHANDLER_H */
