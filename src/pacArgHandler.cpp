#include "pacStable.h"
#include "pacArgHandler.h"
#include "pacIntrinsicArgHandler.h"
#include "pacStringUtil.h"
#include "pacConsole.h"

namespace pac
{
template<> ArgHandlerLib* Singleton<ArgHandlerLib>::msSingleton = 0;

//------------------------------------------------------------------
ArgHandler::ArgHandler(const std::string& name)
	:mName(name)
	 ,mNode(0)
{
}

//------------------------------------------------------------------
void ArgHandler::prompt(const std::string& s)
{
	this->doPrompt(s);
}

//------------------------------------------------------------------
bool ArgHandler::validate(const std::string& s)
{
	return this->doValidate(s);
}

//------------------------------------------------------------------
void ArgHandler::outputErrMessage(const std::string& s)
{
	sgConsole.output(s + " is not a valid " + getName());
}

//------------------------------------------------------------------
NodeArgHandler::NodeArgHandler(const std::string& name, 
		const std::string& ahName, 
		NodeType nt /*= NT_NORMAL*/):
	ArgHandler(name)
	,mNodeType(nt)
	,mBranch(-1)
	,mParent(0)
	,mArgHandler(0)
	,mAhName(ahName)
{
}

//------------------------------------------------------------------
NodeArgHandler::~NodeArgHandler()
{
	std::for_each(mChildren.begin(), mChildren.end(), [&](NodeArgHandler* v)->void
	{
		delete v;	
	});
	mChildren.clear();
}


//------------------------------------------------------------------
NodeArgHandler::NodeArgHandler(const NodeArgHandler& rhs):
	ArgHandler(rhs.getName())
	,mNodeType(rhs.mNodeType)
	,mBranch(rhs.mBranch)
	,mParent(rhs.mParent)
	,mArgHandler(0)
	,mAhName(rhs.mAhName)
{
	//deep copy children, take care of loop type.
	std::for_each(rhs.beginChildIter(), rhs.endChildIter(), [&](NodeArgHandler* v)->void
	{
		if(v->isLoop())
			this->addChildNode(this);
		else
			this->addChildNode(new NodeArgHandler(*v));
	});
}

//------------------------------------------------------------------
NodeArgHandler* NodeArgHandler::addChildNode(NodeArgHandler* child)
{
	mChildren.push_back(child);
	if(child != this)	 //loop type can add it self to child
		child->setParent(this);
	return child;
}

//------------------------------------------------------------------
NodeArgHandler* NodeArgHandler::addChildNode(const std::string& name, const std::string& ahName, 
		NodeType nt /*= NT_NORMAL*/)
{
	NodeArgHandler* node = new NodeArgHandler(name, ahName, nt);
	return this->addChildNode(node);
}

//------------------------------------------------------------------
NodeArgHandler* NodeArgHandler::getChildNode(const std::string& name, bool recursive /*= 1*/)
{
	for (NodeVector::iterator iter = mChildren.begin(); iter != mChildren.end(); ++iter) 
	{
		if(isLeaf())
			continue;

		if((*iter)->getName() == name)			
			return *iter;
		else if(recursive)
		{
			NodeArgHandler* node = (*iter)->getChildNode(name, 1);
			if(node)
			{
				return node;
			}
		}
	}

	return 0;
}

//------------------------------------------------------------------
NodeArgHandler* NodeArgHandler::getAncestorNode(const std::string& name)
{
	if(!mParent)
		return 0;

	if(mParent->getName() == name)
		return mParent;
	else
		return mParent->getAncestorNode(name);
}


//------------------------------------------------------------------
NodeArgHandler* NodeArgHandler::endBranch(int branch)
{
	return this->addChildNode(sgArgLib.createLeafNode(branch));
}

//------------------------------------------------------------------
void NodeArgHandler::doPrompt(const std::string& s)
{
	PacAssert(!isRoot() && !isLeaf(), "It's wrong to prompt with nodetype: root");
	//do nothing if it's leaf
	if(isLeaf())	
		return;

	return getArgHandler()->prompt(s);
}

//------------------------------------------------------------------
bool NodeArgHandler::doValidate(const std::string& s)
{
	PacAssert(!isRoot() , "It's wrong to validate with nodetype: root");
	//always fail if it's leaf
	if(isLeaf())	
		return false;
	
	return getArgHandler()->validate(s);
}


//------------------------------------------------------------------
void NodeArgHandler::setValue(const std::string& v)
{
	if(this->isNormal())
		ArgHandler::setValue(v);
	else if(this->isLoop())
		mValues.push_back(v);
	else
		PAC_EXCEPT(Exception::ERR_INVALID_STATE, "You can not set value on root or leaf");

}


//------------------------------------------------------------------
NodeVector NodeArgHandler::getLeaves()
{
	NodeVector nv;
	if(isLeaf())
	{
		nv.push_back(this);
		return nv;
	}
	else
	{
		std::for_each(mChildren.begin(), mChildren.end(), [&](NodeArgHandler* v)->void
		{
			NodeVector&& childLeaves = v->getLeaves();
			nv.insert(nv.end(), childLeaves.begin(), childLeaves.end());
		});	
	}

	return nv;
}


//------------------------------------------------------------------
std::string NodeArgHandler::getArgPath()
{
	PacAssertS(isLeaf(), "You can not call getArgPath with type :" 
			+ StringUtil::toString(static_cast<int>(mNodeType)));

	StringVector sv;
	NodeArgHandler* node;
	while( (node = getParent()) && !node->isRoot())
	{
		sv.push_back(node->getValue());
	}

	return StringUtil::join(sv);

}

//------------------------------------------------------------------
ArgHandler* NodeArgHandler::getArgHandler() 
{
	if(!mArgHandler)	
	{
		mArgHandler = sgArgLib.createArgHandler(mAhName);
		mArgHandler->setNode(this);
	}

	return mArgHandler;
}


//------------------------------------------------------------------
int NodeArgHandler::getBranch()
{
	PacAssertS(isLeaf(), "You can not call getBranch with type :" 
			+ StringUtil::toString(static_cast<int>(mNodeType)));
	return mBranch;
}

//------------------------------------------------------------------
void NodeArgHandler::setBranch(int v)
{
	PacAssertS(isLeaf(), "You can not call getBranch with type :" 
			+ StringUtil::toString(static_cast<int>(mNodeType)));
	mBranch = v;
}

//------------------------------------------------------------------
NodeVector::const_iterator NodeArgHandler::beginChildIter()const
{
	return mChildren.begin();
}

//------------------------------------------------------------------
NodeVector::const_iterator NodeArgHandler::endChildIter()const
{
	return mChildren.end();
}

//------------------------------------------------------------------
StringVector::const_iterator NodeArgHandler::beginValueIter()const
{
	return mValues.begin();
}

//------------------------------------------------------------------
StringVector::const_iterator NodeArgHandler::endValueIter()const
{
	return mValues.end();
}

//------------------------------------------------------------------
TreeArgHandler::TreeArgHandler(const std::string& name)
	:ArgHandler(name)
	 ,mMatchedLeaf(0)
{
	mRoot = sgArgLib.createRootNode();
}

//------------------------------------------------------------------
TreeArgHandler::TreeArgHandler(const TreeArgHandler& rhs)
	:ArgHandler(rhs.getName())
	 ,mMatchedLeaf(0)
{
	mRoot = new NodeArgHandler(*rhs.getRoot());
	//make sure no duplicate branch exists
	NodeVector&& leaves = mRoot->getLeaves();
	NodeVector::iterator iter = std::unique(leaves.begin(), leaves.end());
	PacAssert(iter == leaves.end(), "duplicate tree branes");
}


//------------------------------------------------------------------
void TreeArgHandler::doPrompt(const std::string& s)
{
	//split by space, add extra empty item if s ends with space
	//split will return 1item StringVector if s is empty, that's ok.
	StringVector&& sv = StringUtil::split(s);
	if (!s.empty() && s[s.length()-1] == ' ') 
		sv.push_back("");
		
	//init candidate node
	NodeVector nodes(mRoot->beginChildIter(), mRoot->endChildIter());

	//get candidate branch node vector 
	std::for_each(sv.begin(), sv.end() - 1, [&](const std::string& v)->void
	{
		nodes = validAndGetCandidate(nodes, v);
	});

	//loop candidate nodes to prompt 
	std::for_each(nodes.begin(), nodes.end(), [&](NodeArgHandler* node)->void
	{
		//output head if multiple candidate exists
		if (!node->isLeaf() && nodes.size() > 1) 
			sgConsole.output(node->getArgPath());

		node->prompt(sv[sv.size() - 1]);
	});
}

//------------------------------------------------------------------
bool TreeArgHandler::doValidate(const std::string& s)
{
	//split by space
	StringVector sv;
	if(!s.empty())
		sv = StringUtil::split(s);

	//init candidate node
	NodeVector nodes(mRoot->beginChildIter(), mRoot->endChildIter());

	//test candidate nodes
	std::for_each(sv.begin(), sv.end(), [&](const std::string& v)->void
	{
		nodes = validAndGetCandidate(nodes, v);
	});

	//filter normal and loop nodes
	NodeVector matchedNodes;
	std::for_each(nodes.begin(), nodes.end(), [&](NodeArgHandler* node)->void
	{
		if(node->isLeaf())
			matchedNodes.push_back(node);
	});

	//make sure only 1 matched branch exists
	if(matchedNodes.size() > 1) 
	{
		sgConsole.outputLine("found multiple valid branches:");
		std::for_each(matchedNodes.begin(), matchedNodes.end(), [&](NodeArgHandler* v)->void
		{
			sgConsole.outputLine(v->getArgPath());
		});
		return false;
	}
	else if(matchedNodes.size() == 0)
	{
		//sgConsole.outputLine("illegal args or options");
		return false;
	}
	else
	{
		setMatchedLeaf(matchedNodes[0]);
		return true;
	}
	
}

//------------------------------------------------------------------
void TreeArgHandler::outputErrMessage(const std::string& s)
{
	sgConsole.outputLine("Illigal format,  " + getName() + " takes following formats:");
	NodeVector&& nv = getLeaves();
	std::for_each(nv.begin(), nv.end(), [&](NodeArgHandler* v)->void
	{
		sgConsole.outputLine(v->getArgPath());
	});
}

//------------------------------------------------------------------
NodeArgHandler* TreeArgHandler::getNode(const std::string& name)
{
	NodeArgHandler* node = mRoot->getChildNode(name);
	if(node == 0)
		PAC_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
				name + " not found in tree handler" + getName());
	return node;
}

