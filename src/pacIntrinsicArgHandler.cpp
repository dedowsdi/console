#include "pacStable.h"
#include "pacIntrinsicArgHandler.h"
#include "pacAbsDir.h"

namespace pac
{

//------------------------------------------------------------------
BoolArgHandler::BoolArgHandler():
	ArgHandler("bool")
{
}

//------------------------------------------------------------------
void BoolArgHandler::doPrompt(const String& s)
{
	sgConsole.output("@@bool@@");
}

//------------------------------------------------------------------
bool BoolArgHandler::doValidate(const String& s)
{
	return s == "true" || s == "false";
}

//------------------------------------------------------------------
StringArgHandler::StringArgHandler(const String& name):
	ArgHandler(name)
{
}

//------------------------------------------------------------------
StringArgHandler::StringArgHandler(const String& name, const String& text):
	ArgHandler(name)
{
	mStrings.insert(text);
}


//------------------------------------------------------------------
StringArgHandler::StringArgHandler():
	ArgHandler("unnamed_string")
{
}

//------------------------------------------------------------------
StringArgHandler& StringArgHandler::insert(const String& s)
{
	mStrings.insert(s);
}

//------------------------------------------------------------------
void StringArgHandler::remove(const String& s)
{
	mStrings.erase(s);
}

//------------------------------------------------------------------
const String& StringArgHandler::get(StringVector::size_type i)
{
	return mStrings[i];
}

//------------------------------------------------------------------
void StringArgHandler::doPrompt(const String& s)
{
	RaiiConsoleBuffer();
	std::for_each(mStrings.begin(), mStrings.end(), [&](const String& v)->void
	{
		if(StringUtil::startsWith(s, v))
		{
			sgConsole.output(v);
		}
	});	
}

//------------------------------------------------------------------
bool StringArgHandler::doValidate(const String& s)
{
	return exists(s);
}

//------------------------------------------------------------------
bool StringArgHandler::exist(const String& value)
{
	return mStrings.find(value) != mStrings.end();
}


//------------------------------------------------------------------
BlankArgHandler::BlankArgHandler()
	:ArgHandler("blank")
{
}

//------------------------------------------------------------------
void BlankArgHandler::doPrompt(const String& s)
{
	"@@blank@@"
}

//------------------------------------------------------------------
bool BlankArgHandler::doValidate(const String& s)
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
	ArgHandler(rhs->getName())
{
	setDir(sgConsole.getDirectory());
}

//------------------------------------------------------------------
void PathArgHandler::doPrompt(const String& s)
{
	RaiiConsoleBuffer();

	const String&& head = StringUtil::getHead(s);
	const String&& tail = StringUtil::getTail(s);

	AbsDir* headDir = AbsDirUtil::findPath(mDir, head);

	std::for_each(headDir->beginChildIter(), headDir->endChildIter() , [&](AbsDir* v)->void
	{
		if (StringUtil::startsWith(v->getName(), s)) 
		{
			sgConsole.output(v->getName());
		}	
	});
}

//------------------------------------------------------------------
bool PathArgHandler::doValidate(const String& s)
{
	return findPath(mDir, s) != 0;
}


//------------------------------------------------------------------
CmdArgHandler::CmdArgHandler():
	StringHandler("cmd")
{
	std::for_each(sgConsole.beginCmdMapIterator(), sgConsole.endCmdMapIterator(), 
	[&](const String& s)->void
	{
		mStrings.insert(s);
	});
}

//------------------------------------------------------------------
ParameterArgHandler::ParameterArgHandler():
	ArgHandler("parameter")
	,mDir(0)
{
}

//------------------------------------------------------------------
ParameterArgHandler::ParameterArgHandler(const ParameterArgHandler& rhs):
	ArgHandler(rhs->getName())
{
	setDir(sgConsole.getDirectory()); 
	StringVector&& sv = mDir->getParameters();
	this->insert(sv.begin(), sv.end());
}

//------------------------------------------------------------------
ValueArgHandler::ValueArgHandler():
	ArgHandler("value")
	,mDir(0)
	,mHandler(0)
{
}

//------------------------------------------------------------------
ValueArgHandler::ValueArgHandler(const ValueArgHandler& rhs):
	ArgHandler(rhs->getName())
	,mDir(0)
	,mHandler(0)
{
	setDir(sgConsole.getDirectory()); 
	NodeArgHandler* valueNode = this->getNode();	
	PacAssert(valueNode, Exception::ERR_INVALID_STATE, 
			"Do you forget to hook node with arg handler?", __FUNCTION__)
	
	NodeArgHandler* paramNode = valueNode->getParentNode("parameter");
	PacAssert(paramNode, Exception::ERR_INVALID_STATE, 
			"can no find paramNode", __FUNCTION__)

	const String& param = paramNode->getValue();
	const String& ahName = mDir->getValueArgHandler(param);

	setHandler(sgArgLib.createArgHandler(ahName));
}

//------------------------------------------------------------------
void ValueArgHandler::doPrompt(const String& s)
{
	PacAssert(mHandler, Exception::ERR_INVALID_STATE,
			"0 handler in value handler", __FUNCTION__)
	return mHandler->prompt(s);
}

//------------------------------------------------------------------
bool ValueArgHandler::doValidate(const String& s)
{
	PacAssert(mHandler, Exception::ERR_INVALID_STATE,
			"0 handler in value handler", __FUNCTION__)
	return mHandler->validate(s);
}

}

