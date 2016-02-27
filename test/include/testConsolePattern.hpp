#ifndef TESTPACCONSOLEPATTERN_H
#define TESTPACCONSOLEPATTERN_H

#include "pacConsolePreRequisite.h"
#include "pacConsolePattern.h"
#include "pacStringUtil.h"
#include <gtest/gtest.h>

using namespace pac;

/**
 * test each item in the the same col has the same length, test if total length
 * overflow
 * @param s : output string from applyPattern
 */
void testOutputString(const std::string& s, size_t textWidth) {
  typedef std::vector<std::vector<std::string>> Grid;
  Grid grid;

  StringVector&& sv = StringUtil::split(s, "\n");
  std::for_each(sv.begin(), sv.end(), [&](const std::string& val) -> void {
    // std::cout << "\"" << val << "\"" << std::endl;
    StringVector&& tmp = StringUtil::split(val, " ", -1, true);
    grid.push_back(StringVector());
    // combine string and space
    for (StringVector::iterator iter = tmp.begin(); iter != tmp.end();
         iter += 2) {
      grid[grid.size() - 1].push_back(*iter + *(iter + 1));
    }
  });

  size_t numCol = grid[0].size();
  size_t numRow = grid.size();
  size_t totalColWidth = 0;

  for (size_t col = 0; col < numCol; ++col) {
    size_t colSize = grid[0][col].size();
    totalColWidth += colSize;

    for (size_t row = 0; row < numRow; ++row) {
      // at lest numCol -1 is full filled
      EXPECT_GE(grid[row].size(), numCol - 1);
      if (col == numCol - 1) {
        // last col might not full filled
        if (grid[row].size() == numCol - 1) {
          // reach 1 past last item, check the rest rows, they must
          // all lack last col
          for (size_t i = numRow + 1; i < numRow; ++i) {
            EXPECT_EQ(grid[i].size(), numCol - 1);
          }
          break;
        }
      }

      const std::string& val = grid[row][col];
      EXPECT_EQ(colSize, val.size());
      // at least 2 space in each item
      EXPECT_GT(val.size(), 2);
      EXPECT_STREQ("  ", val.substr(val.length() - 2).c_str());
    }
  }
  if (numCol > 1) EXPECT_LE(totalColWidth, textWidth);
}

/**
 * It'd better full test some private function
 */
class PimplDefaultPattern : public DefaultPattern {
public:
  PimplDefaultPattern(size_t textWidth) : DefaultPattern(textWidth) {}
  using DefaultPattern::getMinMaxItemLen;
  using DefaultPattern::getCandidateNumColumns;
  using DefaultPattern::valid;
  using DefaultPattern::mTextWidth;
};

/**
 * Fixture of default pattern with textWidth = 80
 */
class TestConsolePattern80 : public ::testing::Test {
protected:
  TestConsolePattern80() { mPattern = new PimplDefaultPattern(80); }
  virtual ~TestConsolePattern80() { delete mPattern; }

  virtual void SetUp() {}
  virtual void TearDown() {}

  /**
   * clean buffer, build buffer with number of item
   * @param item : basic item in buffer
   * @param numItem : number of items
   */
  void resetBuffer(const std::string& item, size_t numItem) {
    mBuffer.clear();
    for (size_t i = 0; i < numItem; ++i) {
      mBuffer.push_back(item);
    }
  }
  template <class _InputIterator>
  void resetBuffer(_InputIterator first, _InputIterator last) {
    mBuffer.assign(first, last);
  }

protected:
  PimplDefaultPattern* mPattern;
  StringVector mBuffer;
};

/**
 * test getCandidateNumColumn
 */
TEST_F(TestConsolePattern80, getCandidateNumColumns) {
  SizetVector sv;
  sv = mPattern->getCandidateNumColumns(1, 1, 1);
  ASSERT_EQ(1, sv.size());
  ASSERT_EQ(1, sv[0]);
  sv = mPattern->getCandidateNumColumns(1, 80, 80);
  ASSERT_EQ(1, sv.size());
  ASSERT_EQ(1, sv[0]);
  sv = mPattern->getCandidateNumColumns(10, 1, 78);
  ASSERT_EQ(1, sv.size());
  ASSERT_EQ(1, sv[0]);
  sv = mPattern->getCandidateNumColumns(1, 1, 80);
  ASSERT_EQ(1, sv.size());
  ASSERT_EQ(1, sv[0]);
  sv = mPattern->getCandidateNumColumns(80, 2,
      75);  // min + max  + 2* space > 80
  ASSERT_EQ(1, sv.size());
  ASSERT_EQ(1, sv[0]);
  sv = mPattern->getCandidateNumColumns(80, 8, 69);  // min + max + 2*space > 80
  ASSERT_EQ(1, sv.size());
  ASSERT_EQ(1, sv[0]);
  sv = mPattern->getCandidateNumColumns(12, 8,
      20);  // normal case, numItem > maxCol
  ASSERT_EQ(5, sv.size());
  size_t s[] = {1, 2, 3, 4, 6};
  for (size_t i = 0; i < sv.size(); ++i) {
    ASSERT_EQ(s[i], sv[i]);
  }
  sv = mPattern->getCandidateNumColumns(8, 8,
      20);  // normal case, numItem = maxCol
  ASSERT_EQ(5, sv.size());
  size_t s1[] = {1, 2, 3, 4, 8};
  for (size_t i = 0; i < sv.size(); ++i) {
    ASSERT_EQ(s1[i], sv[i]);
  }
  sv = mPattern->getCandidateNumColumns(9, 6,
      20);  // normal case, numItem < maxCol
  ASSERT_EQ(5, sv.size());
  size_t s2[] = {1, 2, 3, 5, 9};
  for (size_t i = 0; i < sv.size(); ++i) {
    ASSERT_EQ(s2[i], sv[i]);
  }
}

