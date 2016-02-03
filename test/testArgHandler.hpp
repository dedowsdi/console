#ifndef TESTPACARGHANDLER_H
#define TESTPACARGHANDLER_H 

#include "pacArgHandler.h"
#include "pacStringUtil.h"
#include <limits>
#include <gtest/gtest.h>

using namespace pac;

//test primitive type and array type
class TestArgHandlerLib : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		mBoolTrue = "true";
		mBoolFalse = "false";
		mShortMax = StringUtil::toString(std::numeric_limits<short>::max()); 
		mUshortMax = StringUtil::toString(std::numeric_limits<unsigned short>::max()); 
		mIntMax = StringUtil::toString(std::numeric_limits<int>::max()); 
		mUintMax = StringUtil::toString(std::numeric_limits<unsigned int>::max()); 
		mLongMax = StringUtil::toString(std::numeric_limits<long>::max()); 
		mUlongMax = StringUtil::toString(std::numeric_limits<unsigned long>::max()); 
		mShortMin = StringUtil::toString(std::numeric_limits<short>::min()); 
		mUshortMin = StringUtil::toString(std::numeric_limits<unsigned short>::min()); 
		mIntMin = StringUtil::toString(std::numeric_limits<int>::min()); 
		mUintMin = StringUtil::toString(std::numeric_limits<unsigned int>::min()); 
		mLongMin = StringUtil::toString(std::numeric_limits<long>::min()); 
		mUlongMin = StringUtil::toString(std::numeric_limits<unsigned long>::min()); 
		mReal =  StringUtil::toString(3.1415926);
		mNreal = StringUtil::toString(0.999);
		mInt2 = "1 2";
		mInt3 = "1 2 3";
		mInt4 = "1 2 3 4";
		mInt5 = "1 2 3 4 5";
		mReal2 = "1.02 2.03";
		mReal3 = "1.02 2.03 3.03";
		mReal4 = "1.02 2.03 3.03 4.04";
		mReal5 = "1.02 2.03 3.03 4.04 5.05";
		mNreal2 = "0.02 0.93";
		mNreal3 = "0.82 0.03 -0.03";
		mNreal4 = "0.02 0.53 0.03 -0.04";
		mNreal5 = "0.02 0.83 0.63 0.94 -0.85";
		mMatrix2 = "2.02 -3.03 4.04 5.08";
		mMatrix3 = mMatrix2 + " " +  mMatrix2 + " -9.09";
		mMatrix4 = mMatrix3 + " " +  mReal5 + " " +  mReal2;
		mNmatrix2 = "0.82 -0.73 0.04 -0.08";
		mNmatrix3 = mNmatrix2 + " " +  mNmatrix2 + " -0.99";
		mNmatrix4 = mNmatrix3 + " " + mNreal5 + " "  + mNreal2;
		mBlank = "";
		mCmdCd = "cd";
		mCmdLp = "lp";
		mCmdLs = "ls";
		mCmdPwd = "pwd";
		mCmdSet = "set";

		mMap["boolTrue"]  = mBoolTrue;
		mMap["boolFalse"] = mBoolFalse;
		mMap["shortMax"]  = mShortMax;
		mMap["ushortMax"] = mUshortMax;
		mMap["intMax"]    = mIntMax;
		mMap["uintMax"]   = mUintMax;
		mMap["longMax"]   = mLongMax;
		mMap["ulongMax"]  = mUlongMax;
		mMap["shortMin"]  = mShortMin;
		mMap["ushortMin"] = mUshortMin;
		mMap["intMin"]    = mIntMin;
		mMap["uintMin"]   = mUintMin;
		mMap["longMin"]   = mLongMin;
		mMap["ulongMin"]  = mUlongMin;
		mMap["real"]      = mReal;
		mMap["nreal"]     = mNreal;
		mMap["int2"]      = mInt2;
		mMap["int3"]      = mInt3;
		mMap["int4"]      = mInt4;
		mMap["int5"]      = mInt5;
		mMap["real2"]     = mReal2;
		mMap["real3"]     = mReal3;
		mMap["real4"]     = mReal4;
		mMap["real5"]     = mReal5;
		mMap["nreal2"]    = mNreal2;
		mMap["nreal3"]    = mNreal3;
		mMap["nreal4"]    = mNreal4;
		mMap["nreal5"]    = mNreal5;
		mMap["matrix2"]   = mMatrix2;
		mMap["matrix3"]   = mMatrix3;
		mMap["matrix4"]   = mMatrix4;
		mMap["nmatrix2"]  = mNmatrix2;
		mMap["nmatrix3"]  = mNmatrix3;
		mMap["nmatrix4"]  = mNmatrix4;
		mMap["blank"]     = "";
		mMap["cmdCd"]     = mCmdCd;
		mMap["cmdLp"]     = mCmdLp;
		mMap["cmdLs"]     = mCmdLs;
		mMap["cmdPwd"]    = mCmdPwd;
		mMap["cmdSet"]    = mCmdSet;
	}

	virtual void TearDown()
	{
	
	}

	/**
	 * return true if should use EXCEPT_TRUE
	 * @param beg : begin item
	 * @param end : end item
	 * @param maskPass : mask pass or fail
	 * @param itemName : key name in mMap
	 * @return : 
	 */
	template<typename _InputIterator>
	bool ifUseTrue(const std::string& itemName, _InputIterator beg, 
			_InputIterator end, bool maskPass)
	{
		_InputIterator iter = std::find(beg, end, itemName);
		return (maskPass && iter != end) || (!maskPass && iter == end);
	}

	/**
	 * Only item between beg and end should pass or fail
	 * @param beg : begin item 
	 * @param end : end item 
	 * @param maskPass : mask pass or fail.
	 */
	template<typename _InputIterator>
		void test(const std::string& ahName, _InputIterator beg,
				_InputIterator end, bool maskPass = true)
		{
			//it's expanded on purpose, it's clear to find out which item is
			//wrong this way.

			ArgHandler* handler = sgArgLib.createArgHandler(ahName);
			std::string itemName;

			itemName = "boolTrue";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "boolFalse";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "shortMax";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "ushortMax";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "intMax";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "uintMax";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "longMax";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "ulongMax";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "shortMin";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "ushortMin";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "intMin";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "uintMin";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "longMin";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "ulongMin";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "real"; //make it not nreal
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "nreal";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "int2";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "int3";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "int4";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "int5";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "real2";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "real3";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "real4";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "real5";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "nreal2";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "nreal3";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "nreal4";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "nreal5";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "matrix2";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "matrix3";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "matrix4";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "nmatrix2";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "nmatrix3";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "blank";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "cmdCd";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "cmdLs";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "cmdLp";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "cmdPwd";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));
			itemName = "cmdSet";
			if(ifUseTrue(itemName, beg, end, maskPass))
				EXPECT_TRUE(handler->validate(mMap[itemName]));
			else
				EXPECT_FALSE(handler->validate(mMap[itemName]));

			delete handler;
		}

	std::string mBoolTrue;
	std::string mBoolFalse;
	std::string mShortMax;
	std::string mUshortMax;
	std::string mIntMax;
	std::string mUintMax;
	std::string mLongMax;
	std::string mUlongMax;
	std::string mShortMin;
	std::string mUshortMin;
	std::string mIntMin;
	std::string mUintMin;
	std::string mLongMin;
	std::string mUlongMin;
	std::string mReal; //make it not nreal
	std::string mNreal;
	std::string mInt2;
	std::string mInt3;
	std::string mInt4;
	std::string mInt5;
	std::string mReal2;
	std::string mReal3;
	std::string mReal4;
	std::string mReal5;
	std::string mNreal2;
	std::string mNreal3;
	std::string mNreal4;
	std::string mNreal5;
	std::string mMatrix2;
	std::string mMatrix3;
	std::string mMatrix4;
	std::string mNmatrix2;
	std::string mNmatrix3;
	std::string mNmatrix4;
	std::string mBlank;
	std::string mCmdCd;
	std::string mCmdLp;
	std::string mCmdLs;
	std::string mCmdPwd;
	std::string mCmdSet;

	StrStrMap mMap;
};

