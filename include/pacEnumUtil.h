#ifndef PACENUMUTIL_H
#define PACENUMUTIL_H

#include "pacConsolePreRequisite.h"
#include "pacException.h"
#include <boost/preprocessor.hpp>
namespace pac {

// This is the type that will hold all the strings.  Each enumerate type will
// declare its own specialization.  Any enum that does not have a specialization
// will generate a compiler error indicating that there is no definition of this
// variable (as there should be be no definition of a generic version). I save
// both enum and it's string value to make it easy to handle special enum which
// has unsorted value.
template <typename T>
class EnumData {
public:
  static std::vector<std::string>::const_iterator beginStringIter() {
    PacAssert(mStrings.size() == mEnums.size(), "unequal size");
    return mStrings.begin();
  }
  static std::vector<std::string>::const_iterator endStringIter() {
    PacAssert(mStrings.size() == mEnums.size(), "unequal size");
    return mStrings.end();
  }
  static typename std::vector<T>::const_iterator beginEnumIter() {
    PacAssert(mStrings.size() == mEnums.size(), "unequal size");
    return mEnums.begin();
  }
  static typename std::vector<T>::const_iterator endEnumIter() {
    PacAssert(mStrings.size() == mEnums.size(), "unequal size");
    return mEnums.end();
  }
  static const std::string& getEnumStringAt(size_t i) { return mStrings[i]; }
  static T getEnumAt(size_t i) { return mEnums[i]; }
  static T randomEnum() { return mEnums[rand() % mEnums.size()]; }
  static const std::string& randomEnumStr() {
    return mStrings[rand() % mStrings.size()];
  }
  static size_t size() { return mEnums.size(); }
  static bool empty() { return mEnums.empty(); }

private:
  // these vectors should not be changed after initialization
  static const std::vector<std::string> mStrings;
  static const std::vector<T> mEnums;
};

// change (a) to ("a"), ( and ) need to be reserved
#define OP_TO_STRING(s, data, elem) (OP_TO_STRING_I(data, elem))
// make sure elem got expanded before stringize it
#define OP_TO_STRING_I(data, elem) #elem

// combine scope and elem
#define OP_TO_SCOPE(r, scope, elem) (TO_SCOPE(scope, elem))
#define TO_SCOPE(scope, elem) TO_SCOPE_I(scope, elem)
// don't use ##, scope is not a valid token, it always ends with ::
#define TO_SCOPE_I(scope, elem) scope elem

// define enum and it's convertions, leave scope to empty if you don't need it.
// scope must ends with :: if it's not empty.  you can call it like
// DEFINE_ENUM_WITH_CONVERSION(scope, enum type, (enum0)(enum1)(enum2) )
#define DEFINE_ENUM_WITH_CONVERSION(scope, name, enumerators) \
  enum name { BOOST_PP_SEQ_ENUM(enumerators) };               \
  DEFINE_ENUM_CONVERSION(scope, name, enumerators)

// define convertions of existing enum, leave scope to empty if you don't need
// it.  Scope must ends with :: if it's not empty.  You can call it like
// DEFINE_ENUM_WITH_CONVERSION(scope, enum type, (enum0)(enum1)(enum2) )
#define DEFINE_ENUM_CONVERSION(scope, name, enumerators)                    \
  template <>                                                               \
  const std::vector<TO_SCOPE(scope, name)>                                  \
      EnumData<TO_SCOPE(scope, name)>::mEnums({BOOST_PP_SEQ_ENUM(           \
          BOOST_PP_SEQ_FOR_EACH(OP_TO_SCOPE, scope, enumerators))});        \
  template <>                                                               \
  const std::vector<std::string> EnumData<TO_SCOPE(scope, name)>::mStrings( \
      {BOOST_PP_SEQ_ENUM(                                                   \
          BOOST_PP_SEQ_FOR_EACH(OP_TO_STRING, _, enumerators))});

template <typename T>
const std::string& enumToString(const T& e) {
  auto first = EnumData<T>::beginEnumIter();
  auto last = EnumData<T>::endEnumIter();
  auto iter = std::find(first, last, e);
  assert(iter != last);
  return EnumData<T>::getEnumStringAt(iter - first);
}

template <typename T>
T enumFromString(const std::string& s) {
  auto first = EnumData<T>::beginStringIter();
  auto last = EnumData<T>::endStringIter();
  auto iter = std::find(first, last, s);
  assert(iter != last);
  return EnumData<T>::getEnumAt(iter - first);
}
}

#endif /* PACENUMUTIL_H */
