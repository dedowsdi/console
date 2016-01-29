#ifndef PACINTRINSICARGHANDLER_H
#define PACINTRINSICARGHANDLER_H 

#include "pacArgHandler.h"

namespace pac
{

/**
 * int type argument handler. Registerd with int
 */
class _PacExport IntArgHandler :public ArgHandler
{
public:
	defArgCom(IntArgHandler)
	IntArgHandler();

	virtual void doPrompt(const String& s);
	virtual bool doValidate(const String& s);
};

/**
 * int type argument handler. Registerd with bool
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
 * Real type argument handler. Registerd with Real
 */
class _PacExport RealArgHandler :public ArgHandler
{
public:
	defArgCom(RealArgHandler)
	RealArgHandler();

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

	size_t size(){return mStrings.size();}
	StringArgHandler& insert(const String& s);
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

class _PacExport PathArgHandler :public ArgHandler
{
public:
	defArgCom(PathArgHandler)
	PathArgHandler();

	virtual void doPrompt(const String& s);
	virtual bool doValidate(const String& s);
};

/**
 * Command name arg handler
 */
class _PacExport CmdArgHandler :public StringArgHandler
{
public:
	defArgCom(CmdArgHandler)
	CmdArgHandler();
};



}


#endif /* PACINTRINSICARGHANDLER_H */
