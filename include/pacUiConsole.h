#ifndef PACUICONSOLE_H
#define PACUICONSOLE_H 

namespace pac
{

#include "pacConsolePreRequisite.h"

class UiConsole 
{
public:
	UiConsole();
	~UiConsole();

	void init();

	/**
	 * By default every line ows 80 characters + 1 \n . Override if you want to
	 * change it.
	 * @remark : 
	 * @return : 
	 */
	virtual void setupTextMetric();

	/**
	 * Fake stdout and stderr. Wrap line automatically. 
	 * @param output : outout content
	 * @param type : 1 stdout, 2 stderr 
	 * @return : *this
	 */
	virtual UiConsole& output(const String& output, int type =1);

	/**
	 * End current line 
	 * @param type : 
	 * @return : *this
	 */
	virtual UiConsole& endl(int type = 1) = 0;

	/**
	 * Fake stdout and stderr. Wrap line automatically. 
	 * @param output : outout content
	 * @param type : 1 stdout, 2 stderr 
	 * @return : *this
	 */
	virtual UiConsole& outputLine(const String& output, int type =1);

	/**
	 * Fake stdout and stderr. No Wrap line.
	 * @param output : outout content
	 * @param type : 1 stdout, 2 stderr 
	 * @return : *this
	 */
	virtual UiConsole& outputNoAutoWrap(const String& output, int type = 1) = 0;


	/**
	 * Complete current typing. 
	 * @param s : content
	 */
	virtual UiConsole& complete(const String& s) = 0;

	/**
	 * Set cwd 
	 * @param cwd : current working directory
	 */
	virtual void updateCwd(const String& cwd) = 0;	

	/**
	 * Set command line
	 * @param cmdLine : command line string
	 */
	virtual void updateCommandLine(const String& cmdLine = "") = 0;

	/**
	 * line text width, default to 80. You can change it by overloading
	 * setupTextMetric()
	 * @return : line text width 
	 */
	int getTextWidth() const { return mTextWidth; }

protected:
	void setTextWidth( int v){mTextWidth = v;}

private:

	int mTextWidth;
};


}


#endif /* PACUICONSOLE_H */
