#include "pacStable.h"
#include "pacArgHandler.h"
#include "pacIntrinsicArgHandler.h"
#include "pacStringUtil.h"
#include "pacStdUtil.h"
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
	this->populatePromptBuffer(s);
	this->applyPromptBuffer(s);
}

//------------------------------------------------------------------
bool ArgHandler::validate(const std::string& s)
{
	bool res = this->doValidate(s);
	if(res)
		this->setValue(s);
	return res;
}

//------------------------------------------------------------------
void ArgHandler::applyPromptBuffer(const std::string& s, bool autoComplete) 
{
	if(mPromptBuffer.empty())
		return;

	if(isNoteBuffer(mPromptBuffer[0]))
	{
		std::for_each(mPromptBuffer.begin(), mPromptBuffer.end(), 
				[&](const std::string&  v)->void
		{
			sgConsole.outputLine(v);
		});
	}
	else	
	{
		if(autoComplete)
		{
			RaiiConsoleBuffer();
			const std::string&& iden = StdUtil::getIdenticalString(mPromptBuffer.begin(),
					mPromptBuffer.end());
			//just check again
			if(!StringUtil::startsWith(iden, s))
				PAC_EXCEPT(Exception::ERR_INVALID_STATE, 
						"complete string done's starts with typing!!!!");

			sgConsole.complete(iden.substr(s.size()));
		}

		if(mPromptBuffer.size() > 1 || !autoComplete)
		{
			RaiiConsoleBuffer();
			std::for_each(mPromptBuffer.begin(), mPromptBuffer.end(), 
					[&](const std::string&  v)->void
			{
				sgConsole.output(v);
			});
		}
	}
}

//------------------------------------------------------------------
void ArgHandler::outputErrMessage(const std::string& s)
{
	sgConsole.outputLine(s + " is not a valid " + getName());
}

//------------------------------------------------------------------
StringVector::iterator ArgHandler::beginPromptBuffer()
{
	return mPromptBuffer.begin();
}

//------------------------------------------------------------------
StringVector::iterator ArgHandler::endPromptBuffer()
{
	return mPromptBuffer.end();
}

//------------------------------------------------------------------
size_t ArgHandler::getPromptBufferSize()
{
	return mPromptBuffer.size();
}

//------------------------------------------------------------------
void ArgHandler::appendNoteBuffer(const std::string& buf)
{
	mPromptBuffer.push_back("@@" + buf );
}

//------------------------------------------------------------------
void ArgHandler::appendCompleteBuffer(const std::string& buf)
{
	mPromptBuffer.push_back(buf);
}

//------------------------------------------------------------------
bool ArgHandler::isNoteBuffer(const std::string& buf)
{
	return StringUtil::startsWith(buf, "@@");
}

//------------------------------------------------------------------
bool ArgHandler::isCompleteBuf(const std::string& buf)
{
	return !StringUtil::startsWith(buf, "@@");
}

//------------------------------------------------------------------
Node::Node(const std::string& name, 
		const std::string& ahName, 
		NodeType nt /*= NT_NORMAL*/):
	mNodeType(nt)
	,mBranch(-1)
	,mParent(0)
	,mArgHandler(0)
	,mName(name)
	,mAhName(ahName)
{
}

//------------------------------------------------------------------
Node::~Node()
{
	std::for_each(mChildren.begin(), mChildren.end(), [&](Node* v)->void
	{
		delete v;	
	});
	mChildren.clear();
}


//------------------------------------------------------------------
Node::Node(const Node& rhs):
	mNodeType(rhs.mNodeType)
	,mBranch(rhs.mBranch)
	,mParent(rhs.mParent)
	,mArgHandler(0)
	,mName(rhs.getName())
	,mAhName(rhs.getAhName())
{
	//copy handler
	if(!mAhName.empty())
		mArgHandler = sgArgLib.createArgHandler(mAhName);

	if(!isRoot() && !isLeaf())
		PacAssert(mArgHandler, "failed to create ArgHandler");

	//deep copy children, take care of loop type.
	std::for_each(rhs.beginChildIter(), rhs.endChildIter(), [&](Node* v)->void
	{
		if(v->isLoop())
			this->addChildNode(this);
		else
			this->addChildNode(new Node(*v));
	});
}

