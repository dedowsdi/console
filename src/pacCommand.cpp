#include "pacCommand.h"
#include "pacArgHandler.h"
#include "pacException.h"
#include <boost/regex.hpp>

namespace pac
{

//------------------------------------------------------------------------------
Command::Command(const std::string& name)
	:mName(name)
	 ,mArgHandler(0)
{
	//change this in subclass ctor if you want to use existing arg handler 
	mAhName = getDefAhName();
}

//------------------------------------------------------------------------------
Command::Command(const Command&rhs)
	:mName(rhs.getName())
	 ,mAhName(rhs.getName())
	 ,mArgHandler(0)
{

}

//------------------------------------------------------------------------------
Command::~Command()
{
	PacAssert(mArgHandler != 0, "0 arg handler");
	delete mArgHandler;
	mArgHandler = 0;
}

//------------------------------------------------------------------------------
Command* Command::init()
{
	buildArgHandler();
	if (mArgHandler->getName() == getDefAhName()) 
		sgArgLib.registerArgHandler(mArgHandler);

	return this;
}

//------------------------------------------------------------------------------
void Command::prompt()
{
	getArgHandler()->prompt(mArgs);
}

//------------------------------------------------------------------------------
bool Command::execute()
{
	if (getArgHandler()->validate(mArgs)) 
	{
		bool res = this->doExecute();
		return res;
	}
	else
	{
		getArgHandler()->outputErrMessage(mArgs);
		return false;
	}
}

//------------------------------------------------------------------------------
void Command::setArgsAndOptions( const std::string& v)
{
	//extract options
	boost::regex reOptions("-(\\w+)");
	//word not after -, plus surround space
	boost::regex reArgs("\\s*[^-]?\\<\\w+\\>\\s*"); 

	boost::smatch m;
	//get options
	std::string::const_iterator start  = v.begin();
	std::string::const_iterator end = v.end();
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

//------------------------------------------------------------------------------
ArgHandler* Command::getArgHandler()
{
	if(mArgHandler == 0)
		mArgHandler = sgArgLib.createArgHandler(mAhName);

	return mArgHandler;
}


}

