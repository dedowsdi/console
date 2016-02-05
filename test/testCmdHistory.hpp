#ifndef TESTCMDHISTORY_H
#define TESTCMDHISTORY_H 

#include "pacCmdHistory.h"
#include "pacStringUtil.h"
#include <gtest/gtest.h>

using namespace pac;

/**
 * CmdHistory fixture, push 0 - size-1 into it 
 */
class TestCmdHistory: public ::testing::Test
{
protected:
	TestCmdHistory()
	{
		mHistory = new CmdHistory();
		for (size_t i = 0; i < mHistory->size(); ++i) 
		{
			mHistory->push(StringUtil::toString(i));
		}
	}

	~TestCmdHistory()
	{
		delete mHistory;
	}

	CmdHistory* mHistory;
};

TEST_F(TestCmdHistory, push2size)
{
	ASSERT_EQ(100, mHistory->size());
	size_t numItem = 2 * mHistory->size();
	for (size_t i = 0; i < numItem; ++i) 
	{
		mHistory->push(StringUtil::toString(i));
	}
	ASSERT_EQ(100, mHistory->size());
}

TEST_F(TestCmdHistory, previous)
{
	for (int i = mHistory->size() - 1; i >= 0;  --i) 
	{
		ASSERT_EQ(StringUtil::toString(i), mHistory->previous());
	}

	//previous should always return bottom item if rollover is false and it
	//reached bottom
	for (int i = 0; i < 10; ++i) 
	{
		ASSERT_STREQ("0", mHistory->previous().c_str());
	}
}

TEST_F(TestCmdHistory, next)
{
	for (int i = mHistory->size() - 1; i >= 0;  --i) 
	{
		ASSERT_EQ(StringUtil::toString(i), mHistory->previous());
	}

	for (size_t i = 1; i < mHistory->size(); ++i) 
	{
		ASSERT_EQ(StringUtil::toString(i), mHistory->next());
	}
	
	//previous should always return  blank if rollover is false and it
	//reached top
	for (int i = 0; i < 10; ++i) 
	{
		ASSERT_STREQ("", mHistory->next().c_str());
	}
}

#endif /* TESTCMDHISTORY_H */
