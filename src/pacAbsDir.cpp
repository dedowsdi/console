#include "pacAbsDir.h"
#include "pacStringInterface.h"
#include "pacException.h"
#include "pacStringUtil.h"

namespace pac {

//------------------------------------------------------------------------------
AbsDir::AbsDir(const std::string& name, StringInterface* si /*= 0*/)
    : mTemp(false), mParent(0), mStringInterface(si), mName(name) {
  if (mStringInterface) mStringInterface->onCreateDir(this);
}

//------------------------------------------------------------------------------
AbsDir::~AbsDir() {
  if (mParent) mParent->removeChild(this);
  // operator on copy
  AbsDirs dirs(mChildren);
  std::for_each(dirs.begin(), dirs.end(), [&](AbsDir* v) -> void { delete v; });
  PacAssert(mChildren.size() == 0, "non 0 children after remove children");
  // mChildren.clear();
  if (mStringInterface && mStringInterface->getWrapper())
    delete mStringInterface;
}

//------------------------------------------------------------------------------
std::string AbsDir::getParameter(const std::string& name) {
  PacAssertS(mStringInterface != 0, "0 string interface at " + getName());
  return mStringInterface->getParameter(name);
}

//------------------------------------------------------------------------------
const std::string& AbsDir::getValueArgHandler(const std::string& name) {
  return mStringInterface->getValueArgHandler(name);
}

//------------------------------------------------------------------------------
StringVector AbsDir::getParameters() const {
  static StringVector sv;
  if (!mStringInterface) return sv;

  return mStringInterface->getParameters();
}

//------------------------------------------------------------------------------
bool AbsDir::setParameter(const std::string& name, const std::string& value) {
  if (!mStringInterface)
    PAC_EXCEPT(
        Exception::ERR_INVALID_STATE, "0 string interface at " + getName());
  return mStringInterface->setParameter(name, value);
}

//------------------------------------------------------------------------------
bool AbsDir::setParameter(const std::string& name, ArgHandler* valueHandler) {
  if (!mStringInterface)
    PAC_EXCEPT(
        Exception::ERR_INVALID_STATE, "0 string interface at " + getName());
  return mStringInterface->setParameter(name, valueHandler);
}

//------------------------------------------------------------------------------
void AbsDir::addChild(AbsDir* dir, bool temp /*= true*/) {
  // name check. It's not checked at ctor for root dir name is equal to delim
  if (dir->getName().find_first_of(" " + pac::delim) != std::string::npos)
    PAC_EXCEPT(Exception::ERR_INVALIDPARAMS,
        "Dir name can not contain space or delim");

  // check if exists
  AbsDirs::iterator iter = std::find(mChildren.begin(), mChildren.end(), dir);
  if (iter != mChildren.end())
    PAC_EXCEPT(
        Exception::ERR_DUPLICATE_ITEM, dir->getName() + " already exists");

  dir->setTemp(temp);

  mChildren.push_back(dir);
  dir->setParent(this);
}

//------------------------------------------------------------------------------
AbsDir* AbsDir::addUniqueChild(
    const std::string& name, StringInterface* si, bool temp /*= true*/) {
  if (hasChild(name)) {
    AbsDir* dir = getChildByName(name);
    dir->setStringInterface(si);
    dir->setTemp(temp);
    return dir;
  }

  AbsDir* dir = new AbsDir(name, si);
  this->addChild(dir, temp);
  return dir;
}

//------------------------------------------------------------------------------
std::string AbsDir::getFullPath() {
  if (!mParent) return this->getName();
  return mParent->getFullPath() + this->getName() + pac::delim;
}

//------------------------------------------------------------------------------
AbsDir* AbsDir::getChildAt(size_t i) {
  if (i >= mChildren.size())
    PAC_EXCEPT(
        Exception::ERR_INVALIDPARAMS, "overflow : " + StringUtil::toString(i));
  return mChildren[i];
}

//------------------------------------------------------------------------------
AbsDir* AbsDir::getChildByName(const std::string& name) {
  AbsDirs::iterator iter = std::find_if(mChildren.begin(), mChildren.end(),
      [&](AbsDirs::value_type& v) -> bool { return v->getName() == name; });

  if (iter == mChildren.end())
    PAC_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, name + " not found at " + mName);