TEST_F(TestArgHandlerLib, bool)
{
	StringVector sv;
	sv.push_back("boolTrue");
	sv.push_back("boolFalse");
	test("bool", sv.begin(), sv.end(), 1);
}
TEST_F(TestArgHandlerLib, short)
{
	StringVector sv;
	sv.push_back("shortMax");
	sv.push_back("shortMin");
	sv.push_back("ushortMin");
	sv.push_back("uintMin");
	sv.push_back("ulongMin");
	test("short", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, ushort)
{
	StringVector sv;
	sv.push_back("shortMax");
	sv.push_back("shortMin");
	sv.push_back("ushortMax");
	sv.push_back("ushortMin");
	sv.push_back("uintMin");
	sv.push_back("ulongMin");
	test("ushort", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, int)
{
	StringVector sv;
	sv.push_back("shortMax");
	sv.push_back("shortMin");
	sv.push_back("ushortMax");
	sv.push_back("ushortMin");
	sv.push_back("intMax");
	sv.push_back("intMin");
	sv.push_back("uintMin");
	sv.push_back("ulongMin");
	if(mIntMax == mLongMax) //might depend on machine ?
	{
		sv.push_back("longMin");
		sv.push_back("longMax");
	}
	test("int", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, uint)
{
	StringVector sv;
	sv.push_back("shortMax");
	sv.push_back("shortMin");
	sv.push_back("ushortMax");
	sv.push_back("ushortMin");
	sv.push_back("intMax");
	sv.push_back("intMin");
	sv.push_back("uintMin");
	sv.push_back("uintMax");
	sv.push_back("ulongMin");
	if(mIntMax == mLongMax) //depend on machine 
	{
		sv.push_back("longMin");
		sv.push_back("longMax");
		sv.push_back("ulongMax");
	}
	test("uint", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, long)
{
	StringVector sv;
	sv.push_back("shortMax");
	sv.push_back("shortMin");
	sv.push_back("ushortMax");
	sv.push_back("ushortMin");
	sv.push_back("intMax");
	sv.push_back("intMin");
	sv.push_back("uintMin");
	sv.push_back("longMin");
	sv.push_back("longMax");
	sv.push_back("ulongMin");
	if(mIntMax != mLongMax) //depend on machine 
	{
		sv.push_back("uintMax");
	}
	test("long", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, ulong)
{
	StringVector sv;
	sv.push_back("shortMax");
	sv.push_back("shortMin");
	sv.push_back("ushortMax");
	sv.push_back("ushortMin");
	sv.push_back("intMax");
	sv.push_back("intMin");
	sv.push_back("uintMin");
	sv.push_back("uintMax");
	sv.push_back("longMax");
	sv.push_back("longMin");
	sv.push_back("ulongMin");
	sv.push_back("ulongMax");
	test("ulong", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, real)
{
	StringVector sv;
	sv.push_back("shortMax");
	sv.push_back("shortMin");
	sv.push_back("ushortMax");
	sv.push_back("ushortMin");
	sv.push_back("intMax");
	sv.push_back("intMin");
	sv.push_back("uintMin");
	sv.push_back("uintMax");
	sv.push_back("longMax");
	sv.push_back("longMin");
	sv.push_back("ulongMin");
	sv.push_back("ulongMax");
	sv.push_back("real");
	sv.push_back("nreal");
	test("real", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, nreal)
{
	StringVector sv;
	sv.push_back("nreal");
	sv.push_back("ushortMin");
	sv.push_back("uintMin");
	sv.push_back("ulongMin");
	test("nreal", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, int2)
{
	StringVector sv;
	sv.push_back("int2");
	test("int2", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, int3)
{
	StringVector sv;
	sv.push_back("int3");
	test("int3", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, int4)
{
	StringVector sv;
	sv.push_back("int4");
	test("int4", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, int5)
{
	StringVector sv;
	sv.push_back("int5");
	test("int5", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, real2)
{
	StringVector sv;
	sv.push_back("int2");
	sv.push_back("real2");
	sv.push_back("nreal2");
	test("real2", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, real3)
{
	StringVector sv;
	sv.push_back("int3");
	sv.push_back("real3");
	sv.push_back("nreal3");
	test("real3", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, real4)
{
	StringVector sv;
	sv.push_back("int4");
	sv.push_back("real4");
	sv.push_back("nreal4");
	sv.push_back("matrix2");
	sv.push_back("nmatrix2");
	test("real4", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, real5)
{
	StringVector sv;
	sv.push_back("int5");
	sv.push_back("real5");
	sv.push_back("nreal5");
	test("real5", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, nreal2)
{
	StringVector sv;
	sv.push_back("nreal2");
	test("nreal2", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, nreal3)
{
	StringVector sv;
	sv.push_back("nreal3");
	test("nreal3", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, nreal4)
{
	StringVector sv;
	sv.push_back("nreal4");
	sv.push_back("nmatrix2");
	test("nreal4", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, nreal5)
{
	StringVector sv;
	sv.push_back("nreal5");
	test("nreal5", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, matrix2)
{
	StringVector sv;
	sv.push_back("int4");
	sv.push_back("real4");
	sv.push_back("nreal4");
	sv.push_back("matrix2");
	sv.push_back("nmatrix2");
	test("matrix2", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, matrix3)
{
	StringVector sv;
	sv.push_back("matrix3");
	sv.push_back("nmatrix3");
	test("matrix3", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, matrix4)
{
	StringVector sv;
	sv.push_back("matrix4");
	sv.push_back("nmatrix4");
	test("matrix4", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, nmatrix2)
{
	StringVector sv;
	sv.push_back("nreal4");
	sv.push_back("nmatrix2");
	test("nmatrix2", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, nmatrix3)
{
	StringVector sv;
	sv.push_back("nmatrix3");
	test("nmatrix3", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, nmatrix4)
{
	StringVector sv;
	sv.push_back("nmatrix4");
	test("nmatrix4", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, blank)
{
	StringVector sv;
	sv.push_back("blank");
	test("blank", sv.begin(), sv.end(), 1);
}

TEST_F(TestArgHandlerLib, cmd)
{
	StringVector sv;
	sv.push_back("cd");
	sv.push_back("lp");
	sv.push_back("pwd");
	sv.push_back("ls");
	sv.push_back("set");
	test("cmd", sv.begin(), sv.end(), 1);
}

#endif /* TESTPACARGHANDLER_H */
