#include "pacCommand.h"
#include "pacArgHandler.h"
#include <boost/regex.hpp>

namespace pac
{

//------------------------------------------------------------------
Command::Command(const String& name)
	:mName(name)
	 ,mArgHandler(0)
{
	//change this in subclass ctor if you want to use existing arg handler 
	mAhName = getDefAhName();
}

//------------------------------------------------------------------
Command::Command(const Command&rhs)
	:mName(rhs.getName())
	 ,mAhName(rhs.getName())
	 ,mArgHandler(0)
{

}

//------------------------------------------------------------------
Command::~Command()
{
}

//------------------------------------------------------------------
Command* Command::init()
{
	buildArgHandler();
	sgArgLib.registerArgHandler(mArgHandler);

	return this;
}

//------------------------------------------------------------------
void Command::prompt()
{
	getArgHandler()->prompt(mArgs);
}

//------------------------------------------------------------------
bool Command::execute()
{
	if (getArgHandler()->validate(mArgs)) 
	{
		return this->doExecute();
	}
	else
	{
		getArgHandler()->outputErrMessage(mArgs);
		return false;
	}
}

//------------------------------------------------------------------
void Command::setArgsAndOptions( const String& v)
{
	//extract options
	boost::regex reOptions("-(\w+)");
	//word not after -, plus surround space
	boost::regex reArgs("\s*[^-]?\<\w+\>\s*"); 

	boost::smatch m;
	//get options
	String::const_iterator start  = v.begin();
	String::const_iterator end = v.end();
	while(boost::regex_search(start, end, m, reOptions))
	{
		mOptions += m[1];
		start = m[0].second;
	}

	//get args
	start  = v.begin();
	while(boost::regex_search(start, end, m, reArgs))
	{
		mArgs += m[1];
		start = m[0].second;
	}
}

//------------------------------------------------------------------
ArgHandler* Command::getArgHandler()
{
	if(mArgHandler == 0)
		mArgHandler = sgArgLib.createArgHandler(mAhName);

	return mArgHandler;
}


}

