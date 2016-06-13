#ifndef TESTPACSTRINGUTIL_H
#define TESTPACSTRINGUTIL_H

#include "pacStringUtil.h"
#include <gtest/gtest.h>
using namespace pac;

TEST(StringUtil_join, randomSep) {
  srand(time(NULL));
  size_t numTests = 50;
  while (numTests--) {
    size_t numItems = rand() % 50 + 1;
    StringVector values;

    for (size_t i = 0; i < numItems; ++i) {
      values.push_back(StringUtil::toString(i));
    }
    char sepChar = rand() % 256;
    std::string sep(1, sepChar);

    const std::string&& s = StringUtil::join(values, sep);

    // use a different way from StringUtil::join to create manual
    std::string manual;
    std::for_each(values.begin(), values.end(),
        [&](const std::string& v)
            -> void { manual += manual.empty() ? v : sep + v; });

    ASSERT_EQ(manual, s);
  }
}

TEST(StringUtil_getHead, blank) {
  ASSERT_STREQ("", StringUtil::getHead("").c_str());
}
TEST(StringUtil_getHead, delimOnly) {
  ASSERT_EQ(pac::delim, StringUtil::getHead(pac::delim));
}
TEST(StringUtil_getHead, relativeWithNoDelim) {
  ASSERT_STREQ(".", StringUtil::getHead("abcd").c_str());
}
TEST(StringUtil_getHead, relativeWithDelim) {
  ASSERT_STREQ(
      "abcd", StringUtil::getHead("abcd" + pac::delim + "efg").c_str());
}
TEST(StringUtil_getHead, absolute1Delim) {
  ASSERT_EQ(pac::delim, StringUtil::getHead(pac::delim + "abcd.ext"));
}
TEST(StringUtil_getHead, absolute2Delim) {
  ASSERT_EQ(pac::delim + "abcd",
      StringUtil::getHead(pac::delim + "abcd" + pac::delim + "efg"));
}

TEST(StringUtil_getTail, blank) {
  ASSERT_STREQ("", StringUtil::getTail("").c_str());
}
TEST(StringUtil_getTail, delimOnly) {
  ASSERT_EQ("", StringUtil::getTail(pac::delim));
}
TEST(StringUtil_getTail, relativeWithNoDelim) {
  ASSERT_STREQ("abcd", StringUtil::getTail("abcd").c_str());
}
TEST(StringUtil_getTail, relativeWithDelim) {
  ASSERT_STREQ("efg", StringUtil::getTail("abcd" + pac::delim + "efg").c_str());
}
TEST(StringUtil_getTail, absolute1Delim) {
  ASSERT_STREQ(
      "abcd.ext", StringUtil::getTail(pac::delim + "abcd.ext").c_str());
}
TEST(StringUtil_getTail, absolute2Delim) {
  ASSERT_STREQ("efg",
      StringUtil::getTail(pac::delim + "abcd" + pac::delim + "efg").c_str());
}

#endif  // TESTPACSTRINGUTIL_H
