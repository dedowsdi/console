#include "pacStable.h"
#include "pacConsole.h"
#include "pacCommand.h"
#include "pacArgHandler.h"
#include "pacIntrinsicCmd.h"
#include <boost/regex.hpp>


namespace pac
{

Console* Singleton<Console>::msSingleton = 0;

//------------------------------------------------------------------
Console::Console(UiConsole* ui)
	:mUi(ui)
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
}

//------------------------------------------------------------------
bool Console::execute(const String& cmdLine)
{
	fakeOutputDirAndCmd(cmdLine);
	addCmdLineToHistory(cmdLine);

	//clear cmd line
	mUi->updateCommandLine();

	boost::regex reCmd2("^\s*(\w+)\s*(.*)$");
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
void Console::prompt(const String& cmdLine)
{
	fakeOutputDirAndCmd();

	boost::regex reCmd("^\s*(\w*)$");
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
		boost::regex reCmd2("^\s*(\w+)\s*(.*)$");
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
					+ cmdLine, __FUNCTION__);
		}
	}
}

//------------------------------------------------------------------
Console& Console::output(const String& s, int type /*= 1*/)
{
	if(type == 1 && mIsBuffering)
		this->appendBuffer(s);
	else
		mUi->output(s, type);

	return *this;
}

//------------------------------------------------------------------
Console& Console::outputLine(const String& s, int type /*= 1*/)
{
	mUi->outputLine(s, type)
	return *this;
}

//------------------------------------------------------------------
Console& Console::endl(int type = 1)
{
	mUi->endl(type);
	return *this;
}

//------------------------------------------------------------------
Console& Console::complete(const String& s)
{
	mUi->complete(s);
}

//------------------------------------------------------------------
void Console::changeDirectory(AbsDir* dir)
{
	mDir = dir;
}

//------------------------------------------------------------------
void Console::registerCommand(Command* cmdProto)
{
	//check if it's already registerd	
	CmdMap::iterator iter = std::find_if( mCmdMap.begin(), mCmdMap.end(),
			[&](CmdMap::value_type& v)->bool
			{
			return v.first == cmdProto->getName();
			});

	if (iter != mCmdMap.end()) 
	{
		mCmdMap[cmdProto->getCmdName()]	= cmdProto;
	}
	else
	{
		PAC_EXCEPT(Exception::ERR_DUPLICATE_ITEM, cmdproto->getName() + " already registed!");
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
	//@TODO implement
	throw new std::runtime_error("unimplemented function called");
}

//------------------------------------------------------------------
CmdMap::const_iterator Console::beginCmdMapIterator() const
{
	return mCmdMap.begin();
}

//------------------------------------------------------------------
CmdMap::const_iterator Console::endCmdMapIterator() const
{
	return mCmdMap.end();
}

//------------------------------------------------------------------
Command* Console::createCommand(const String& cmdName)
{
	CmdMap::iterator iter = mCmdMap.find(cmdName);
	if(iter != mCmdMap.end())
	{
		return iter->second->clone();
	}
	else
	{
		PAC_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, cmdName + " not found", __FUNCTION__);
	}
}


//------------------------------------------------------------------
void Console::promptCommandName(const String& cmdName)
{
	ArgHandler* handler = sgArgLib.createArgHandler("cmd");
	handler->prompt(cmdName);
	delete handler;
}

//------------------------------------------------------------------
void Console::fakeOutputDirAndCmd(const String& cmdLine)
{
	output(mDir->getFullPath() + ":" + cmdLine);
}

//------------------------------------------------------------------
void Console::addCmdLineToHistory(const String& cmdLine)
{
	//@TODO implement
	throw new std::runtime_error("unimplemented function called");
}

//------------------------------------------------------------------
RaiiConsoleBuffer::RaiiConsoleBuffer()
{
	PacAssert(sgConsole().getBuffer().empty(), "Do you forget to flash buffer?");
	sgConsole.startBuffer();
}

//------------------------------------------------------------------
RaiiConsoleBuffer::~RaiiConsoleBuffer()	
{
	sgConsole.endBuffer();
}

}

