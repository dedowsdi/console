#ifndef PACINTRINSICARGHANDLER_H
#define PACINTRINSICARGHANDLER_H 

#include "pacArgHandler.h"
#include "pacConsole.h"
#include "pacStringUtil.h"

namespace pac
{

/**
 * Primitive decimal arg handler
 * @remark : should work from unsigned short until long long 
 */
template<class T>
class _PacExport PriDecArgHandler()
{
	defArgCom(IntArgHandler)

	PriDecArgHandler(const String& name):
		ArgHandler(name){}
	
	virtual void doPrompt(const String& s)
	{
		sgConsole.outputLine("@@" + getName() + "@@");
	}
	virtual bool doValidate(const String& s)
	{
		return StringUtil::isPrimitiveDecimal<T>(s);
	}
}


/**
 * Decimal in range. Used to build normalized Real (-1.0 to 1.0)
 */
template<class T>
class _PacExport PriDecRangeArgHandler :public ArgHandler
{
public:
	defArgCom(PriDecRangeArgHandler)
	/**
	 * ctor
	 * @param name : arg handler name 
	 * @param min : min value
	 * @param max : max value 
	 * @param equal : use <= >= or < > 
	 * @return : 
	 */
	PriDecRangeArgHandler(name, T min, T max, bool equal = true):
		ArgHandler(name)
		,mMin(min)
		,mMax(max)
		,mEqual(equal)

	virtual void doPrompt(const String& s)
	{
		$sgConsole.outputLine("@@" + getName() + " between " 
				+ StringUtil::toString(mMin) + "and " + StringUtil::toString(mMax) + "@@");
	}
	virtual bool doValidate(const String& s)
	{
		if(StringUtil::isPrimitiveDecimal<T>(s))
		{
			T t = StringUtil::parsePrimitiveDecimal<T>(s);
			if(mEqual)
				return t <= mMax && t >= mMax:
			else
				return t < mMax && t > mMax;
		}
	}

private:
	T mMin, mMax;
	bool mEqual;
};

/**
 * bool type argument handler. Registerd with bool
 */
class _PacExport BoolArgHandler :public ArgHandler
{
public:
	defArgCom(BoolArgHandler)
	BoolArgHandler();

	virtual void doPrompt(const String& s);
	virtual bool doValidate(const String& s);
};

/**
 * Base class of string type handler. 
 */
class _PacExport StringArgHandler :public ArgHandler
{
public:
	defArgCom(StringArgHandler)
	StringArgHandler(const String& name);

	/**
	 * Single string handler
	 * @param text: item text
	 */
	StringArgHandler(const String& name, const String& text);

	StringArgHandler& insert(const String& s);
	template<class _InputIterator>
	void insert(_InputIterator first, _InputIterator last)
	{
		mStrings.insert(mStrings.end(), first, last);
	}

	size_t size(){return mStrings.size();}
	void remove(const String& s);

	virtual void doPrompt(const String& s);
	virtual bool doValidate(const String& s);

private:

	bool exist(const String& value);

protected:
	StringSet mStrings;
};


/**
 * used at cmd taks no argument
 */
class _PacExport BlankArgHandler :public ArgHandler
{
public:
	defArgCom(BlankArgHandler)
	BlankArgHandler();

	virtual void doPrompt(const String& s);
	virtual bool doValidate(const String& s);
};

/**
 * path
 */
class _PacExport PathArgHandler :public ArgHandler
{
public:
	defArgCom(PathArgHandler)
	PathArgHandler();
	PathArgHandler(const PathArgHandler& rhs);

	virtual void doPrompt(const String& s);
	virtual bool doValidate(const String& s);

	AbsDir* getDir() const { return mDir; }
	void setDir( AbsDir* v){mDir = v;}

private:
	AbsDir* mDir; //cwd
};

/**
 * cmd
 */
class _PacExport CmdArgHandler :public StringArgHandler
{
public:
	defArgCom(CmdArgHandler)
	CmdArgHandler();
};

/**
 * parameter handler. Will be populated with parameters of cwd.
 */
class _PacExport ParameterArgHandler :public StringArgHandler
{
public:
	defArgCom(ParameterArgHandler)
	ParameterArgHandler();
	ParameterArgHandler(const ParameterArgHandler& rhs);

	AbsDir* getDir() const { return mDir; }
	void setDir( AbsDir* v){mDir = v;}

private:
	AbsDir* mDir; //cwd
};

/**
 * parameter value handler. Must follow parameter handler. registered with value
 */
class _PacExport ValueArgHandler :public ArgHandler 
{
public:
	defArgCom(ValueArgHandler)
	ValueArgHandler();
	ValueArgHandler(const ValueArgHandler& rhs);

	AbsDir* getDir() const { return mDir; }
	void setDir( AbsDir* v){mDir = v;}

	ArgHandler* getHandler() const { return mHandler; }
	void setHandler( ArgHandler* v){mHandler = v;}

	virtual void doPrompt(const String& s);
	virtual bool doValidate(const String& s);

private:
	ArgHandler* mHandler;
	AbsDir* mDir;
};

}

#endif /* PACINTRINSICARGHANDLER_H */