/**
 * test single item
 */
TEST_F(TestConsolePattern80, 1item) {
  resetBuffer("1", 1);
  const std::string&& s =
      mPattern->applyPattern(mBuffer.begin(), mBuffer.end());
  size_t numLine = std::count(s.begin(), s.end(), '\n');
  EXPECT_EQ(1, numLine);
  EXPECT_EQ(4, s.size());  // 2 spacing
  EXPECT_STREQ("\n1  ", s.c_str());
  testOutputString(s, mPattern->mTextWidth);
}

/**
 * test single item which is bigger than 80
 */
TEST_F(TestConsolePattern80, 1tooBigItem) {
  std::string item(81, 'x');
  resetBuffer(item, 1);
  const std::string&& s =
      mPattern->applyPattern(mBuffer.begin(), mBuffer.end());
  size_t numLine = std::count(s.begin(), s.end(), '\n');
  EXPECT_EQ(1, numLine);
  EXPECT_EQ(84, s.size());
  EXPECT_STREQ(("\n" + item + "  ").c_str(), s.c_str());
  testOutputString(s, mPattern->mTextWidth);
}

/**
 * test n items with 1 too bigger item  which is bigger than 80
 */
TEST_F(TestConsolePattern80, 10with1tooBigItem) {
  std::string item(81, 'x');
  resetBuffer("abcde", 9);
  mBuffer.push_back(item);

  const std::string&& s =
      mPattern->applyPattern(mBuffer.begin(), mBuffer.end());
  size_t numLine = std::count(s.begin(), s.end(), '\n');
  EXPECT_EQ(10, numLine);
  testOutputString(s, mPattern->mTextWidth);
}

/**
 * 5 items in 1 line
 */
TEST_F(TestConsolePattern80, 5ItemOneLine) {
  resetBuffer("abcde", 5);

  const std::string&& s =
      mPattern->applyPattern(mBuffer.begin(), mBuffer.end());
  size_t numLine = std::count(s.begin(), s.end(), '\n');
  EXPECT_EQ(1, numLine);
  EXPECT_EQ(5 * 5 + 2 * 5 + 1, s.size());
  EXPECT_STREQ("\nabcde  abcde  abcde  abcde  abcde  ", s.c_str());
  testOutputString(s, mPattern->mTextWidth);
}

/**
 * 20 items in 2 line
 */
TEST_F(TestConsolePattern80, 16ItemTwoLine) {
  resetBuffer("abcdefgh", 16);

  const std::string&& s =
      mPattern->applyPattern(mBuffer.begin(), mBuffer.end());
  size_t numLine = std::count(s.begin(), s.end(), '\n');
  EXPECT_EQ(2, numLine);
  EXPECT_EQ(8 * 16 + 2 * 16 + 2, s.size());
  std::stringstream ss;
  for (int i = 0; i < 16; ++i) {
    if (i % 8 == 0) ss << "\n";
    ss << "abcdefgh  ";
  }
  EXPECT_STREQ(ss.str().c_str(), s.c_str());
  testOutputString(s, mPattern->mTextWidth);
}

/**
 * factorial of 20 of 'x'. get result, separate by row and column, check if each
 * item in the same column is of the same length
 */
TEST_F(TestConsolePattern80, 20factorialOfx) {
  for (int i = 1; i < 21; ++i) {
    std::string s(i, 'x');
    mBuffer.push_back(s);
  }
  // v[row[col]]
  typedef std::vector<std::vector<std::string>> Grid;
  Grid grid;

  const std::string&& s =
      mPattern->applyPattern(mBuffer.begin(), mBuffer.end());
  testOutputString(s, mPattern->mTextWidth);
}

/**
 * random num item ,random item len
 */
TEST_F(TestConsolePattern80, 1000timesRandom) {
  size_t maxItemLen = 40, maxNumItem = 100;
  srand((unsigned)time(NULL));
  for (int i = 0; i < 1000; ++i) {
    StringVector buffer;
    size_t numItem = std::rand() % maxNumItem + 1;
    while (numItem--) {
      std::string s(std::rand() % maxItemLen + 1, 'x');
      buffer.push_back(s);
    }
    resetBuffer(buffer.begin(), buffer.end());
    testOutputString(mPattern->applyPattern(mBuffer.begin(), mBuffer.end()),
        mPattern->mTextWidth);
  }
}

#endif  // TESTPACCONSOLEPATTERN_H
