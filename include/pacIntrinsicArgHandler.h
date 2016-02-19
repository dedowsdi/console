#ifndef PACINTRINSICARGHANDLER_H
#define PACINTRINSICARGHANDLER_H

#include "pacArgHandler.h"
#include "pacConsole.h"
#include "pacStringUtil.h"

namespace pac {

/**
 * Primitive decimal arg handler
 * @remark : should work from unsigned short until long long
 */
template <class T>
class _PacExport PriDeciArgHandler : public ArgHandler {
public:
  defArgCom(PriDeciArgHandler)

      PriDeciArgHandler(const std::string& name)
      : ArgHandler(name) {}

  virtual void populatePromptBuffer(const std::string& s) {
    (void)s;
    appendPromptBuffer(getName());
  }
  virtual bool doValidate(const std::string& s) {
    return StringUtil::isPrimitiveDecimal<T>(s);
  }
};

/**
 * Decimal in range. Used to build normalized Real (-1.0 to 1.0)
 */
template <class T>
class _PacExport PriDeciRangeArgHandler : public ArgHandler {
public:
  defArgCom(PriDeciRangeArgHandler)
      /**
       * ctor
       * @param name : arg handler name
       * @param min : min value
       * @param max : max value
       * @param equal : use <= >= or < >
       * @return :
       */
      PriDeciRangeArgHandler(
          const std::string& name, T min, T max, bool equal = true)
      : ArgHandler(name), mMin(min), mMax(max), mEqual(equal) {}

  virtual void populatePromptBuffer(const std::string& s) {
    (void)s;
    appendPromptBuffer(getName() + " between " + StringUtil::toString(mMin) +
                       "and " + StringUtil::toString(mMax));
  }
  virtual bool doValidate(const std::string& s) {
    if (StringUtil::isPrimitiveDecimal<T>(s)) {
      T t = StringUtil::parsePrimitiveDecimal<T>(s);
      if (mEqual)
        return t <= mMax && t >= mMin;
      else
        return t < mMax && t > mMin;
    }

    return false;
  }

private:
  T mMin, mMax;
  bool mEqual;
};

/**
 * Base class of string type handler.
 */
class _PacExport StringArgHandler : public ArgHandler {
public:
  defArgCom(StringArgHandler) StringArgHandler(const std::string& name);

  /**
   * Single string handler
   * @param text: item text
   */
  StringArgHandler(const std::string& name, const std::string& text);

  /**
   * insert new element
   * @param s : new element
   * @return : this 
   */
  StringArgHandler* insert(const std::string& s);

  template <class _InputIterator>
  void insert(_InputIterator first, _InputIterator last) {
    mStrings.insert(first, last);
  }

  size_t size() { return mStrings.size(); }
  void remove(const std::string& s);

  virtual void populatePromptBuffer(const std::string& s);
  virtual bool doValidate(const std::string& s);

private:
  bool exist(const std::string& value);

protected:
  StringSet mStrings;
};

/**
 * bool type argument handler. Registerd with bool
 */
class _PacExport BoolArgHandler : public StringArgHandler {
public:
  defArgCom(BoolArgHandler) BoolArgHandler();
};

/**
 * used at cmd taks no argument
 */
class _PacExport BlankArgHandler : public ArgHandler {
public:
  defArgCom(BlankArgHandler) BlankArgHandler();

  virtual void populatePromptBuffer(const std::string& s);
  virtual bool doValidate(const std::string& s);
};

/**
 * path
 */
class _PacExport PathArgHandler : public ArgHandler {
public:
  defArgCom(PathArgHandler) PathArgHandler();
  PathArgHandler(const PathArgHandler& rhs);

  virtual void populatePromptBuffer(const std::string& s);
  virtual bool doValidate(const std::string& s);

  AbsDir* getDir() const { return mDir; }
  void setDir(AbsDir* v) { mDir = v; }

private:
  AbsDir* mDir;  // cwd
};

/**
 * cmd
 */
class _PacExport CmdArgHandler : public StringArgHandler {
public:
  defArgCom(CmdArgHandler) CmdArgHandler();
};

/**
 * parameter handler. Will be populated with parameters of cwd.
 */
class _PacExport ParameterArgHandler : public StringArgHandler {
public:
  defArgCom(ParameterArgHandler) ParameterArgHandler();
  ParameterArgHandler(const ParameterArgHandler& rhs);

  AbsDir* getDir() const { return mDir; }
  void setDir(AbsDir* v) { mDir = v; }

private:
  AbsDir* mDir;  // cwd
};

/**
 * parameter value handler. Must follow parameter handler. registered with value
 */
class _PacExport ValueArgHandler : public ArgHandler {
public:
  defArgCom(ValueArgHandler) ValueArgHandler();
  ValueArgHandler(const ValueArgHandler& rhs);

  void runtimeInit();

  AbsDir* getDir() const { return mDir; }
  void setDir(AbsDir* v) { mDir = v; }

  ArgHandler* getHandler() const { return mHandler; }
  void setHandler(ArgHandler* v) { mHandler = v; }

  virtual void populatePromptBuffer(const std::string& s);
  virtual bool doValidate(const std::string& s);

private:
  ArgHandler* mHandler;
  AbsDir* mDir;
};
}

#endif /* PACINTRINSICARGHANDLER_H */
