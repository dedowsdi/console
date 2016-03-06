#ifndef OGRESIWRAPPER_H
#define OGRESIWRAPPER_H

#include "OgreConsolePreRequisite.h"
#include "pacStringInterface.h"

namespace pac {

class AhDict {
public:
  void bindArgHandler(const std::string& param, const std::string& ahName);
  const std::string& getParamAhName(const std::string& param);

private:
  StrStrMap mMap;
};

typedef std::map<std::string, AhDict> ParamAhDictMap;

/**
 * wrapper for Ogre::StringInterface
 */
class OgreSiWrapper : public StringInterface {
public:
  OgreSiWrapper(const std::string& name, Ogre::StringInterface* si);

  virtual bool setParameter(const std::string& name, ArgHandler* handler);

  virtual std::string getParameter(const std::string& name) const;

  const std::string& getValueArgHandler(const std::string& name);

  bool createaAhDict();
protected:

  void bindArghanderByType();

protected:
  Ogre::StringInterface* mOgreSI;
  AhDict* mAhDict;
  static ParamAhDictMap msAhDictMap;
};
}

#endif /* OGRESIWRAPPER_H */
