#ifndef PACCONSOLE_H
#define PACCONSOLE_H 

#include "pacSingleton.h"
#include "pacRollStack.h"

namespace pac
{

/**
 * Must be decoupled from ui and command.
 */
class Console: public Singleton<Console>
{
public:
	
	friend class RaiiConsoleBuffer;

	typedef std::map<std::string, Command*> CmdMap;

	Console(UiConsole* ui);
	virtual ~Console();

	virtual void init();

	/**
	 * Execute cmdLine. Add cmdLine to command history. 
	 * @param cmdLine : cmd line to be executed
	 */
	bool execute(const std::string& cmdLine);
	/**
	 * Output prompt for cmdLine 
	 * @param cmdLine : current typing cmd line.
	 */
	void prompt(const std::string& cmdLine);
	/**
	 * Output to ui console or buffer. Call startBuffer before this if you want
	 * to out put to buffer, call endBuffer after you finished output. Context
	 * in buffer will be aligned automatically. Buffer is only used for type 1.
	 * @param s : output content
	 * @param type : 1 stdout, 2 stderr 
	 * @return : Console&
	 */
	Console& output(const std::string& s, int type = 1);

	Console& outputLine(const std::string& s, int type = 1);
	/**
	 * End current line
	 */
	Console& endl(int type = 1);

	/**
	 * Complete current typing. 
	 * @param s : string to be added after current cursor
	 */
	Console& complete(const std::string& s);

	/**
	 * Change current working dir 
	 * @param dir : target dir
	 */
	void changeDirectory(AbsDir* dir);

	AbsDir* getDirectory(){ return mDir; }
	
	/**
	 * Register new command and it's argument handler.
	 * @remark : don't release prototype by yourself
	 * @param cmdProto : command prototype,
	 */
	void registerCommand(Command* cmdProto);

	/**
	 * Buffer output, to be aligned later. This function will reset buffer. Use
	 * RaiiConsoleBuffer to make suer endBuffer get called after this.
	 */
	void startBuffer();
	/**
	 * flash buffer 
	 */
	void endBuffer();

	/**
	 * Roll command history 
	 * @param backWard : roll back or roll front
	 */
	void rollCommand(bool backWard = true);

	CmdMap::const_iterator beginCmdMapIterator() const;
	CmdMap::const_iterator endCmdMapIterator() const;

private:


	/**
	 * Create command by command name 
	 * @param cmdName : command name
	 * @return : newly created command or 0 
	 */
	Command* createCommand(const std::string& cmdName);

	/**
	 * Prompt for command name .
	 * @param cmdLine : 
	 */
	void promptCommandName(const std::string& cmdName);

	/**
	 * When you hit tab or enter in term, there will be a record of cwd and
	 * command line. This is used to fake that.
	 * @param cmdLine : current command line
	 */
	void fakeOutputDirAndCmd(const std::string& cmdLine);

	/**
	 * Add cmdLine to history 
	 * @param cmdLine : command line
	 */
	void addCmdLineToHistory(const std::string& cmdLine);

	void appendBuffer( const std::string& v);

	/**
	 * Get max lenth of string item 
	 * @param beg : begin iterator
	 * @param end : end iterator 
	 * @return : length of longest string  
	 */
	int getColumnWidth(StringVector::iterator beg, StringVector::iterator end);

	/**
	 * Determine column number and width of each column in buffer. Just a place
	 * to shrink endBuffer().  
	 * @param numCol : column number
	 * @param totalColWidth : total column width without spacing
	 * @param colWidthes : vectoer of column width 
	 * @return : 
	 */
	void getColumnNumber(size_t &numCol, size_t &totalColWidth, IntVector& colWidthes);


private:

	int mIsBuffering;

	AbsDir* mDir;
	UiConsole* mUi;
	CmdMap mCmdMap;
	StringVector mBuffer;

	RollStack<std::string> mCmdHistory;

};

/**
 * RAII of console buffer
 */
class RaiiConsoleBuffer
{
public:
	RaiiConsoleBuffer();
	~RaiiConsoleBuffer();	
};


}


#endif /* PACCONSOLE_H */
