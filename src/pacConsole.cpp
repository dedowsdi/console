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

	Real textwidth = static_cast<Real>(()mUi->getTextWidth());

	//sort buffer
	std::sort(mBuffer.begin(), mBuffer.end());
	int numCol, totalColWidth;
	IntVector colWidthes;
	getColumnNumber(numCol, totalColWidth, colWidthes);

	//determine spacing
	int spacing = (textwidth  - totalColWidth) / (numCol - 1);
	int colSize = static_cast<int>(mBuffer.size() / numCol);

	//output horizental. Becareful, item seq in mBuffers is vertical.
	for (int i = 0; i < mBuffer.size(); ++i) 
	{
		int col = i % numCol;	
		int row = i / numCol;
		int colWidth = colWidthes[col] + spacing;
		int itemIndex = col * colSize  + row; 
		mUi->outputNoAutoWrap(StringUtil::toFixedLength(mBuffer[itemIndex]), colWidth);
		//line break at last column
		if(col == numCol - 1)	
			mUi->endl();
	}

	mBuffer.clear();
}

//------------------------------------------------------------------
void Console::rollCommand(bool backWard /*= true*/)
{
	if(mCmdHistory.size() == 0)
		return;

	if(backWard)
	{
		this->updateCommand(mCmdHistory.previous());
	}
	else
	{
		if(mCmdHistory.isRolling())
		this->updateCommand(mCmdHistory.next());
	}
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
	mCmdHistory.push(cmdLine);
}

//------------------------------------------------------------------
void Console::appendBuffer( const String& v)
{
	mBuffer.push_back(v);
}

//------------------------------------------------------------------
int Console::getColumnWidth(StringVector::iterator beg, StringVector::iterator end)
{
	int l = 0;
	std::for_each(beg, end, [&](const String& v)->void
	{
		if(v.length() > l)	
			l = v.length()
	});

	return l;
}

//------------------------------------------------------------------
void Console::getColumnNumber(int &numCol, int &totalColWidth, IntVector& colWidthes)
{
	//find max length item, use it to determine least column number
	int maxItemWidth = getColumnWidth(mBuffer.begin(), mBuffer.end());
	numCol = ceil(textwidth / (maxItemWidth + 1);	//maxItemWidth + 1 spacing

	if(numCol >= mBuffer.size()) //one line is enough 
	{
		numCol = mBuffer.size();
		std::for_each(mBuffer.begin(), mBuffer.end(), [&](const String& v)->void
		{
			totalColWidth += v.size();	
			colWidthes.push_back(v.size());
		});
	}
	else
	{
		//loop to find max numCol 
		while(true)
		{
			++numCol;
			int colSize = static_cast<int>(mBuffer.size() / numCol);
			int _totalColWidth = 0;
			IntVector _colWidthes;
			//loop column, accumulate length of each column 
			for (i = 0; i < numCol; ++i) 
			{
				StringVector::iterator beg = mBuffer.begin() + colSize * i;
				StringVector::iterator end = i == numCol - 1 ?
					mBuffer.end() : mBuffer.begin() + colSize * (i + 1);
				
				int w = getColumnWidth(beg, end);
				_colWidthes.push_back(w);
				_totalColWidth += w;
			}

			if(_totalColWidth + numCol > textwidth)	//overflow check
			{
				--numCol;
				colWidthes.assign(_colWidthes.begin(), _colWidthes.end());
				totalColWidth = _totalColWidth;
				break;
			}
		}
	}
}

//------------------------------------------------------------------
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

