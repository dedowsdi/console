#include "pacStable.h"
#include "pacConsole.h"
#include "pacCommand.h"
#include "pacArgHandler.h"
#include "pacIntrinsicCmd.h"
#include "pacUiConsole.h"
#include "pacAbsDir.h"
#include "pacConsolePattern.h"
#include "pacCmdHistory.h"
#include <boost/regex.hpp>

namespace pac
{

template<>Console* Singleton<Console>::msSingleton = 0;

//------------------------------------------------------------------
Console::Console(UiConsole* ui) :
	mDir(0)
	,mUi(ui)
	,mPattern(0)
	,mCmdHistory(0)
{
}

//------------------------------------------------------------------
Console::~Console()
{
	//clean command
	std::for_each(mCmdMap.begin(), mCmdMap.end(), [&](CmdMap::value_type& v)->void
	{
		delete v.second;	
	});
	mCmdMap.clear();

	delete mCmdHistory;
	mCmdHistory = 0;

	//clean arg lib
	delete &sgArgLib;
}

//------------------------------------------------------------------
void Console::init()
{
	//register intrinsic arg handler
	new ArgHandlerLib();

	sgArgLib.init();

	//register intrinsic commands
	this->registerCommand((new LsCmd())->init());
	this->registerCommand((new PwdCmd())->init());
	this->registerCommand((new CdCmd())->init());
	this->registerCommand((new SetCmd())->init());
	this->registerCommand((new LpCmd())->init());

	initConoslePattern();
}

//------------------------------------------------------------------
void Console::initConoslePattern()
{
	this->mPattern = new DefaultPattern(mUi->getTextWidth());
}

//------------------------------------------------------------------
void Console::initCmdPattern()
{
	mCmdHistory = new CmdHistory();
}

//------------------------------------------------------------------
bool Console::execute(const std::string& cmdLine)
{
	fakeOutputDirAndCmd(cmdLine);
	addCmdLineToHistory(cmdLine);

	//clear cmd line
	mUi->updateCommandLine();

	boost::regex reCmd2("^\\s*(\\w+)\\s*(.*)$");
	boost::smatch m;
	if(boost::regex_match(cmdLine, m, reCmd2))
	{
		Command* cmd = this->createCommand(m[1]);
		if(cmd)
		{
			cmd->setArgsAndOptions(m[2]);
			return cmd->execute();
		}
	}
	return false;
}

//------------------------------------------------------------------
void Console::prompt(const std::string& cmdLine)
{
	fakeOutputDirAndCmd(cmdLine);

	boost::regex reCmd("^\\s*(\\w*)$");
	boost::smatch m;
	if(boost::regex_match(cmdLine, m, reCmd))	
	{
		//prompt command name
		this->promptCommandName(m[1]);
	}
	else
	{
		//prompt argument
		//extract command name, args and options
		boost::regex reCmd2("^\\s*(\\w+)\\s*(.*)$");
		if(boost::regex_match(cmdLine, m, reCmd2))
		{
			Command* cmd = this->createCommand(m[1]);
			if(cmd)
			{
				cmd->setArgsAndOptions(m[2]);
				cmd->prompt();
			}
		}
		else
		{
			PAC_EXCEPT(Exception::ERR_INVALID_STATE, "invalid state caused by command line :"
					+ cmdLine);
		}
	}
}

//------------------------------------------------------------------
Console& Console::output(const std::string& s, int type /*= 1*/)
{
	if(type == 1 && mIsBuffering)
		this->appendBuffer(s);
	else
		mUi->output(s, type);

	return *this;
}

//------------------------------------------------------------------
Console& Console::outputLine(const std::string& s, int type /*= 1*/)
{
	if(mIsBuffering)
		PAC_EXCEPT(Exception::ERR_INVALID_STATE, "Can not output line while buffering.");
	mUi->outputLine(s, type);
	return *this;
}


//------------------------------------------------------------------
Console& Console::complete(const std::string& s)
{
	mUi->complete(s);
	return *this;
}

//------------------------------------------------------------------
void Console::changeDirectory(AbsDir* dir)
{
	mDir = dir;
}

//------------------------------------------------------------------
void Console::registerCommand(Command* cmdProto)
{
	PacAssert(!cmdProto->getName().empty(), "empty cmd name");
	std::cout << "register command " + cmdProto->getName() << std::endl;
	//check if it's already registerd	
	CmdMap::iterator iter = std::find_if( mCmdMap.begin(), mCmdMap.end(),
			[&](CmdMap::value_type& v)->bool
			{
			return v.first == cmdProto->getName();
			});

	if (iter == mCmdMap.end()) 
	{
		mCmdMap[cmdProto->getName()]	= cmdProto;
	}
	else
	{
		PAC_EXCEPT(Exception::ERR_DUPLICATE_ITEM, cmdProto->getName() + " already registed!");
	}
}

//------------------------------------------------------------------
void Console::startBuffer()
{
	PacAssert(!mIsBuffering, "It'w wrong to start buffer twice");
	PacAssert(!mBuffer.empty(), "It'w wrong to start buffer when buffer is not empty");

	mIsBuffering = true;
}

//------------------------------------------------------------------
void Console::endBuffer()
{
	PacAssert(mIsBuffering, "It'w wrong to end buffer without start it");
	mIsBuffering = false;
	if(!mBuffer.empty())
		mUi->outputNoAutoWrap(mPattern->applyPattern(mBuffer.begin(), mBuffer.end()));

	mBuffer.clear();
}

//------------------------------------------------------------------
void Console::rollCommand(bool backWard /*= true*/)
{
	if(backWard)
		mUi->updateCommandLine(mCmdHistory->previous());
	else
		mUi->updateCommandLine(mCmdHistory->next());
}

//------------------------------------------------------------------
Console::CmdMap::const_iterator Console::beginCmdMapIterator() const
{
	return mCmdMap.begin();
}

//------------------------------------------------------------------
Console::CmdMap::const_iterator Console::endCmdMapIterator() const
{
	return mCmdMap.end();
}

//------------------------------------------------------------------
Command* Console::createCommand(const std::string& cmdName)
{
	CmdMap::iterator iter = mCmdMap.find(cmdName);
	if(iter != mCmdMap.end())
	{
		return iter->second->clone();
	}
	else
	{
		PAC_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, cmdName + " not found");
	}
}


//------------------------------------------------------------------
void Console::promptCommandName(const std::string& cmdName)
{
	ArgHandler* handler = sgArgLib.createArgHandler("cmd");
	handler->prompt(cmdName);
	delete handler;
}

//------------------------------------------------------------------
void Console::fakeOutputDirAndCmd(const std::string& cmdLine)
{
	output(mDir->getFullPath() + ":" + cmdLine);
}

//------------------------------------------------------------------
void Console::addCmdLineToHistory(const std::string& cmdLine)
{
	mCmdHistory->push(cmdLine);
}

//------------------------------------------------------------------
void Console::appendBuffer( const std::string& v)
{
	mBuffer.push_back(v);
}

//------------------------------------------ be------------------------
RaiiConsoleBuffer::RaiiConsoleBuffer()
{
	sgConsole.startBuffer();
}

//------------------------------------------------------------------
RaiiConsoleBuffer::~RaiiConsoleBuffer()	
{
	sgConsole.endBuffer();
}

}