//------------------------------------------------------------------
const std::string& TreeArgHandler::getNodeValue(const std::string& name)
{
	return getNode(name)->getValue();
}

//------------------------------------------------------------------
NodeVector TreeArgHandler::getLeaves()
{
	return mRoot->getLeaves();
}

//------------------------------------------------------------------
int TreeArgHandler::getMatchedBranch()
{
	PacAssertS(mMatchedLeaf, "Havn't found matched leaf for " + getName());
	return mMatchedLeaf->getBranch();
}

//------------------------------------------------------------------
NodeArgHandler* TreeArgHandler::getMatchedNode(const std::string& name)
{
	return getMatchedLeaf()->getAncestorNode(name);
}

//------------------------------------------------------------------
NodeVector TreeArgHandler::validAndGetCandidate(const NodeVector& nv, const std::string& s)
{
	NodeVector candidates;
	std::for_each(nv.begin(), nv.end(), [&](NodeArgHandler* v)->void
	{
		if(v->validate(s))	
			candidates.insert(candidates.end(), v->beginChildIter(), v->endChildIter());
	});
	return candidates;
}

//------------------------------------------------------------------
ArgHandlerLib::~ArgHandlerLib()
{
	std::for_each(mArgHandlerMap.begin(), mArgHandlerMap.end(), 
			[&](ArgHandlerMap::value_type& v)->void
			{
				delete v.second;	
			});
	mArgHandlerMap.clear();
}



