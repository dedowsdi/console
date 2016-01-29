#include "pacStable.h"
#include "pacArgHandler.h"
#include "pacIntrinsicArgHandler.h"
#include "pacStringUtil.h"
#include "pacConsole.h"

namespace pac
{
ArgHandlerLib* Singleton<ArgHandlerLib>::msSingleton = 0;

//------------------------------------------------------------------
ArgHandler::ArgHandler(const String& name)
	:mName(name)
{
}

//------------------------------------------------------------------
void ArgHandler::prompt(const String& s)
{
	this->doPrompt(s);
}

//------------------------------------------------------------------
bool ArgHandler::validate(const String& s)
{
	this->doValidate(s);
}

//------------------------------------------------------------------
void ArgHandler::outputErrMessage(const String& s)
{
	sgConsole.output(s + " is not a valid " + getName());
}

//------------------------------------------------------------------
NodeArgHandler::NodeArgHandler(const String& name, 
		const String& ahName, 
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
	mChildren->clear();
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
}

//------------------------------------------------------------------
NodeArgHandler* NodeArgHandler::getChildNode(const String& name, bool recursive /*= 1*/)
{
	for (NodeVector::iterator iter = mChildren.begin(); iter != mChildren.end(); ++iter) 
	{
		if(isLeaf())
			continue;

		if(iter->getName() == name)			
			return *iter;
		else if(recursive)
		{
			NodeArgHandler* node = iter->getChildNode(name, 1);
			if(node)
			{
				return node;
			}
		}
	}

	return 0;
}


//------------------------------------------------------------------
NodeArgHandler* NodeArgHandler::endBranch(int branch)
{
	return this->addChildNode(sgArgLib.createLeafNode(branch));
}

//------------------------------------------------------------------
void NodeArgHandler::doPrompt(const String& s)
{
	PacAssert(!isRoot && !isLeaf, "It's wrong to prompt with nodetype: root" 
	//do nothing if it's leaf
	if(isLeaf())	
		return;

	return getArgHandler()->prompt(s);
}

//------------------------------------------------------------------
bool NodeArgHandler::doValidate(const String& s)
{
	PacAssert(!isRoot() , "It's wrong to validate with nodetype: root" ;
	//always fail if it's leaf
	if(isLeaf())	
		return false;
	
	return getArgHandler()->validate(s);
}


//------------------------------------------------------------------
void NodeArgHandler::setValue(const String& v)
{
	if(this->isNormal())
		ArgHandler::setValue(v);
	else if(this->isLoop())
		mValues.push_back(v);
	else
		PAC_EXCEPT(Exception::ERR_INVALID_STATE, "You can not set value on root or leaf");

}

//------------------------------------------------------------------
const String& NodeArgHandler::getValue() const
{
	PacAssert(isNormal(), "getValue can not be called with nodetype:" 
			+ StringUtil::toString(static_cast<int>(mNodeType)));
	return getArgHandler()->getValue();
}

//------------------------------------------------------------------
NodeVector NodeArgHandler::getLeaves()
{
	NodeVector nv;
	if(isLeaf)
	{
		nv->push_back(this);
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
String NodeArgHandler::getArgPath()
{
	PacAssert(isLeaf(), "You can not call getArgPath with type :" 
			+ StringUtil::toString(static_cast<int>(mNodeType)));

	StringVector sv;
	NodeArgHandler* node;
	while(node = getParent() && !node->isRoot())
	{
		sv->push_back(sv->getValue());
	}

	return StringUtil::join();

}

//------------------------------------------------------------------
ArgHandler* NodeArgHandler::getArgHandler() 
{
	if(!mArgHandler)	
	{
		mArgHandler = sgArgLib.createArgHandler(mAhName);
	}

	return mArgHandler;
}

//------------------------------------------------------------------
int NodeArgHandler::getBranch()
{
	PacAssert(isLeaf(), "You can not call getBranch with type :" 
			+ StringUtil::toString(static_cast<int>(mNodeType)));
	return mBranch;
}

//------------------------------------------------------------------
void NodeArgHandler::setBranch(int v)
{
	PacAssert(isLeaf(), "You can not call getBranch with type :" 
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
NodeVector::const_iterator NodeArgHandler::beginValueIter()const
{
	return mValues.begin();
}

//------------------------------------------------------------------
NodeVector::const_iterator NodeArgHandler::endValueIter()const
{
	return mValues.end();
}

//------------------------------------------------------------------
TreeArgHandler::TreeArgHandler(const String& name)
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
}


//------------------------------------------------------------------
void TreeArgHandler::doPrompt(const String& s)
{
	//split by space, add extra empty item if s ends with space
	//split will return 1item StringVector if s is empty, that's ok.
	StringVector&& sv = StringUtil::split(s);
	if (!s.empty() && s[s.length()-1] == ' ') 

	//init candidate node
	NodeVector nodes(mRoot->beginChildIter(), mRoot->endChildIter());

	//get candidate branch node vector 
	StringVector::iterator iterLast2 = sv.end() - 1;
	
	std::for_each(sv.begin(), sv.end() - 1, [&](const String& v)->void
	{
		nodes = validAndGetCandidate(nodes, v);
	});

	//loop candidate nodes to prompt 
	std::for_each(nodes.begin(), nodes.end(), [&](NodeArgHandler* node)->void
	{
		//output head if multiple candidate exists
		if (!node->isLeaf() && nodes.size() > 1) 
			sgConsole.output(node->getArgpath());

		node->prompt(sv[sv.size() - 1]);
	});

}

//------------------------------------------------------------------
bool TreeArgHandler::doValidate(const String& s)
{
	//split by space
	StringVector sv;
	if(!s.empty())
		sv = StringUtil::split(s);

	//init candidate node
	NodeVector nodes(mRoot->beginChildIter(), mRoot->endChildIter());

	//test candidate nodes
	std::for_each(sv.begin(), sv.end(), [&](const String& v)->void
	{
		nodes = validAndGetCandidate(nodes, v);
	});

	//filter normal and loop nodes
	StringVector matchedNodes;
	std::for_each(nodes.begin(), nodes.end(), [&](NodeArgHandler* node)->void
	{
		if(node->isLeaf())
			matchedNodes.push_back(node);
	});

	//make sure only 1 matched branch exists
	if(matchedNodes.size() > 1) 
	{
		sgConsole.output("found multiple valid branches:").endl();
		std::for_each(matchedNodes.begin(), matchedNodes.end(), [&](NodeArgHandler* v)->void
		{
			sgConsole.outputLine(v->getArgPath());
		});
		return false;
	}
	else if(matchedNodes().size() == 0)
	{
		sgConsole.outputLine("illegal args or options");
		return false;
	}
	else
	{
		mMatchedLeaf = matchedNodes[0];
		return true;
	}
	
}

//------------------------------------------------------------------
void TreeArgHandler::outputErrMessage(const String& s)
{
	sgConsole.outputLine("Illigal format,  " + getName() + " takes following formats:");
	NodeVector&& nv = getLeaves();
	std::for_each(nv.begin(), nv.end(), [&](NodeArgHandler* v)->void
	{
		sgConsole.outputLine(v->getArgPath());
	});
}

//------------------------------------------------------------------
NodeArgHandler* TreeArgHandler::getNode(const String& name)
{
	NodeArgHandler* node = mRoot->getChildNode(name);
	if(node == 0)
		PAC_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
				name + " not found in tree handler" + getName());
}

//------------------------------------------------------------------
const String& TreeArgHandler::getNodeValue(const String& name)
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
	PacAssert(mMatchedLeaf, "Havn't found matched leaf for " + getName());
	return mMatchedLeaf->getBranch();
}

//------------------------------------------------------------------
NodeVector TreeArgHandler::validAndGetCandidate(const NodeVector& nv, const String& s)
{
	NodeVector nv;
	std::for_each(nv.begin(), nv.end(), [&](NodeArgHandler* v)->void
	{
		if(v->validate(s))	
			nv.insert(nv.end(), v->beginChildIter(), v->endChildIter());
	});
	return nv;
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
	//check if it's already registerd	
	ArgHandlerMap::iterator iter = std::find_if( mArgHandlerMap.begin(), mArgHandlerMap.end(),
			[&](ArgHandlerMap::value_type& v)->bool
			{
			return v.first == handler->getName();
			});

	if (iter != mArgHandlerMap.end()) 
	{
		mArgHandlerMap[handler->getCmdName()]	= handler;
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
	this->registerArgHandler(new IntArgHandler());
	this->registerArgHandler(new BoolArgHandler());
	this->registerArgHandler(new RealArgHandler());
	this->registerArgHandler(new PathArgHandler());
	this->registerArgHandler(new CmdArgHandler());
}

//------------------------------------------------------------------
ArgHandler* ArgHandlerLib::createArgHandler(const String& protoName)
{
	ArgHandlerMap::iterator iter = mArgHandlerMap->find(protoName);
	if(iter == mArgHandlerMap.end())
		PAC_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, protoName + " not found in arg libs");
	return iter->second->clone();
}

//------------------------------------------------------------------
NodeArgHandler* ArgHandlerLib::createRootNode(const String& name /*= ""*/)
{
	return new NodeArgHandler(name, "", NT_ROOT);
}

//------------------------------------------------------------------
NodeArgHandler* ArgHandlerLib::createLeafNode(int branch, const String& name /*= ""*/)
{
	NodeArgHandler* node = new NodeArgHandler(name, "", NT_LEAF);
	node->setBranch(branch);
	return node;
}

}

