#ifndef PACCOMMAND_H
#define PACCOMMAND_H 

#include "pacConsolePreRequisite.h"

namespace pac
{

#define defCmdCom(type)\
virtual Command* clone(){return new type(*this);}

class Command 
{
public:

	friend class Console;

	/**
	 * ctor. 
	 * @param name : ctor
	 */
	Command(const String& name);

	/**
	 * copy ctor, it doesn't copy mArgHandler.
	 * @param rhs : 
	 */
	Command(const Command&rhs);

	virtual ~Command();

	/**
	 * output prompt for argument 
	 * @remark : 
	 * @return : 
	 */
	virtual void prompt();
	
	/**
	 * execute this cmd
	 * @return : true or false
	 */
	virtual bool execute();

	virtual bool doExecute() = 0;

	virtual Command* clone()=0;
	
	const String& getName() const { return mName; }
	void setName( const String& v){mName = v;}

	String getCmdLine(){ return mCmdLine; }
	/**
	 * Parse args and options 
	 * @param v : args and options string 
	 */
	void setArgsAndOptions( const String& v);

	/**
	 * Get arg handler from arglib if it's 0. 
	 * @return : mArgHandler
	 */
	ArgHandler* getArgHandler();

	const String& getAhName() const { return mAhName; }
	void setAhName( const String& v){mAhName = v;}

private:
	/**
	 * Init cmd, build arghandler. Should be called when registered
	 * @return : this
	 */
	Command* init();

protected:

	/**
	 * Get default command argument handler name. It's "cmd_"+getName()
	 * @return : default command argument handler name
	 */
	String getDefAhName(){ return "cmd_" + getName(); }

private:
	/**
	 * Init argHandler, register it to arghandlerlib. Don't override this if you
	 * use primitive arg handler.
	 */
	virtual void buildArgHandler(){};

protected:
	String mName;
	String mCmdLine;
	String mOptions;
	String mArgs;
	String mAhName;		//argument ahandler name, it's cmd_{mName} by default

	ArgHandler* mArgHandler;
};


}


#endif /* PACCOMMAND_H */
