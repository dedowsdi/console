#ifndef PACSTDUTIL_H
#define PACSTDUTIL_H

#include "pacConsolePreRequisite.h"
#include <boost/regex.hpp>

namespace pac {

class StdUtil {
private:
  StdUtil() {}

public:
  /**
   * Get kyes of a map
   * @param t : map
   * @return : vector of map keys
   */
  template <class T>
  static std::vector<typename T::key_type> keys(const T& t) {
    std::vector<typename T::key_type> v;
    std::for_each(t.begin(), t.end(),
        [&](typename T::value_type val) -> void { v.push_back(val.first); });

    return v;
  }

  template <class T>
  static bool exist(const T& t, const typename T::value_type& v) {
    return std::find(t.begin(), t.end(), v) != t.end();
  }

  /**
   * Get max identical string(from beginning) from string vector .
   */
  static std::string getIdenticalString(
      StringVector::iterator beg, StringVector::iterator end);
};

namespace fo {

class RegexMatch {
public:
  RegexMatch(const std::string& regexExp)
      : mString(regexExp), mRegex(regexExp) {}

  bool operator()(const std::string& s) {
    return boost::regex_match(s, mMatch, mRegex);
  }

private:
  std::string mString;
  boost::regex mRegex;
  boost::smatch mMatch;
};
}
}

#endif /* PACSTDUTIL_H */
