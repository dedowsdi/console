#include "pacAbsDir.h"
#include "pacStringInterface.h"
#include "pacException.h"
#include "pacStringUtil.h"

namespace pac
{

//------------------------------------------------------------------
AbsDir::AbsDir(const String& name, StringInterface* si)
	:mName(name)
	 ,mStringInterface(si)
{
}

//------------------------------------------------------------------
AbsDir::~AbsDir()
{
	std::for_each(mChildren.begin(), mChildren.end(), [&](AbsDir* v)->void
	{
		delete v;	
	});
	mChildren.clear();
}

//------------------------------------------------------------------
String AbsDir::getParameter(const String& name)
{
	PacAssert(mStringInterface != 0, "0 string interface at " + getName());
	return mStringInterface->getParameter(name);
}

//------------------------------------------------------------------
bool AbsDir::setParameter(const String& name, const String& value)
{
	PacAssert(mStringInterface != 0, "0 string interface at " + getName());
	return mStringInterface->setParameter(name, value);
}

//------------------------------------------------------------------
void AbsDir::setParameter(const String& name, ArgHandler* valueHandler)
{
	PacAssert(mStringInterface != 0, "0 string interface at " + getName());
	return mStringInterface->setParameter(name, valueHandler);
}

//------------------------------------------------------------------
void AbsDir::addChild(AbsDir* dir)
{
	//check if exists
	Dirs::iterator iter = std::find_if(mChildren.begin(), mChildren.end());
	if(iter != mChildren.end())
		PAC_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
				dri->getName() + " already exists", __FUNCTION__);
	
	mChildren.push_back(dir);
	dir->setParent(this);
}

//------------------------------------------------------------------
AbsDir* AbsDir::enterPath()
{
	return this;
}

//------------------------------------------------------------------
String AbsDir::getFullPath()
{
	if(this->getName() == pac::delim)	
		return this->geTname();
	else
		return this->getParent()->getFullPath() + pac::delim +  this->getName();
}

//------------------------------------------------------------------
AbsDir* AbsDir::getChildAt(size_t i)
{
	if(i >= mChildren.size())
		PAC_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				"overflow : " + StringUtil::toString(i), __FUNCTION__);
	return mChildren[i];
}

//------------------------------------------------------------------
AbsDir* AbsDir::getChildByName(const String& name)
{
	Dirs::iterator iter = std::find_if(mChildren.begin(), mChildren.end(), 
			[&](Dirs::value_type& v)->bool
			{
			return v->getName() == name;
			});
	
	return iter == mChildren.end() ? 0 : *iter;
}

//------------------------------------------------------------------
AbsDirs::iterator AbsDir::beginChildIter()
{
	return mChildren.begin();
}

//------------------------------------------------------------------
AbsDirs::iterator AbsDir::endChildIter()
{
	return mChildren.end();
}

//------------------------------------------------------------------
RootDir::RootDir():
	AbsDir(pac::delim, 0)
{
}

//------------------------------------------------------------------
DotDir::DotDir():
	AbsDir(".", 0)
{
}

//------------------------------------------------------------------
AbsDir* DotDir::enterPath()
{
	return this->getParent();
}

//------------------------------------------------------------------
DotDotDir::DotDotDir():
	AbsDir("..", 0)
{
}

//------------------------------------------------------------------
AbsDir* DotDotDir::enterPath()
{
	return this->getParent()->getParent();
}

//------------------------------------------------------------------
AbsDir* AbsDirUtil::findPath(const String& path, AbsDir* curDir)
{
	if(path.empty())
		return curDir;

	if(StringUtil::isAbsolutePath(path))	
		return findAbsolutePath(path);
	else
		return findRelativePath(path, curDir)
	
	
}

//------------------------------------------------------------------
AbsDir* AbsDirUtil::findAbsolutePath(const String& path)
{
	if(path == pac::delim)
		return &sgDirRoot;
	else
		return findRelativePath(path.substr(1), &sgDirRoot);
}

//------------------------------------------------------------------
AbsDir* AbsDirUtil::findRelativePath(const String &path, AbsDir* curDir)
{
	StringVector sv = StringUtil::split(path, pac::delim);

	AbsDir* parentDir;
	for (size_t i = 0; i != sv.size(); ++i)
	{
		parentDir = curDir;

		curDir = parentDir->getChildByName(sv[i]);
		if(curDir == 0)
			return 0;

		//deal with . and ..
		curDir = curDir->enterPath();
	}

	return curDir;
}

}
