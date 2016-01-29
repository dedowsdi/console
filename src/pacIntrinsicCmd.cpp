#include "pacStable.h"
#include "pacIntrinsicCmd.h"
#include "pacConsole.h"
#include "pacAbsDir.h"
#include "pacArgHandler.h"

namespace pac
{

//------------------------------------------------------------------
LsCmd::LsCmd()
	:Command(ls)
{
}

//------------------------------------------------------------------
bool LsCmd::doExecute()
{
	TreeArgHandler* handler = static_cast<TreeArgHandler*>(mArgHandler);
	AbsDir* curDir = sgConsole.getDirectory();

	if (handler->getMatchedBranch() == 1) 
	{
		const StringVector& sv = handler->getNodeValues("path");
		std::for_each(sv.begin(), sv.end(), [&](const String& path)->void
		{
			AbsDir* dir = AbsDirUtil::findDir(curDir, path);
			sgConsole.output(dir->getName() + ":").endl();
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
	TreeArgHandler* handler = new TreeArgHandler();
	NodeArgHandler* root = handler->getRoot();
	root->endBranch(0);
	root->addChildNode("path", "path", NodeArgHandler::NT_LOOP)->endBranch(1);
	handler->setName(getDefAhName());

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
	this->mArgHandler = sgArgLig.createArgHandler("blank");
}

//------------------------------------------------------------------
CdCmd::CdCmd():
	Command("cd")
{
}

//------------------------------------------------------------------
bool CdCmd::doExecute()
{
	sgConsole->changeDirectory(AbsDirUtil::findDir(mArgHandler->getValue()));
}

//------------------------------------------------------------------
void CdCmd::buildArgHandler()
{
	this->mArgHandler = sgArgLig.createArgHandler("path");
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
	const String& property = handler->getNodeValue("property");
	NodeArgHandler* valueHanlder = handler->getNode("value");
	AbsDir* curDir = sgConsole.getDirectory();

	curDir->setProperty(property, valueHandler->getArgHandler());
}

//------------------------------------------------------------------
void SetCmd::buildArgHandler()
{
	TreeArgHandler* handler = new TreeArgHandler();
	this->mArgHandler = handler;
	NodeArgHandler* root = handler->getRoot();
	NodeArgHandler* node = root->addChildNode("property", "property");
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
		const StringVector& sv = handler->getNodeValues("path");
		std::for_each(sv.begin(), sv.end(), [&](const String& path)->void
		{
			AbsDir* dir = AbsDirUtil::findDir(curDir, path);
			sgConsole.output(dir->getName() + ":").endl();
			outputProperties(dir);
		});
	}
	else	//output current dir
	{	
		outputProperties(curDir);
	}
}

//------------------------------------------------------------------
void LpCmd::buildArgHandler()
{
	TreeArgHandler* handler = new TreeArgHandler();
	this->mArgHandler = handler;
	NodeArgHandler* root = handler->getRoot();
	root->endBranch(0)
	root->addChildNode("path", "path", NT_LOOP)->endBranch(1);
}

//------------------------------------------------------------------
void LpCmd::outputProperties(AbsDir* dir)
{
	PAC_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "not implemented", __FUNCTION__);
}

}
