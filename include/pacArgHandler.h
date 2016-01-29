#ifndef PACARGHANDLER_H
#define PACARGHANDLER_H 

#include "pacSingleton.h"

namespace pac
{

#define defArgCom(type)\
virtual ArgHandler* clone(){return new type(*this);}
/**
 * Base class of all Argument handler.
 */
class ArgHandler 
{
public:
	

	ArgHandler(const String& name);
	virtual ~ArgHandler(){}

	/**
	 * complete, doPrompt for current typing.
	 */
	void prompt(const String& s);
	/**
	 * check if s is valid. Set mValue to s if it's valid. 
	 */
	bool validate(const String& s);

	virtual void doPrompt(const String& s) = 0;
	virtual bool doValidate(const String& s) = 0;
	virtual void outputErrMessage(const String& s);

	/**
	 * This name will be used at ArgHanderLib to reference this handler.
	 */
	const String& getName() const { return mName; }
	void setName( const String& v){mName = v;}

	virtual const String& getValue() const { return mValue; }
	virtual void setValue( const String& v){mValue = v;}
	
	virtual ArgHandler* clone() = 0;

private:
	String mName;
	String mValue;

};

typedef std::vector<NodeArgHandler*> NodeVector;
/**
 * Used in TreeHandler, represent a node in tree. Normal node handler need
 * another arghandler to do the doPrompt and validte.
 */
class NodeArgHandler: public ArgHandler
{
public:

	defArgCom(NodeArgHandler)

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
	NodeArgHandler(const String& name, const String& ahName, NodeType nt = NT_NORMAL);
	~NodeArgHandler();
	/**
	 * deep copy
	 */
	NodeArgHandler(const NodeArgHandler& rhs);

	/**
	 * Every node can have n child nodes
	 * @param child : child node
	 * @return : added child node
	 */
	NodeArgHandler* addChildNode(NodeArgHandler* child);

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
	NodeArgHandler* getChildNode(const String& name, bool recursive = 1);


	NodeArgHandler* addChildNode(const String& name, const String& ahName, 
			NodeType = NT_NORMAL);

	/**
	 * Add leaf node, end current buranch.
	 */
	NodeArgHandler* endBranch(int branch = 0);

	NodeArgHandler* getParent(){ return mParent; }
	void setParent( NodeArgHandler* v){mParent = v;}
	
	bool isRoot(){ return  mNodeType == NT_ROOT; }
	bool isNormal(){ return  mNodeType == NT_NORMAL; }
	bool isLeaf(){ return  mNodeType == NT_LEAF; }
	bool isLoop(){ return  mNodeType == NT_LOOP; }
	
	virtual void doPrompt(const String& s);
	virtual bool doValidate(const String& s);

	/**
	 * Set value if it's type of NT_NORMAL, add value if it's NT_LOOP.
	 * Throw invalid state if it's root or leaf
	 * @param v : value
	 */
	virtual void setValue(const String& v);
	const String& getValue() const;

	/**
	 * Recursively get child leaves 
	 * @return : node vector
	 */
	NodeVector getLeaves();

	/**
	 *  Join argument handler type until root. Only for leaf type.
	 */
	String getArgPath();

	/**
	 * Get underlying arg handler. Create it with mAhName if it's 0 
	 * @return : underlying arg handler
	 */
	ArgHandler* getArgHandler(); 

	NodeType getNodeType() const { return mNodeType; }
	void setNodeType( NodeType v){mNodeType = v;}


	int getBranch();
	void setBranch(int v);

	NodeVector::const_iterator beginChildIter()const;
	NodeVector::const_iterator endChildIter()const;
	
	NodeVector::const_iterator beginValueIter()const;
	NodeVector::const_iterator endValueIter()const;

private:
	NodeType mNodeType;			
	int mBranch;				//only for leaf node

	NodeArgHandler* mParent;	//parent node
	ArgHandler* mArgHandler;	//underlying arghandler

	NodeVector mChildren;		//children

	String mAhName;				//underlying arghandler name
	String mNodeName;			//node name in current tree
	String mValue;				//for normal node
	StringVector mValues;		//for loop node
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

	TreeArgHandler(const String& name);
	/**
	 * deep copy
	 */
	TreeArgHandler(const TreeArgHandler& rhs);
	
	virtual void doPrompt(const String& s);
	virtual bool doValidate(const String& s);
	virtual void outputErrMessage(const String& s);

	NodeArgHandler* getRoot(){ return mRoot; }
	const NodeArgHandler* getRoot()const { return mRoot; }

	/**
	 * Get child node by name recursively. Throw if not found
	 * @param name : child node name
	 * @return : Child node pointer 
	 */
	NodeArgHandler* getNode(const String& name);

	/**
	 * Get string value catched by specified node.
	 * @param name : node name
	 * @return : catched value
	 */
	const String& getNodeValue(const String& name);

	/**
	 * get all leaves 
	 * @return : leaves vector
	 */
	NodeVector getLeaves();

	/**
	 * Get id of matched branch.
	 */
	int getMatchedBranch();

private:

	/**
	 * Loop nv to valid s. Add child nodes to result if it's valid. 
	 * @param nv : candidate nodes
	 * @param s : test string
	 * @return : new candidate nodes 
	 */
	NodeVector validAndGetCandidate(const NodeVector& nv, const String& s);


private:
	NodeArgHandler* mRoot;
	NodeArgHandler* mMatchedLeaf;

};

class ArgHandlerLib: public Singleton<ArgHandlerLib>
{
public:

	typedef std::map<String, ArgHandler*> ArgHandlerMap;

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
	ArgHandler* createArgHandler(const String& protoName);

	NodeArgHandler* createRootNode(const String& name = "");
	NodeArgHandler* createLeafNode(int branch, const String& name = "");

private:
	ArgHandlerMap mArgHandlerMap;
};

}


#endif /* PACARGHANDLER_H */
