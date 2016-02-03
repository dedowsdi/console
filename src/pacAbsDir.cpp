#include "pacAbsDir.h"
#include "pacStringInterface.h"
#include "pacException.h"
#include "pacStringUtil.h"

namespace pac
{
template<> RootDir* Singleton<RootDir>::msSingleton = 0;

//------------------------------------------------------------------
AbsDir::AbsDir(const std::string& name, StringInterface* si)
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
std::string AbsDir::getParameter(const std::string& name)
{
	PacAssertS(mStringInterface != 0, "0 string interface at " + getName());
	return mStringInterface->getParameter(name);
}

//------------------------------------------------------------------
const std::string& AbsDir::getValueArgHandler(const std::string& name)
{
	return mStringInterface->getValueArgHandler(name);
}

//------------------------------------------------------------------
StringVector AbsDir::getParameters() const
{
	return mStringInterface->getParameters();
}

//------------------------------------------------------------------
bool AbsDir::setParameter(const std::string& name, const std::string& value)
{
	PacAssertS(mStringInterface != 0, "0 string interface at " + getName());
	return mStringInterface->setParameter(name, value);
}

//------------------------------------------------------------------
bool AbsDir::setParameter(const std::string& name, ArgHandler* valueHandler)
{
	PacAssertS(mStringInterface != 0, "0 string interface at " + getName());
	return mStringInterface->setParameter(name, valueHandler);
}

//------------------------------------------------------------------
void AbsDir::addChild(AbsDir* dir)
{
	//check if exists
	AbsDirs::iterator iter = std::find(mChildren.begin(), mChildren.end(), dir);
	if(iter != mChildren.end())
		PAC_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
				dir->getName() + " already exists");
	
	mChildren.push_back(dir);
	dir->setParent(this);
}

//------------------------------------------------------------------
AbsDir* AbsDir::enterPath()
{
	return this;
}

//------------------------------------------------------------------
std::string AbsDir::getFullPath()
{
	if(this->getName() == pac::delim)	
		return this->getName();
	else
		return this->getParent()->getFullPath() + pac::delim + this->getName();
}

//------------------------------------------------------------------
AbsDir* AbsDir::getChildAt(size_t i)
{
	if(i >= mChildren.size())
		PAC_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				"overflow : " + StringUtil::toString(i));
	return mChildren[i];
}

//------------------------------------------------------------------
AbsDir* AbsDir::getChildByName(const std::string& name)
{
	AbsDirs::iterator iter = std::find_if(mChildren.begin(), mChildren.end(), 
			[&](AbsDirs::value_type& v)->bool
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
AbsDir* AbsDirUtil::findPath(const std::string& path, AbsDir* curDir)
{
	if(path.empty())
		return curDir;

	if(StringUtil::isAbsolutePath(path))	
		return findAbsolutePath(path);
	else
		return findRelativePath(path, curDir);
}

//------------------------------------------------------------------
AbsDir* AbsDirUtil::findAbsolutePath(const std::string& path)
{
	if(path == pac::delim)
		return &sgDirRoot;
	else
		return findRelativePath(path.substr(1), &sgDirRoot);
}

//------------------------------------------------------------------
AbsDir* AbsDirUtil::findRelativePath(const std::string &path, AbsDir* curDir)
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
