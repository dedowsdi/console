#include "pacStable.h"
#include "pacIntrinsicCmd.h"
#include "pacConsole.h"
#include "pacAbsDir.h"
#include "pacArgHandler.h"

namespace pac
{

//------------------------------------------------------------------
LsCmd::LsCmd()
	:Command("ls")
{
}

//------------------------------------------------------------------
bool LsCmd::doExecute()
{
	TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
	AbsDir* curDir = sgConsole.getDirectory();

	if (handler->getMatchedBranch() == 1) 
	{
		Node* pathNode = handler->getMatchedNode("path");
		std::for_each(pathNode->beginValueIter(), pathNode->endValueIter(), 
				[&](const std::string& path)->void
				{
				AbsDir* dir = AbsDirUtil::findPath(path, curDir);
				sgConsole.outputLine(dir->getName() + ":");
				outputChildren(dir);
				});
	}
	else	//output current dir
	{	
		outputChildren(curDir);
	}
	return true;
}

//------------------------------------------------------------------
void LsCmd::buildArgHandler()
{
	TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
	Node* root = handler->getRoot();
	root->endBranch(0);
	root->addChildNode("path", "path", Node::NT_LOOP)->endBranch(1);
	this->mArgHandler = handler;
}

//------------------------------------------------------------------
void LsCmd::outputChildren(AbsDir* dir)
{
	RaiiConsoleBuffer();
	std::for_each(dir->beginChildIter(), dir->endChildIter(), [&](AbsDir* childDir)->void
	{
		sgConsole.output(childDir->getName());
	});
}

//------------------------------------------------------------------
PwdCmd::PwdCmd():
	Command("pwd")
{
}

//------------------------------------------------------------------
bool PwdCmd::doExecute()
{
	AbsDir* curDir = sgConsole.getDirectory();
	sgConsole.output(curDir->getName());
	return true;
}

//------------------------------------------------------------------
void PwdCmd::buildArgHandler()
{
	this->mArgHandler = sgArgLib.createArgHandler("blank");
}

//------------------------------------------------------------------
CdCmd::CdCmd():
	Command("cd")
{
}

//------------------------------------------------------------------
bool CdCmd::doExecute()
{
	AbsDir* curDir = sgConsole.getDirectory();
	AbsDir* targetDir = AbsDirUtil::findPath(mArgHandler->getValue(), curDir);
	sgConsole.changeCurrentDirectory(targetDir);
	return true;
}

//------------------------------------------------------------------
void CdCmd::buildArgHandler()
{
	this->mArgHandler = sgArgLib.createArgHandler("path");
}

//------------------------------------------------------------------
SetCmd::SetCmd():
	Command("set")
{
}

//------------------------------------------------------------------
bool SetCmd::doExecute()
{
	TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
	const std::string& property = handler->getNodeValue("parameter");
	Node* valueNode = handler->getMatchedNode("value");
	AbsDir* curDir = sgConsole.getDirectory();

	curDir->setParameter(property, valueNode->getArgHandler());
	return true;
}

//------------------------------------------------------------------
void SetCmd::buildArgHandler()
{
	TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
	this->mArgHandler = handler;
	Node* root = handler->getRoot();
	Node* node = root->addChildNode("parameter", "parameter");
	node->addChildNode("value", "value")->endBranch();
}

//------------------------------------------------------------------
LpCmd::LpCmd():
	Command("lp")
{
}

//------------------------------------------------------------------
bool LpCmd::doExecute()
{
	TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
	AbsDir* curDir = sgConsole.getDirectory();

	if (handler->getMatchedBranch() == 1) 
	{
		Node* pathNode = handler->getMatchedNode("path");
		std::for_each(pathNode->beginValueIter(), pathNode->endValueIter(), 
				[&](const std::string& path)->void
				{
				AbsDir* dir = AbsDirUtil::findPath(path, curDir);
				sgConsole.outputLine(dir->getName() + ":");
				outputProperties(dir);
				});
	}
	else	//output current dir
	{	
		outputProperties(curDir);
	}

	return true;
}

//------------------------------------------------------------------
void LpCmd::buildArgHandler()
{
	TreeArgHandler* handler = new TreeArgHandler(getDefAhName());
	this->mArgHandler = handler;
	Node* root = handler->getRoot();
	root->endBranch(0);
	root->addChildNode("path", "path", Node::NT_LOOP)->endBranch(1);
}

//------------------------------------------------------------------
void LpCmd::outputProperties(AbsDir* dir)
{
	RaiiConsoleBuffer();
	StringVector&& params = dir->getParameters();
	std::for_each(params.begin(), params.end(), [&](const std::string&  v)->void
	{
		sgConsole.output(v + " : " + dir->getParameter(v));
	});
}

}
