#ifndef PACABSDIR_H
#define PACABSDIR_H 
#include "pacConsolePreRequisite.h"

namespace pac
{


typedef std::vector<AbsDir*> DirVector;

/**
 * Abstract directory. Used to get set property of what every you want.
 */
class AbsDir 
{
public:
	AbsDir(const String& name);
	~AbsDir();

	/**
	 * Get property value. 
	 * @param name : property name
	 * @return : property value
	 */
	String getProperty(const String& name);
	/**
	 * Set property value. This is only used for the most simple case.
	 * @param name : property name
	 * @param value : property value
	 */
	void setProperty(const String& name, const String& value);

	/**
	 * Set property value. You can get specific value components from
	 * valueHandler, it's primitive arghandler for simple case, treearghandler
	 * for intricate case.
	 * @param name : property name
	 * @param valueHander : property value handler
	 */
	void setProperty(const String& name, ArgHandler* valueHandler);

	const String& getName() const { return mName; }
	void setName( const String& v){mName = v;}

	AbsDir* getParent() const { return mParent; }
	void setParent( AbsDir* v){mParent = v;}

	/**
	 * Get full path until root 
	 * @return : full path
	 */
	String getFullPath();

	DirVector::iterator beginChildIter();
	DirVector::iterator endChildIter();


protected:

	String mName;
	AbsDir* mParent;
	DirVector mChildren;
	
};

class AbsDirUtil
{
private:
	AbsDirUtil();

public:

	/**
	 * Find dir by path. Path can be relative or absolute. 
	 * @param curDir : current dir
	 * @param path : target path
	 * @return : target dir
	 */
	static AbsDir* findPath(AbsDir* curDir, const String& path);

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
