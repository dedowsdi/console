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
  virtual ArgHandler* clone() { return new PriDeciArgHandler(*this); }

  PriDeciArgHandler(const std::string& name) : ArgHandler(name) {
    setPromptType(PT_PROMPTONLY);
  }

  virtual void populatePromptBuffer(const std::string& s) {
    (void)s;
    appendPromptBuffer(getName());
  }

protected:
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
  virtual ArgHandler* clone() { return new PriDeciRangeArgHandler(*this); }

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

protected:
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
  virtual ArgHandler* clone() { return new StringArgHandler(*this); }

  StringArgHandler(const std::string& name);
  StringArgHandler(
      const std::string& name, std::initializer_list<std::string> il);

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

protected:
  virtual bool doValidate(const std::string& s);

protected:
  StringSet mStrings;
};

/**
 * Enumerate type handler, you need to call DEFINE_ENUM_CONVERSION first before
 * you create arg handler for enum. It's name should start with en_ by
 * tridiation, it's regex formal will be re_en_
 */
template <typename T>
class _PacExport EnumArgHandler : public StringArgHandler {
public:
  EnumArgHandler(const std::string& name) : StringArgHandler(name) {
    if (EnumData<T>::empty())
      PAC_EXCEPT(Exception::ERR_INVALID_STATE,
          "Pls use DEFINE_ENUM_CONVERSION to define enum string conversion "
          "before you call registerEnumHandler");
    mStrings.insert(
        EnumData<T>::beginStringIter(), EnumData<T>::endStringIter());
  }
};

/**
 * Literial handler. It's name should start with ltl_ + text
 */
class _PacExport LiteralArgHandler : public ArgHandler {
public:
  LiteralArgHandler(const std::string& text);
  virtual ArgHandler* clone() { return new LiteralArgHandler(*this); }

  virtual void populatePromptBuffer(const std::string& s);

protected:
  virtual bool doValidate(const std::string& s);

protected:
  std::string mText;
};

/**
 * used at cmd taks no argument
 */
class _PacExport BlankArgHandler : public ArgHandler {
public:
  virtual ArgHandler* clone() { return new BlankArgHandler(*this); }

  BlankArgHandler();

  virtual void populatePromptBuffer(const std::string& s);

protected:
  virtual bool doValidate(const std::string& s);
};

/**
 * path
 */
class _PacExport PathArgHandler : public ArgHandler {
public:
  virtual ArgHandler* clone() { return new PathArgHandler(*this); }

  PathArgHandler();
  PathArgHandler(const PathArgHandler& rhs);

  AbsDir* getDir() const { return mDir; }
  void setDir(AbsDir* v) { mDir = v; }
  AbsDir* getPathDir() const { return mPathDir; }
  void setPathDir(AbsDir* v) { mPathDir = v; }

  virtual void populatePromptBuffer(const std::string& s);

protected:
  virtual bool doValidate(const std::string& s);
  virtual void completeTyping(const std::string& s);

private:
  AbsDir* mDir;      // cwd
  AbsDir* mPathDir;  // dir of path, avoid 2nd time findPath
};

/**
 * cmd
 */
class _PacExport CmdArgHandler : public StringArgHandler {
public:
  virtual ArgHandler* clone() { return new CmdArgHandler(*this); }

  CmdArgHandler();
};

/**
 * param handler. "path"  can be followed with a "param" handler
 */
class _PacExport ParamArgHandler : public StringArgHandler {
public:
  virtual ArgHandler* clone() { return new ParamArgHandler(*this); }

  ParamArgHandler();

  AbsDir* getDir() const { return mDir; }
  void setDir(AbsDir* v) { mDir = v; }

  virtual void runtimeInit();

protected:
  virtual void onLinked(Node* grandNode);

private:
  AbsDir* mDir;  // cwd
  Node* mPathNode;
};

/**
 * Deprecated. pparam handler. path + param. Takes following format
 * param( branch "0")
 * path param (branch "1")
 */
class _PacExport PparamArgHandler : public TreeArgHandler {
public:
  virtual ArgHandler* clone() { return new PparamArgHandler(*this); }

  PparamArgHandler();
  /**
   * Get param handler in matched branch
   * @return : param handler in matched branch
   */
  ParamArgHandler* getParamHandler();

private:
  AbsDir* mDir;  // cwd
};

/**
 * parameter value handler. Must follow param or pparam
 */
class _PacExport ValueArgHandler : public ArgHandler {
public:
  virtual ArgHandler* clone() { return new ValueArgHandler(*this); }
  ValueArgHandler();
  ValueArgHandler(const ValueArgHandler& rhs);

  void runtimeInit();

  AbsDir* getDir() const { return mDir; }
  void setDir(AbsDir* v) { mDir = v; }

  ArgHandler* getHandler() const { return mHandler; }
  void setHandler(ArgHandler* v) { mHandler = v; }

  /**
   * Get param handler from previous node.
   */
  ParamArgHandler* getParamHandler();

  virtual void populatePromptBuffer(const std::string& s);

protected:
  virtual bool doValidate(const std::string& s);

private:
  ArgHandler* mHandler;
  AbsDir* mDir;
};

/**
 * identifier
 */
class _PacExport IdArgHandler : public ArgHandler {
public:
  IdArgHandler();
  virtual ArgHandler* clone() { return new IdArgHandler(*this); }

protected:
  virtual bool doValidate(const std::string& s);
};

/**
 * regex
 */
class _PacExport RegexArgHandler : public ArgHandler {
public:
  RegexArgHandler() : ArgHandler("regex") {}
  virtual ArgHandler* clone() { return new RegexArgHandler(*this); }

  virtual void populatePromptBuffer(const std::string& s);

protected:
  virtual bool doValidate(const std::string& s) {
    (void)s;
    return true;
  };
};

/*
 * readonly
 */
class _PacExport ReadonlyArgHandler : public ArgHandler {
public:
  ReadonlyArgHandler();
  virtual ArgHandler* clone() { return new ReadonlyArgHandler(*this); }

  virtual void populatePromptBuffer(const std::string& s);

protected:
  virtual bool doValidate(const std::string& s) {
    (void)s;
    return false;
  };
};

/**
 * quaternion:
 * real real real real
 * ltl_angleAxis real real real real
 */
class _PacExport QuaternionArgHandler : public TreeArgHandler {
public:
  QuaternionArgHandler();
  virtual ArgHandler* clone() { return new QuaternionArgHandler(*this); }
  std::string getUniformValue() const;
};
}

#endif /* PACINTRINSICARGHANDLER_H */
