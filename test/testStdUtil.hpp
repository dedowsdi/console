#ifndef TESTPACSTDUTIL_H
#define TESTPACSTDUTIL_H

#include "pacStdUtil.h"
#include <gtest/gtest.h>

using namespace pac;

TEST(StdUtili_keys, 100timesIntInt) {
  srand(time(NULL));
  size_t numTests = 100;
  while (numTests--) {
    size_t numItems = rand() % 50 + 1;
    std::map<int, int> m;
    std::set<int> keys;

    for (size_t i = 0; i < numItems; ++i) {
      keys.insert(rand());
    }

    std::for_each(
        keys.begin(), keys.end(), [&](int v) -> void { m[v] = rand(); });

    std::vector<int> kv = StdUtil::keys(m);
    std::for_each(kv.begin(), kv.end(),
        [&](int v) -> void { ASSERT_TRUE(StdUtil::exist(keys, v)); });
  }
}

#endif /* TESTPACSTDUTIL_H */