  return *iter;
}

//------------------------------------------------------------------------------
bool AbsDir::hasChild(const std::string& name) {
  AbsDirs::iterator iter = std::find_if(mChildren.begin(), mChildren.end(),
      [&](AbsDirs::value_type& v) -> bool { return v->getName() == name; });
  return iter != mChildren.end();
}

//------------------------------------------------------------------------------
void AbsDir::removeChildAt(size_t i) {
  if (i >= mChildren.size())
    PAC_EXCEPT(
        Exception::ERR_INVALIDPARAMS, "overflow : " + StringUtil::toString(i));
  mChildren.erase(mChildren.begin() + i);
}

//------------------------------------------------------------------------------
void AbsDir::removeChildByName(const std::string& name) {
  AbsDirs::iterator iter = std::find_if(mChildren.begin(), mChildren.end(),
      [&](AbsDirs::value_type& v) -> bool { return v->getName() == name; });

  if (iter == mChildren.end())
    PAC_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, name + " not found at " + mName);
  mChildren.erase(iter);
}

//------------------------------------------------------------------------------
void AbsDir::removeChild(AbsDir* dir) {
  AbsDirs::iterator iter = std::find(mChildren.begin(), mChildren.end(), dir);

  if (iter == mChildren.end())
    PAC_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, dir->getName() + " not found ");
  mChildren.erase(iter);
}

//------------------------------------------------------------------------------
void AbsDir::serialize(
    std::ostream& os, bool recursive /*= true*/, size_t lvl /*= 0*/) {
  if (mStringInterface) mStringInterface->serialize(os, lvl);

  if (recursive) {
    size_t childLvl = ++lvl;
    std::for_each(mChildren.begin(), mChildren.end(),
        [&](AbsDir* v) -> void { v->serialize(os, recursive, childLvl); });
  }
}

//------------------------------------------------------------------------------
void AbsDir::cleanTempDirs() {
  if (mTemp) {
    delete this;
    return;
  }
  std::for_each(mChildren.begin(), mChildren.end(),
      [&](AbsDir* v) -> void { v->cleanTempDirs(); });
}

//------------------------------------------------------------------------------
AbsDirs::iterator AbsDir::beginChildIter() { return mChildren.begin(); }

//------------------------------------------------------------------------------
AbsDirs::iterator AbsDir::endChildIter() { return mChildren.end(); }

//------------------------------------------------------------------------------
RootDir::RootDir() : AbsDir(pac::delim, 0) {}

//------------------------------------------------------------------------------
AbsDir* AbsDirUtil::findPath(const std::string& path, AbsDir* curDir /*=0*/) {
  if (path.find(" ") != std::string::npos)
    PAC_EXCEPT(
        Exception::ERR_INVALIDPARAMS, "\"" + path + "\" contains space!");

  if (path.empty()) return curDir;

  if (StringUtil::isAbsolutePath(path))
    return findAbsolutePath(path);
  else
    return findRelativePath(path, curDir);
}

//------------------------------------------------------------------------------
AbsDir* AbsDirUtil::findAbsolutePath(const std::string& path) {
  if (path == pac::delim)
    return &sgRootDir;
  else
    return findRelativePath(path.substr(1), &sgRootDir);
}

//------------------------------------------------------------------------------
AbsDir* AbsDirUtil::findRelativePath(const std::string& path, AbsDir* curDir) {
  if (!curDir) PAC_EXCEPT(Exception::ERR_INVALIDPARAMS, "0 current dir");

  StringVector sv = StringUtil::split(path, pac::delim);
  for (size_t i = 0; i != sv.size(); ++i) {
    if (!curDir) break;
    if (sv[i] == ".") continue;
    if (sv[i] == "..") {
      curDir = curDir->getParent();
      continue;
    }
    try {
      curDir = curDir->getChildByName(sv[i]);
    } catch (ItemIdentityException e) {
      return 0;
    }
  }
  return curDir;
}
template <>
RootDir* Singleton<RootDir>::msSingleton = 0;
}
