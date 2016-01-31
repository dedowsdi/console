#include "pacStable.h"
#include "pacConsole.h"
#include "pacCommand.h"
#include "pacArgHandler.h"
#include "pacIntrinsicCmd.h"
#include "pacUiConsole.h"
#include "pacAbsDir.h"
#include <boost/regex.hpp>

namespace pac
{

template<>Console* Singleton<Console>::msSingleton = 0;

//------------------------------------------------------------------
Console::Console(UiConsole* ui) :
	mDir(0)
	,mUi(ui)
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
	mUi->outputLine(s, type);
	return *this;
}

//------------------------------------------------------------------
Console& Console::endl(int type)
{
	mUi->endl(type);
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
	//check if it's already registerd	
	CmdMap::iterator iter = std::find_if( mCmdMap.begin(), mCmdMap.end(),
			[&](CmdMap::value_type& v)->bool
			{
			return v.first == cmdProto->getName();
			});

	if (iter != mCmdMap.end()) 
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

	Real textwidth = static_cast<Real>(mUi->getTextWidth());
	//sort buffer
	std::sort(mBuffer.begin(), mBuffer.end());
	size_t numCol, totalColWidth;
	IntVector colWidthes;
	getColumnNumber(numCol, totalColWidth, colWidthes);

	//determine spacing
	size_t spacing = (textwidth  - totalColWidth) / (numCol - 1);
	size_t colSize = static_cast<int>(mBuffer.size() / numCol);

	//output horizental. Becareful, item seq in mBuffers is vertical.
	for (size_t i = 0; i < mBuffer.size(); ++i) 
	{
		size_t col = i % numCol;	
		size_t row = i / numCol;
		size_t colWidth = colWidthes[col] + spacing;
		size_t itemIndex = col * colSize  + row; 
		mUi->outputNoAutoWrap(StringUtil::toFixedLength(mBuffer[itemIndex], colWidth));
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
		mUi->updateCommandLine(mCmdHistory.previous());
	}
	else
	{
		if(mCmdHistory.isRolling())
		mUi->updateCommandLine(mCmdHistory.next());
	}
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
	mCmdHistory.push(cmdLine);
}

//------------------------------------------------------------------
void Console::appendBuffer( const std::string& v)
{
	mBuffer.push_back(v);
}

//------------------------------------------------------------------
int Console::getColumnWidth(StringVector::iterator beg, StringVector::iterator end)
{
	size_t l = 0;
	std::for_each(beg, end, [&](const std::string& v)->void
	{
		if(v.length() > l)	
			l = v.length();
	});

	return l;
}

//------------------------------------------------------------------
void Console::getColumnNumber(size_t &numCol, size_t &totalColWidth, IntVector& colWidthes)
{
	//find max length item, use it to determine least column number
	int maxItemWidth = getColumnWidth(mBuffer.begin(), mBuffer.end());

	Real textwidth = static_cast<Real>(mUi->getTextWidth());
	numCol = ceil(textwidth / (maxItemWidth + 1));	//maxItemWidth + 1 spacing

	if(numCol >= mBuffer.size()) //one line is enough 
	{
		numCol = mBuffer.size();
		std::for_each(mBuffer.begin(), mBuffer.end(), [&](const std::string& v)->void
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
			size_t colSize = static_cast<int>(mBuffer.size() / numCol);
			size_t _totalColWidth = 0;
			IntVector _colWidthes;
			//loop column, accumulate length of each column 
			for (size_t i = 0; i < numCol; ++i) 
			{
				StringVector::iterator beg = mBuffer.begin() + colSize * i;
				StringVector::iterator end = i == numCol - 1 ?
					mBuffer.end() : mBuffer.begin() + colSize * (i + 1);
				
				size_t w = getColumnWidth(beg, end);
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

