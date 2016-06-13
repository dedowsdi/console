#include "pacStable.h"
#include "pacConsolePattern.h"
#include <iostream>
#include "pacLogger.h"

namespace pac {

//------------------------------------------------------------------------------
ConsolePattern::ConsolePattern(size_t textWidth) : mTextWidth(textWidth) {}

//------------------------------------------------------------------------------
DefaultPattern::DefaultPattern(size_t textWidth, size_t spacing)
    : ConsolePattern(textWidth), mSpacing(spacing) {
  sgLogger.logMessage(
      "init console pattern text width to " + StringUtil::toString(textWidth));
}

//------------------------------------------------------------------------------
std::string DefaultPattern::applyPattern(SVIter beg, SVIter end) {
  std::sort(beg, end);
  size_t numCol = 0, minItemLen = -1, maxItemLen = 0;
  SizetVector colWidthes;
  size_t numItem = end - beg;
  getMinMaxItemLen(beg, end, minItemLen, maxItemLen);
  SizetVector&& candidates =
      getCandidateNumColumns(numItem, minItemLen, maxItemLen);
  if (candidates.size() == 1 &&
      candidates[0] == 1)  // crazy case, 1 column only
  {
    std::stringstream ss;
    std::for_each(beg, end, [&](const std::string& v) -> void {
      ss << StringUtil::toFixedLength(v, maxItemLen + mSpacing) << "\n";
    });
    return ss.str();
  } else  // find max column number
  {
    // loop from biggest one
    for (SizetVector::reverse_iterator iter = candidates.rbegin();
         iter != candidates.rend(); ++iter) {
      if (valid(beg, end, *iter, colWidthes)) {
        numCol = *iter;
        break;
      }
    }

    if (numCol == 0) {
      PAC_EXCEPT(Exception::ERR_INVALID_STATE, "found no valid column number");
    }

    size_t numRow = ceil(static_cast<Real>(numItem) / numCol);
    size_t numGrid = numRow * colWidthes.size();

    // output horizental. Becareful, item seq in mBuffers is vertical.
    std::stringstream ss;
    for (size_t i = 0; i != numGrid; ++i) {
      size_t col = i % numCol;
      size_t row = i / numCol;
      size_t itemIndex = col * numRow + row;
      if (itemIndex >= numItem) {
        // meet empty grid in last col
        ss << "\n";
        continue;
      }
      ss << StringUtil::toFixedLength(*(beg + itemIndex), colWidthes[col]);
      // line break at last column
      if (col == numCol - 1) ss << "\n";
    }

    return ss.str();
  }
}

//------------------------------------------------------------------------------
void DefaultPattern::getMinMaxItemLen(
    SVIter beg, SVIter end, size_t& minItemLen, size_t& maxItemLen) {
  PacAssert(beg != end, "Nothing go work on");
  minItemLen = maxItemLen = beg->size();

  std::for_each(beg, end, [&](const std::string& v) -> void {
    if (v.length() > maxItemLen) maxItemLen = v.length();
    if (v.length() < minItemLen) minItemLen = v.length();
  });
}

//------------------------------------------------------------------------------
SizetVector DefaultPattern::getCandidateNumColumns(
    size_t numItem, size_t minItemLen, size_t maxItemLen) {
  if (maxItemLen < minItemLen)
    PAC_EXCEPT(
        Exception::ERR_INVALIDPARAMS, "maxItemLen smaller than minItemLen!");
  SizetVector res;
  res.push_back(1);

  // one column case
  if (mTextWidth < (maxItemLen + minItemLen + mSpacing * 2) || numItem == 1)
    return res;

  // col num will not between ceil(float(numItem)/2) and numItem
  size_t maxNumCol = std::min(mTextWidth / (minItemLen + mSpacing),
      static_cast<size_t>(ceil(static_cast<Real>(numItem) / 2)));

  // multi column case
  for (size_t numCol = 2; numCol <= maxNumCol; ++numCol) {
    size_t numRow = ceil(static_cast<Real>(numItem) / numCol);
    // check if every column except last one is full filled
    if ((numCol - 1) * numRow < numItem) {
      res.push_back(numCol);
    }
  }

  // numItem might = col number
  if (numItem <= mTextWidth / (minItemLen + mSpacing)) res.push_back(numItem);
  return res;
}

//------------------------------------------------------------------------------
bool DefaultPattern::valid(
    SVIter beg, SVIter end, size_t numCol, SizetVector& colWidthes) {
  size_t numItem = end - beg;

  size_t numRow = ceil(static_cast<Real>(numItem) / numCol);
  size_t totalColWidth = 0;
  size_t minItemLen, maxItemLen;

  colWidthes.clear();

  // loop column, get length of each max item in column
  for (size_t i = 0; i < numCol; ++i) {
    SVIter first = beg + numRow * i;
    SVIter last = i == numCol - 1 ? end : first + numRow;

    getMinMaxItemLen(first, last, minItemLen, maxItemLen);
    size_t w = maxItemLen + mSpacing;
    colWidthes.push_back(w);
    totalColWidth += w;
  }

  // total length check
  return totalColWidth <= mTextWidth;
}
}