//------------------------------------------------------------------
void ArgHandlerLib::registerArgHandler(ArgHandler* handler)
{
	PacAssert(!handler->getName().empty(), "empty handler name");
	std::cout << "register handler " + handler->getName() << std::endl;
	//check if it's already registerd	
	ArgHandlerMap::iterator iter = std::find_if( mArgHandlerMap.begin(), mArgHandlerMap.end(),
			[&](ArgHandlerMap::value_type& v)->bool
			{
			return v.first == handler->getName();
			});

	if (iter == mArgHandlerMap.end()) 
	{
		mArgHandlerMap[handler->getName()] = handler;
	}
	else
	{
		PAC_EXCEPT(Exception::ERR_DUPLICATE_ITEM, handler->getName() + " already registed!");
	}
}

//------------------------------------------------------------------
void ArgHandlerLib::init()
{
	this->registerArgHandler(new BlankArgHandler());
	this->registerArgHandler(new BoolArgHandler());
	//primitive decimal arg handlers 
	this->registerArgHandler(new PriDeciArgHandler<short>("short"));
	this->registerArgHandler(new PriDeciArgHandler<unsigned short>("ushort"));
	this->registerArgHandler(new PriDeciArgHandler<int>("int"));
	this->registerArgHandler(new PriDeciArgHandler<unsigned int>("uint"));
	this->registerArgHandler(new PriDeciArgHandler<long>("long"));
	this->registerArgHandler(new PriDeciArgHandler<unsigned long>("ulong"));
	this->registerArgHandler(new PriDeciArgHandler<Real>("real"));
	//normalized real
	this->registerArgHandler(new PriDeciRangeArgHandler<Real>("nreal",-1.0, 1.0));
	//int 2..int5, real2..real5, matrix2, matrix3, matrix4
	this->registerArgHandler(sgArgLib.createMonoTree("int2","int", 2));
	this->registerArgHandler(sgArgLib.createMonoTree("int3","int", 3));
	this->registerArgHandler(sgArgLib.createMonoTree("int4","int", 4));
	this->registerArgHandler(sgArgLib.createMonoTree("int5","int", 5));
	this->registerArgHandler(sgArgLib.createMonoTree("real2","real", 2));
	this->registerArgHandler(sgArgLib.createMonoTree("real3","real", 3));
	this->registerArgHandler(sgArgLib.createMonoTree("real4","real", 4));
	this->registerArgHandler(sgArgLib.createMonoTree("real5","real", 5));
	this->registerArgHandler(sgArgLib.createMonoTree("nreal2","nreal", 2));
	this->registerArgHandler(sgArgLib.createMonoTree("nreal3","nreal", 3));
	this->registerArgHandler(sgArgLib.createMonoTree("nreal4","nreal", 4));
	this->registerArgHandler(sgArgLib.createMonoTree("nreal5","nreal", 5));
	this->registerArgHandler(sgArgLib.createMonoTree("matrix2","real", 4));
	this->registerArgHandler(sgArgLib.createMonoTree("matrix3","real", 9));
	this->registerArgHandler(sgArgLib.createMonoTree("matrix4","real", 16));
	this->registerArgHandler(sgArgLib.createMonoTree("nmatrix2","nreal", 4));
	this->registerArgHandler(sgArgLib.createMonoTree("nmatrix3","nreal", 9));
	this->registerArgHandler(sgArgLib.createMonoTree("nmatrix4","nreal", 16));

	//special handlers
	this->registerArgHandler(new PathArgHandler());
	this->registerArgHandler(new CmdArgHandler());
	this->registerArgHandler(new ParameterArgHandler());
	this->registerArgHandler(new ValueArgHandler());
}