//------------------------------------------------------------------
Node* Node::addChildNode(Node* child)
{
	mChildren.push_back(child);
	if(child != this)	 //loop type can add it self to child
		child->setParent(this);
	return child;
}

//------------------------------------------------------------------
Node* Node::addChildNode(const std::string& name, const std::string& ahName, 
		NodeType nt /*= NT_NORMAL*/)
{
	Node* node = new Node(name, ahName, nt);
	return this->addChildNode(node);
}

//------------------------------------------------------------------
Node* Node::getChildNode(const std::string& name, bool recursive /*= 1*/)
{
	for (NodeVector::iterator iter = mChildren.begin(); iter != mChildren.end(); ++iter) 
	{
		if(isLeaf())
			continue;

		if((*iter)->getName() == name)			
			return *iter;
		else if(recursive)
		{
			Node* node = (*iter)->getChildNode(name, 1);
			if(node)
			{
				return node;
			}
		}
	}

	return 0;
}

//------------------------------------------------------------------
Node* Node::getAncestorNode(const std::string& name)
{
	if(!mParent)
		return 0;

	if(mParent->getName() == name)
		return mParent;
	else
		return mParent->getAncestorNode(name);
}


//------------------------------------------------------------------
Node* Node::endBranch(int branch)
{
	return this->addChildNode(sgArgLib.createLeafNode(branch));
}

//------------------------------------------------------------------
bool Node::validate(const std::string& s)
{
	if(isRoot())
		PAC_EXCEPT(Exception::ERR_INVALID_STATE, "validate on root");

	if(isLeaf())
		return false;
	
	bool res = this->mArgHandler->validate(s);
	if(res && this->isLoop())
		this->addValue(mArgHandler->getValue());

	return res;
}

//------------------------------------------------------------------
void Node::addValue(const std::string& v)
{
	if(this->isLoop())
		mValues.push_back(v);
	else
		PAC_EXCEPT(Exception::ERR_INVALID_STATE, "You can not set value on root or leaf");

}


//------------------------------------------------------------------
NodeVector Node::getLeaves()
{
	NodeVector nv;
	if(isLeaf())
	{
		nv.push_back(this);
		return nv;
	}
	else
	{
		std::for_each(mChildren.begin(), mChildren.end(), [&](Node* v)->void
		{
			NodeVector&& childLeaves = v->getLeaves();
			nv.insert(nv.end(), childLeaves.begin(), childLeaves.end());
		});	
	}

	return nv;
}

//------------------------------------------------------------------
std::string Node::getArgPath()
{
	PacAssertS(isLeaf(), "You can not call getArgPath with type :" 
			+ StringUtil::toString(static_cast<int>(mNodeType)));

	StringVector sv;
	Node* node;
	while( (node = getParent()) && !node->isRoot())
	{
		sv.push_back(node->getValue());
	}

	return StringUtil::join(sv);

}

//------------------------------------------------------------------
ArgHandler* Node::getArgHandler() const 
{
	PacAssert(mArgHandler, "0 arghandler");
	return mArgHandler;
}

//------------------------------------------------------------------
int Node::getBranch()
{
	PacAssertS(isLeaf(), "You can not call getBranch with type :" 
			+ StringUtil::toString(static_cast<int>(mNodeType)));
	return mBranch;
}

//------------------------------------------------------------------
void Node::setBranch(int v)
{
	PacAssertS(isLeaf(), "You can not call getBranch with type :" 
			+ StringUtil::toString(static_cast<int>(mNodeType)));
	mBranch = v;
}

//------------------------------------------------------------------
NodeVector::const_iterator Node::beginChildIter()const
{
	return mChildren.begin();
}

