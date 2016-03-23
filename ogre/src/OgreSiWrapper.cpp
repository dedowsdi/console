#include "pacStable.h"
#include "pacArgHandler.h"
#include "OgreSiWrapper.h"
#include "OgreStringInterface.h"

namespace pac {
ParamAhDictMap OgreSiWrapper::msAhDictMap;

//------------------------------------------------------------------------------
void AhDict::bindArgHandler(
    const std::string& param, const std::string& ahName) {
  mMap[param] = ahName;
}

//------------------------------------------------------------------------------
const std::string& AhDict::getParamAhName(const std::string& param) {
  StrStrMap::iterator iter = mMap.find(param);
  if (iter == mMap.end())
    PAC_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "can not find ah for " + param);

  return iter->second;
}

//------------------------------------------------------------------------------
OgreSiWrapper::OgreSiWrapper(const std::string& name, Ogre::StringInterface* si)
    : StringInterface(name, true), mOgreSI(si), mAhDict(0) {}

//------------------------------------------------------------------------------
bool OgreSiWrapper::setParameter(const std::string& name, ArgHandler* handler) {
  return mOgreSI->setParameter(name, handler->getValue());
}

//------------------------------------------------------------------------------
std::string OgreSiWrapper::getParameter(const std::string& name) const {
  return mOgreSI->getParameter(name);
}

//------------------------------------------------------------------------------
const std::string& OgreSiWrapper::getValueArgHandler(const std::string& name) {
  if (!mAhDict) PAC_EXCEPT(Exception::ERR_INVALID_STATE, "0 ahDict");
  return mAhDict->getParamAhName(name);
}

//------------------------------------------------------------------------------
bool OgreSiWrapper::createaAhDict() {
  ParamAhDictMap::iterator iter = msAhDictMap.find(mName);

  if (iter == msAhDictMap.end()) {
    mAhDict =
        &msAhDictMap.insert(std::make_pair(mName, AhDict())).first->second;
    return true;
  } else {
    mAhDict = &iter->second;
    return false;
  }
}

//------------------------------------------------------------------------------
void OgreSiWrapper::bindArgHandlerByType()
{
  const Ogre::ParameterList& paramList = mOgreSI->getParameters();
  std::for_each(paramList.begin(), paramList.end(),
      [&](const Ogre::ParameterList::value_type& v) -> void {
        std::string ahName;
        switch (v.paramType) {
          case Ogre::PT_BOOL:
            ahName = "bool";
            break;
          case Ogre::PT_REAL:
            ahName = "real";
            break;
          case Ogre::PT_INT:
            ahName = "int";
            break;
          case Ogre::PT_UNSIGNED_INT:
            ahName = "uint";
            break;
          case Ogre::PT_SHORT:
            ahName = "short";
            break;
          case Ogre::PT_UNSIGNED_SHORT:
            ahName = "short";
            break;
          case Ogre::PT_LONG:
            ahName = "long";
            break;
          case Ogre::PT_UNSIGNED_LONG:
            ahName = "ulong";
            break;
          case Ogre::PT_VECTOR3:
            ahName = "real3";
            break;
          case Ogre::PT_MATRIX3:
            ahName = "matrix3";
            break;
          case Ogre::PT_MATRIX4:
            ahName = "matrix4";
            break;
          case Ogre::PT_QUATERNION:
            ahName = "quaternion";
            break;
          case Ogre::PT_COLOURVALUE:
            ahName = "nreal4";
            break;
          default:
            break;
        }
        if (!ahName.empty()) mAhDict->bindArgHandler(v.name, ahName);
      });
}
}
