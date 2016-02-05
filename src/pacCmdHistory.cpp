#include "pacCmdHistory.h"
#include "pacStringUtil.h"
#include "pacException.h"

namespace pac
{
std::string  CmdHistory::msBlank;

//------------------------------------------------------------------
CmdHistory::CmdHistory(size_t size /*= 100*/, bool rollOver /*= false*/):
	mRollOver(rollOver)
	,mSearchIndex(-1)
	,mTopIndex(size - 1)
{
	mStack.resize(size);
}

//------------------------------------------------------------------
void CmdHistory::push(const std::string& cmdLine)
{
	std::string l(cmdLine);
	StringUtil::trim(l);

	if(l.empty())
		PAC_EXCEPT(Exception::ERR_INVALIDPARAMS, "empty cmdline!");

	mTopIndex = getNextRollingIndex(mTopIndex);
	mStack[mTopIndex] = l;

	resetRolling();
}

//------------------------------------------------------------------
const std::string& CmdHistory::previous()
{
	if(mSearchIndex == static_cast<size_t>(-1))
	{
		//init rolling
		mSearchIndex = mTopIndex; 
		return mStack[mSearchIndex];
	}
	else
	{
		size_t searchIndex = getPrevRollingIndex(mSearchIndex);
		if (mRollOver || searchIndex != mTopIndex) //check roll over
			mSearchIndex = searchIndex;
		
		return mStack[mSearchIndex];
	}
}

//------------------------------------------------------------------
const std::string& CmdHistory::next()
{
	static std::string BLANK;
	if(mSearchIndex == static_cast<size_t>(-1) || mSearchIndex == mTopIndex)
	{
		return msBlank;
	}
	else
	{
		mSearchIndex = getNextRollingIndex(mSearchIndex);
		return mStack[mSearchIndex];
	}
}

//------------------------------------------------------------------
void CmdHistory::resetRolling()
{
	mSearchIndex = -1;
}

//------------------------------------------------------------------
size_t CmdHistory::getNextRollingIndex(size_t i)
{ 
	return (i + mStack.size() + 1) % mStack.size(); 
}

//------------------------------------------------------------------
size_t CmdHistory::getPrevRollingIndex(size_t i)
{ 
	return (i + mStack.size() - 1) % mStack.size(); 
}

}
