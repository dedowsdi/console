#ifndef PACABSDIR_H
#define PACABSDIR_H 
#include "pacConsolePreRequisite.h"
#include "pacSingleton.h"

namespace pac
{


typedef std::vector<AbsDir*> AbsDirs;

/**
 * Abstract directory. Used to get set parameter of what every you want.
 */
class AbsDir 
{
public:
	AbsDir(const String& name, StringInterface* si);
	virtual ~AbsDir();

	/**
	 * Get parameter value. 
	 * @param name : parameter name
	 * @return : parameter value
	 */
	String getParameter(const String& name);
	/**
	 * Set parameter value. This is only used for the most simple case.
	 * @param name : parameter name
	 * @param value : parameter value
	 */
	bool setParameter(const String& name, const String& value);

	/**
	 * Set parameter value. You can get specific value components from
	 * valueHandler, it's primitive arghandler for simple case, treearghandler
	 * for intricate case.
	 * @param name : parameter name
	 * @param valueHander : parameter value handler
	 */
	void setParameter(const String& name, ArgHandler* valueHandler);

	const String& getName() const { return mName; }
	void setName( const String& v){mName = v;}

	AbsDir* getParent() const { return mParent; }
	void setParent( AbsDir* v){mParent = v;}

	StringInterface* getStringInterface() const { return mStringInterface; }
	void setStringInterface( StringInterface* v){mStringInterface = v;}

	/**
	 * Add dir to children.  
	 * @param dir : child dir
	 */
	void addChild(AbsDir* dir);

	/**
	 * for . and .. 
	 * @return : target dir 
	 */
	virtual AbsDir* enterPath();

	/**
	 * Get full path until root 
	 * @return : full path
	 */
	String getFullPath();
	
	/**
	 * Throw if i overflow. 
	 * @param i : index
	 * @return : 
	 */
	AbsDir* getChildAt(size_t i);

	/**
	 * Return 0 if not found.
	 * @param name : child dir name
	 * @return : 0 or child dir 
	 */
	AbsDir* getChildByName(const String& name);

	AbsDirs::iterator beginChildIter();
	AbsDirs::iterator endChildIter();

protected:

	String mName;
	AbsDir* mParent;
	StringInterface* mStringInterface;
	AbsDirs mChildren;
};

/*
 *root dir, the same as path delimiter, it's singleton.
 */
class RootDir: public AbsDir, public Singleton<RootDir>
{
public:
	RootDir();
};

class DotDir: public AbsDir
{
public:
	DotDir();

	/**
	 * return self 
	 */
	virtual AbsDir* enterPath();
};

class DotDotDir: public AbsDir
{
public:
	DotDotDir();

	/**
	 * return parent
	 */
	virtual AbsDir* enterPath();
};


class AbsDirUtil
{

private:
	AbsDirUtil(){}

public:

	/**
	 * Find dir by path. Path can be relative or absolute. 
	 * @param path : target path
	 * @param curDir : current dir
	 * @return : target dir
	 */
	static AbsDir* findPath(const String& path, AbsDir* curDir);

	/**
	 * find dir by absolute path
	 * @param path : absolute path
	 * @return : target dir or 0
	 */
	static AbsDir* findAbsolutePath(const String& path);
	/**
	 * find dir by relative path
	 * @param &path : relative path
	 * @param curDir : current working dir
	 * @return : target dir or 0
	 */
	static AbsDir* findRelativePath(const String &path, AbsDir* curDir);

};

}

#endif /* PACABSDIR_H */
