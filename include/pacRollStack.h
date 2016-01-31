#ifndef PACROLLSTACK_H
#define PACROLLSTACK_H 

#include "pacConsolePreRequisite.h"
#include "pacException.h"

namespace pac
{

/**
 * Fixed size container, LIFO. Used to record history like stuff.
 */
template <class T>
class RollStack
{
public:

	/**
	 * ctor
	 * @param size : item size
	 * @param rollOver : whether wrap to start when touch bottom 
	 */
	RollStack(size_t size = 50, bool rollOver = false);

	/**
	 * Push new item into stack. 
	 * @param t : new item
	 */
	void push(const T& t);

	/**
	 * get previous element, decrement search index.
	 * @return : previous element
	 */
	const T& previous();

	/**
	 * get next element, increment search index.
	 * @return : next element
	 */
	const T& next();

	/**
	 * Get top element 
	 * @return : top element
	 */
	const T& top();

	size_t size(){ return mStack.size(); }

	bool isRolling(){ return mIsRolling; }

	//this must be called every time you push an item or finish a query
	void resetRolling();

private:

	bool mRollOver; //if set to true, when touch bottom, return first, vice versa
	bool mIsRolling;
	size_t mSearchIndex; //current  search index
	size_t mTopIndex;  //current top index
	size_t mSize;
	typename std::vector<T> mStack;
};

template <class T>
void RollStack<T>::resetRolling()
{
	if(mStack.size() < mSize)
		mSearchIndex = mTopIndex = mStack.size() - 1;
	else
		mSearchIndex = mTopIndex;

	mIsRolling = false;

}

template <class T>
const T& RollStack<T>::top()
{
	if(mTopIndex >= mStack.size())
		PAC_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "mTopIndex overflow");

	return mStack[mTopIndex];
}

template <class T>
RollStack<T>::RollStack( size_t size, bool rollOver):
	mRollOver(rollOver)
	,mIsRolling(false)
	,mSearchIndex(0)
	,mTopIndex(0)
	,mSize(size)
{

}


template <class T>
const T& RollStack<T>::next()
{
	PacAssert(mIsRolling, "you can not call next, it's not rolling");
	
	//-1 as mStack.size() -1, mStack.size() as 0
	if(mRollOver)
		mSearchIndex = (mSearchIndex + 1) % mStack.size(); 
	else
		mSearchIndex = mSearchIndex == mStack.size() - 1 ? mSearchIndex : mSearchIndex + 1; 

	return mStack[mSearchIndex]; 
}


template <class T>
const T& RollStack<T>::previous()
{
	if(!mIsRolling)
	{
		mIsRolling = true;
	}
	else
	{
		if(mRollOver)
			mSearchIndex = (mSearchIndex - 1 + mStack.size() ) % mStack.size(); 
		else
			mSearchIndex = mSearchIndex == 0 ? mSearchIndex : mSearchIndex - 1;
	}

	return mStack[mSearchIndex]; //careful,  nottop index
}

template <class T>
void RollStack<T>::push( const T& t )
{
	if(mStack.size() < mSize)
	{
		mStack.push_back(t);
	}
	else
	{
		mTopIndex = (mTopIndex + 1) % mSize;
		mStack[mTopIndex] = t;
	}
}

}

#endif /* PACROLLSTACK_H */
