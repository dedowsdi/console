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
	Command(const std::string& name);

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
	
	const std::string& getName() const { return mName; }
	void setName( const std::string& v){mName = v;}

	std::string getCmdLine(){ return mCmdLine; }
	/**
	 * Parse args and options 
	 * @param v : args and options string 
	 */
	void setArgsAndOptions( const std::string& v);

	/**
	 * Get arg handler from arglib if it's 0. 
	 * @return : mArgHandler
	 */
	ArgHandler* getArgHandler();

	const std::string& getAhName() const { return mAhName; }
	void setAhName( const std::string& v){mAhName = v;}

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
	std::string getDefAhName(){ return "cmd_" + getName(); }

private:
	/**
	 * Init argHandler, register it to arghandlerlib. Don't override this if you
	 * use primitive arg handler.
	 */
	virtual void buildArgHandler(){};

protected:
	std::string mName;
	std::string mCmdLine;
	std::string mOptions;
	std::string mArgs;
	std::string mAhName;		//argument ahandler name, it's cmd_{mName} by default

	ArgHandler* mArgHandler;
};


}


#endif /* PACCOMMAND_H */
