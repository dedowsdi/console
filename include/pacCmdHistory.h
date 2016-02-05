#ifndef PACROLLSTACK_H
#define PACROLLSTACK_H 

#include "pacConsolePreRequisite.h"

namespace pac
{

class CmdHistory
{
public:

	/**
	 * ctor
	 * mTopIndex will be set to size-1, so the 1st pushed element will at 0
	 * @param size : record size
	 * @param wrapSearch : wrap search at start and end
	 */
	CmdHistory(size_t size = 100, bool rollOver = false);

	/**
	 * push cmdLine into history, it will be trimmed first.
	 * @param cmdLine : cmdLine
	 * @return : 
	 */
	void push(const std::string& cmdLine);

	/**
	 * Get previous command line, decrement search index.
	 * @return : previous command line 
	 */
	const std::string& previous();

	/**
	 * get next element, increment search index.
	 * @return : next element
	 */
	const std::string& next();

	size_t size(){ return mStack.size(); }

private:
	//this must be called every time you push an item 
	void resetRolling();

	/**
	 * @param i : current index
	 * @return : next index in rolling stack
	 */
	size_t getNextRollingIndex(size_t i);

	/**
	 * @param i : current index
	 * @return : prev index in rolling stack
	 */
	size_t getPrevRollingIndex(size_t i);

private:

	bool mRollOver; 	//if set to true, when touch bottom, return first, vice versa
	size_t mSearchIndex; //current  search index
	size_t mTopIndex;  	//current top index
	StringVector mStack;
	static std::string msBlank;
};


}

#endif /* PACROLLSTACK_H */