//------------------------------------------------------------------
ArgHandler* ArgHandlerLib::createArgHandler(const std::string& protoName)
{
	ArgHandlerMap::iterator iter = mArgHandlerMap.find(protoName);
	if(iter == mArgHandlerMap.end())
		PAC_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, protoName + " not found in arg libs");
	return iter->second->clone();
}

//------------------------------------------------------------------
NodeArgHandler* ArgHandlerLib::createRootNode(const std::string& name /*= ""*/)
{
	return new NodeArgHandler(name, "", NodeArgHandler::NT_ROOT);
}

//------------------------------------------------------------------
NodeArgHandler* ArgHandlerLib::createLeafNode(int branch, const std::string& name /*= ""*/)
{
	NodeArgHandler* node = new NodeArgHandler(name, "", NodeArgHandler::NT_LEAF);
	node->setBranch(branch);
	return node;
}

//------------------------------------------------------------------
TreeArgHandler* ArgHandlerLib::createMonoTree(const std::string& name, const std::string& ahName, int num)
{
	TreeArgHandler* tree = new TreeArgHandler(name);

	NodeArgHandler* node = tree->getRoot();
	for (int i = 0; i < num; ++i) 
	{
		node = node->addChildNode(name + StringUtil::toString(i), ahName);
	}
	node->endBranch(0);

	return tree;
}

}

