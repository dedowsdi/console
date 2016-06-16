#ifndef PACSTRINGINTERFACE_H
#define PACSTRINGINTERFACE_H

#include "pacConsolePreRequisite.h"
#include "pacStdUtil.h"
#include "pacException.h"

namespace pac {

/**
 * Abstract class which is command object which gets/sets parameters.
 */
class _PacExport ParamCmd {
public:
  ParamCmd(const std::string& _ahName) : ahName(_ahName) {}
  void doSet(void* target, const std::string& val);
  virtual std::string doGet(const void* target) const = 0;
  virtual void doSet(void* target, ArgHandler* handler);
  virtual ~ParamCmd() {}
  std::string ahName;  // argument handler name
};

class _PacExport ReadonlyParamCmd : public ParamCmd {
public:
  ReadonlyParamCmd() : ParamCmd("readonly") {}
  void doSet(void* target, ArgHandler* handler);
};

/**
 * Param definition. include getset command and argument handler name.
 */
class _PacExport ParamDef {
public:
  std::string name;    // parameter name
  std::string desc;    // description
  ParamCmd* paramCmd;  // getset command

  ParamDef(const std::string& _name, ParamCmd* _paramCmd,
      const std::string& _desc = "")
      : name(_name), desc(_desc), paramCmd(_paramCmd) {}
};

typedef std::map<std::string, ParamDef> ParamMap;
// typedef std::vector<std::string> Params;

/**
 * Class to hold a dictionary of parameters for a single class.
 */
class _PacExport ParamDictionary {
  friend class StringInterface;

protected:
  /// Definitions of parameters
  ParamMap mParamMap;

  /**
   * Retrieves the parameter command object for a named parameter.
   * @param name : parameter name
   * @return : parameer command object or 0 if not found.
   */
  ParamCmd* getParamCmd(const std::string& name);

  const ParamCmd* getParamCmd(const std::string& name) const;

  /**
   * get parameter argument handler name
   */
  const std::string& getParamAhName(const std::string& name) const;

public:
  ParamDictionary() {}

  /**
   * Method for adding a parameter definition for this class.
   * @param paramDef : A ParamDef object defining the parameter
   */
  void addParameter(const ParamDef& paramDef);

  void addParameter(const std::string& name, ParamCmd* paramCmd,
      const std::string& desc = "");

  /**
   * Retrieve parameter name list.
   * @return : vector of parameter names
   */
  StringVector getParameters(void) const;
};
typedef std::map<std::string, ParamDictionary> ParamDictionaryMap;

/**
 * Class defining the common interface which classes can use to present a
 * reflection-style, self-defining parameter set to callers.
 * @remarks This
 * class also holds a static map of class name to parameter dictionaries for
 * each subclass to use. See ParamDictionary for details.
 * @remarks In order to use this class, each subclass must call createParamDict
 * in their constructors which will create a parameter dictionary for the class
 * if it does not exist yet.
 */
class _PacExport StringInterface {
protected:
  /**
   * Internal method for creating a parameter dictionary for the class, if it
   * does not already exist.
   */
  bool createParamDict();
  virtual void buildParams() {}

public:
  StringInterface(const std::string& name, bool wrapper)
      : mWrapper(wrapper), mName(name), mParamDict(NULL) {}
  virtual ~StringInterface() {}

  // must be called after ctor
  void initParams();

  ParamDictionary* getParamDict(void) { return mParamDict; }
  const ParamDictionary* getParamDict(void) const { return mParamDict; }

  virtual StringVector getParameters(void) const;

  /**
   * Set parameter value. It doesn't matter if specified prameter doesn't
   * exists, it will return false in this case.
   * @param name : parameter name
   * @param value : parameter value
   * @return : true if success, false fail or parameter not found.
   */
  virtual bool setParameter(const std::string& name, const std::string& value);

  /**
   * Use arghandler to
   * @param name : parameter name
   * @param handler : parameter argument handler
   * @return : true if success, false fail or parameter not found.
   */
  virtual bool setParameter(const std::string& name, ArgHandler* handler);

  /**
   * Generic multiple parameter setting method.
   * @param paramList : name/value pair list
   */
  virtual void setParameterList(const NameValuePairList& paramList);

  /**
   * Get parameter value.
   * @param name : parameter name
   * @return : parameter value
   */
  virtual std::string getParameter(const std::string& name) const;

  /**
   * Get param value arg handler  name
   * @param name : parameter name
   * @return : param value arg handler name
   */
  virtual const std::string& getValueArgHandler(const std::string& name);

  /**
   * Copy parameter values to another stringinterface. These two
   * stringinterface don't need to have the same parameters.
   * @param dest : dest parameters
   */
  virtual void copyParametersTo(StringInterface* dest) const;
  /** Cleans up the static 'msDictionary' required to reset Ogre,
    otherwise the containers are left with invalid pointers, which will lead to
    a crash
    as soon as one of the ResourceManager implementers (e.g. MaterialManager)
    initializes.*/

  virtual void serialize(std::ostream& os, size_t lvl = 0);

  static void cleanupDictionary();

  // called when dir was created with this StringInterface
  virtual void onCreateDir(AbsDir* dir) { (void)dir; }

  bool getWrapper() const { return mWrapper; }
  void setWrapper(bool v) { mWrapper = v; }

  const std::string& getName() const { return mName; }
  void setName(const std::string& v) { mName = v; }

protected:
  bool mWrapper;
  std::string mName;
  // Dictionary of parameters

  // Class name for this instance to be used as a lookup (must be
  // initialised by subclasses)
  ParamDictionary* mParamDict;

  static ParamDictionaryMap msDictionary;
};
}

#endif