//------------------------------------------------------------------
NodeVector::const_iterator Node::endChildIter()const
{
	return mChildren.end();
}

//------------------------------------------------------------------
StringVector::const_iterator Node::beginValueIter()const
{
	return mValues.begin();
}

//------------------------------------------------------------------
StringVector::const_iterator Node::endValueIter()const
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
	mRoot = new Node(*rhs.getRoot());
	//make sure no duplicate branch exists
	NodeVector&& leaves = mRoot->getLeaves();
	NodeVector::iterator iter = std::unique(leaves.begin(), leaves.end());
	PacAssert(iter == leaves.end(), "duplicate tree branes");
}


//------------------------------------------------------------------
void TreeArgHandler::prompt(const std::string& s)
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
		nodes = validAndGetCandidate(nodes, v, true);
	});

	if(nodes.size() == 1)	//one candidate
		nodes[0]->getArgHandler()->prompt(s);
	else
	{
		//mutiple candidates
		std::for_each(nodes.begin(), nodes.end(), [&](Node* node)->void
		{
			ArgHandler* handler = node->getArgHandler();
			handler->populatePromptBuffer(s);
			if (handler->getPromptBufferSize() > 0)
			{
				//output head
				sgConsole.outputLine(node->getArgPath() + ":");
				handler->applyPromptBuffer(s, false);
			}
		});
	}
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
		nodes = validAndGetCandidate(nodes, v, false);
	});

	//filter normal and loop nodes
	NodeVector matchedNodes;
	std::for_each(nodes.begin(), nodes.end(), [&](Node* node)->void
	{
		if(node->isLeaf())
			matchedNodes.push_back(node);
	});

	//make sure only 1 matched branch exists
	if(matchedNodes.size() > 1) 
	{
		sgConsole.outputLine("found multiple valid branches:");
		std::for_each(matchedNodes.begin(), matchedNodes.end(), [&](Node* v)->void
		{
			sgConsole.outputLine(v->getArgPath());
		});
		return false;
	}
	else if(matchedNodes.size() == 0)
	{
		//appendNoteBufferLine("illegal args or options");
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
	std::for_each(nv.begin(), nv.end(), [&](Node* v)->void
	{
		sgConsole.outputLine(v->getArgPath());
	});
}

//------------------------------------------------------------------
Node* TreeArgHandler::getNode(const std::string& name)
{
	Node* node = mRoot->getChildNode(name);
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
Node* TreeArgHandler::getMatchedNode(const std::string& name)
{
	return getMatchedLeaf()->getAncestorNode(name);
}

//------------------------------------------------------------------
NodeVector TreeArgHandler::validAndGetCandidate(const NodeVector& nv, 
		const std::string& s, bool noLeaf)
{
	NodeVector candidates;
	std::for_each(nv.begin(), nv.end(), [&](Node* v)->void
	{
		if(v->validate(s))
		{
			if(!noLeaf)
				candidates.insert(candidates.end(), v->beginChildIter(), v->endChildIter());
			else
			{
				std::for_each(v->beginChildIter(), v->endChildIter(), [&](Node* n)->void
				{
					if(!n->isLeaf())
						candidates.push_back(n);
				});
			}
		}
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
Node* ArgHandlerLib::createRootNode(const std::string& name /*= ""*/)
{
	return new Node(name, "", Node::NT_ROOT);
}

//------------------------------------------------------------------
Node* ArgHandlerLib::createLeafNode(int branch, const std::string& name /*= ""*/)
{
	Node* node = new Node(name, "", Node::NT_LEAF);
	node->setBranch(branch);
	return node;
}

//------------------------------------------------------------------
TreeArgHandler* ArgHandlerLib::createMonoTree(const std::string& name, const std::string& ahName, int num)
{
	TreeArgHandler* tree = new TreeArgHandler(name);

	Node* node = tree->getRoot();
	for (int i = 0; i < num; ++i) 
	{
		node = node->addChildNode(name + StringUtil::toString(i), ahName);
	}
	node->endBranch(0);

	return tree;
}

}

