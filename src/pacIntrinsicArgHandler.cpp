#include "pacStable.h"
#include "pacIntrinsicArgHandler.h"
#include "pacConsole.h"
#include "pacStringUtil.h"

namespace pac
{

//------------------------------------------------------------------
IntArgHandler::IntArgHandler():
	ArgHandler("int")
{
}

//------------------------------------------------------------------
void IntArgHandler::doPrompt(const String& s)
{
	sgConsole.output("@@int@@");
}

//------------------------------------------------------------------
bool IntArgHandler::doValidate(const String& s)
{
	return StringUtil::isInt(s);
}


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
RealArgHandler::RealArgHandler():
	ArgHandler("Real")
{
}

//------------------------------------------------------------------
void RealArgHandler::doPrompt(const String& s)
{
	sgConsole.output("@@Real@@");
}

//------------------------------------------------------------------
bool RealArgHandler::doValidate(const String& s)
{
	return StringUtil::isNumber(s);
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
{
}

//------------------------------------------------------------------
void PathArgHandler::doPrompt(const String& s)
{
	RaiiConsoleBuffer();
	AbsDir* curDir = sgConsole.getDirectory();

	const String&& head = StringUtil::getHead(s);
	const String&& tail = StringUtil::getTail(s);

	AbsDir* headDir = AbsDirUtil::findPath(curDir, head);

	std::for_each(headDIr->beginChildIter(), headDir->endChildIter() , [&](AbsDir* v)->void
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
	AbsDir* curDir = sgConsole.getDirectory();
	return findPath(curDir, s) != 0;
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

}

