#include "pacStable.h"
#include "pacIntrinsicArgHandler.h"
#include "pacAbsDir.h"
#include "pacConsole.h"

namespace pac
{

//------------------------------------------------------------------
BoolArgHandler::BoolArgHandler():
	ArgHandler("bool")
{
}

//------------------------------------------------------------------
void BoolArgHandler::doPrompt(const std::string& s)
{
	sgConsole.output("@@bool@@");
}

//------------------------------------------------------------------
bool BoolArgHandler::doValidate(const std::string& s)
{
	return s == "true" || s == "false";
}

//------------------------------------------------------------------
StringArgHandler::StringArgHandler(const std::string& name, const std::string& text):
	ArgHandler(name)
{
	mStrings.insert(text);
}


//------------------------------------------------------------------
StringArgHandler::StringArgHandler(const std::string& name):
	ArgHandler(name)
{
}

//------------------------------------------------------------------
StringArgHandler& StringArgHandler::insert(const std::string& s)
{
	mStrings.insert(s);
	return *this;
}

//------------------------------------------------------------------
void StringArgHandler::remove(const std::string& s)
{
	mStrings.erase(s);
}

//------------------------------------------------------------------
void StringArgHandler::doPrompt(const std::string& s)
{
	RaiiConsoleBuffer();
	std::for_each(mStrings.begin(), mStrings.end(), [&](const std::string& v)->void
	{
		if(StringUtil::startsWith(s, v))
		{
			sgConsole.output(v);
		}
	});	
}

//------------------------------------------------------------------
bool StringArgHandler::doValidate(const std::string& s)
{
	return exist(s);
}

//------------------------------------------------------------------
bool StringArgHandler::exist(const std::string& value)
{
	return mStrings.find(value) != mStrings.end();
}


//------------------------------------------------------------------
BlankArgHandler::BlankArgHandler()
	:ArgHandler("blank")
{
}

//------------------------------------------------------------------
void BlankArgHandler::doPrompt(const std::string& s)
{
	sgConsole.outputLine("@@blank@@");
}

//------------------------------------------------------------------
bool BlankArgHandler::doValidate(const std::string& s)
{
	return s.empty();
}


//------------------------------------------------------------------
PathArgHandler::PathArgHandler()
	:ArgHandler("path")
	 ,mDir(0)
{
}

//------------------------------------------------------------------
PathArgHandler::PathArgHandler(const PathArgHandler& rhs):
	ArgHandler(rhs.getName())
{
	setDir(sgConsole.getDirectory());
}

//------------------------------------------------------------------
void PathArgHandler::doPrompt(const std::string& s)
{
	RaiiConsoleBuffer();

	const std::string&& head = StringUtil::getHead(s);
	const std::string&& tail = StringUtil::getTail(s);

	AbsDir* headDir = AbsDirUtil::findPath(head, mDir);

	std::for_each(headDir->beginChildIter(), headDir->endChildIter() , [&](AbsDir* v)->void
	{
		if (StringUtil::startsWith(v->getName(), tail)) 
		{
			sgConsole.output(v->getName());
		}	
	});
}

//------------------------------------------------------------------
bool PathArgHandler::doValidate(const std::string& s)
{
	return AbsDirUtil::findPath(s, mDir) != 0;
}


//------------------------------------------------------------------
CmdArgHandler::CmdArgHandler():
	StringArgHandler("cmd")
{
	std::for_each(sgConsole.beginCmdMapIterator(), sgConsole.endCmdMapIterator(), 
	[&](const Console::CmdMap::value_type& v)->void
	{
		mStrings.insert(v.first);
	});
}

//------------------------------------------------------------------
ParameterArgHandler::ParameterArgHandler():
	StringArgHandler("parameter")
	,mDir(0)
{
}

//------------------------------------------------------------------
ParameterArgHandler::ParameterArgHandler(const ParameterArgHandler& rhs):
	StringArgHandler(rhs.getName())
{
	setDir(sgConsole.getDirectory()); 
	StringVector&& sv = mDir->getParameters();
	this->insert(sv.begin(), sv.end());
}

//------------------------------------------------------------------
ValueArgHandler::ValueArgHandler():
	ArgHandler("value")
	,mHandler(0)
	,mDir(0)
{
}

//------------------------------------------------------------------
ValueArgHandler::ValueArgHandler(const ValueArgHandler& rhs):
	ArgHandler(rhs.getName())
	,mHandler(0)
	,mDir(0)
{
	setDir(sgConsole.getDirectory()); 
	NodeArgHandler* valueNode = this->getNode();	
	if(!valueNode)
		PAC_EXCEPT(Exception::ERR_INVALID_STATE, 
				"Do you forget to hook node with arg handler?");
	
	NodeArgHandler* paramNode = valueNode->getAncestorNode("parameter");
	if(!paramNode)
		PAC_EXCEPT(Exception::ERR_INVALID_STATE, 
				"can no find paramNode");

	const std::string& param = paramNode->getValue();
	const std::string& ahName = mDir->getValueArgHandler(param);

	setHandler(sgArgLib.createArgHandler(ahName));
}

//------------------------------------------------------------------
void ValueArgHandler::doPrompt(const std::string& s)
{
	PacAssert(mHandler, "0 handler in value handler");
	return mHandler->prompt(s);
}

//------------------------------------------------------------------
bool ValueArgHandler::doValidate(const std::string& s)
{
	PacAssert(mHandler, "0 handler in value handler");
	return mHandler->validate(s);
}

}

