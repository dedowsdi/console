#include "pacStringUtil.h"

namespace pac {

//-----------------------------------------------------------------------
const std::string StringUtil::BLANK;
std::string StringUtil::msDefaultStringLocale = "C";
std::locale StringUtil::msLocale = std::locale(msDefaultStringLocale.c_str());
bool StringUtil::msUseLocale = false;

//-----------------------------------------------------------------------
void StringUtil::trim(std::string& str, bool left, bool right) {
  /*
  size_t lspaces, rspaces, len = length(), i;

  lspaces = rspaces = 0;

  if( left )
  {
      // Find spaces / tabs on the left
      for( i = 0;
    i < len && ( at(i) == ' ' || at(i) == '\t' || at(i) == '\r');
    ++lspaces, ++i );
  }

  if( right && lspaces < len )
  {
      // Find spaces / tabs on the right
      for( i = len - 1;
    i >= 0 && ( at(i) == ' ' || at(i) == '\t' || at(i) == '\r');
    rspaces++, i-- );
  }

  *this = substr(lspaces, len-lspaces-rspaces);
  */
  static const std::string delims = " \t\r";
  if (right) str.erase(str.find_last_not_of(delims) + 1);  // trim right
  if (left) str.erase(0, str.find_first_not_of(delims));   // trim left
}

//-----------------------------------------------------------------------
StringVector StringUtil::split(const std::string& str,
    const std::string& delims, unsigned int maxSplits, bool preserveDelims) {
  StringVector ret;
  // Pre-allocate some space for performance
  ret.reserve(
      maxSplits ? maxSplits + 1 : 10);  // 10 is guessed capacity for most case

  unsigned int numSplits = 0;

  // Use STL methods
  size_t start, pos;
  start = 0;
  do {
    pos = str.find_first_of(delims, start);
    if (pos == start) {
      // Do nothing
      start = pos + 1;
    } else if (pos == std::string::npos ||
               (maxSplits && numSplits == maxSplits)) {
      // Copy the rest of the string
      ret.push_back(str.substr(start));
      break;
    } else {
      // Copy up to delimiter
      ret.push_back(str.substr(start, pos - start));

      if (preserveDelims) {
        // Sometimes there could be more than one delimiter in a row.
        // Loop until we don't find any more delims
        size_t delimStart = pos, delimPos;
        delimPos = str.find_first_not_of(delims, delimStart);
        if (delimPos == std::string::npos) {
          // Copy the rest of the string
          ret.push_back(str.substr(delimStart));
        } else {
          ret.push_back(str.substr(delimStart, delimPos - delimStart));
        }
      }

      start = pos + 1;
    }
    // parse up to next real data
    start = str.find_first_not_of(delims, start);
    ++numSplits;

  } while (pos != std::string::npos);

  return ret;
}
//-----------------------------------------------------------------------
StringVector StringUtil::tokenise(const std::string& str,
    const std::string& singleDelims, const std::string& doubleDelims,
    unsigned int maxSplits) {
  StringVector ret;
  // Pre-allocate some space for performance
  ret.reserve(
      maxSplits ? maxSplits + 1 : 10);  // 10 is guessed capacity for most case

  unsigned int numSplits = 0;
  std::string delims = singleDelims + doubleDelims;

  // Use STL methods
  size_t start, pos;
  char curDoubleDelim = 0;
  start = 0;
  do {
    if (curDoubleDelim != 0) {
      pos = str.find(curDoubleDelim, start);
    } else {
      pos = str.find_first_of(delims, start);
    }

    if (pos == start) {
      char curDelim = str.at(pos);
      if (doubleDelims.find_first_of(curDelim) != std::string::npos) {
        curDoubleDelim = curDelim;
      }
      // Do nothing
      start = pos + 1;
    } else if (pos == std::string::npos ||
               (maxSplits && numSplits == maxSplits)) {
      if (curDoubleDelim != 0) {
        // Missing closer. Warn or throw exception?
      }
      // Copy the rest of the string
      ret.push_back(str.substr(start));
      break;
    } else {
      if (curDoubleDelim != 0) {
        curDoubleDelim = 0;
      }

      // Copy up to delimiter
      ret.push_back(str.substr(start, pos - start));
      start = pos + 1;
    }
    if (curDoubleDelim == 0) {
      // parse up to next real data
      start = str.find_first_not_of(singleDelims, start);
    }

    ++numSplits;

  } while (start != std::string::npos);

  return ret;
}
//-----------------------------------------------------------------------
void StringUtil::toLowerCase(std::string& str) {
  std::transform(str.begin(), str.end(), str.begin(), tolower);
}

//-----------------------------------------------------------------------
void StringUtil::toUpperCase(std::string& str) {
  std::transform(str.begin(), str.end(), str.begin(), toupper);
}
//-----------------------------------------------------------------------
bool StringUtil::startsWith(
    const std::string& str, const std::string& pattern, bool lowerCase) {
  size_t thisLen = str.length();
  size_t patternLen = pattern.length();
  if (thisLen < patternLen || patternLen == 0) return false;

  std::string startOfThis = str.substr(0, patternLen);
  if (lowerCase) {
    std::string lowerCasePattern = pattern;
    StringUtil::toLowerCase(lowerCasePattern);
    StringUtil::toLowerCase(startOfThis);
    return (startOfThis == lowerCasePattern);
  }

  return (startOfThis == pattern);
}
//-----------------------------------------------------------------------
bool StringUtil::endsWith(
    const std::string& str, const std::string& pattern, bool lowerCase) {
  size_t thisLen = str.length();
  size_t patternLen = pattern.length();
  if (thisLen < patternLen || patternLen == 0) return false;

  std::string endOfThis = str.substr(thisLen - patternLen, patternLen);
  if (lowerCase) {
    std::string lowerCasePattern = pattern;
    StringUtil::toLowerCase(lowerCasePattern);
    StringUtil::toLowerCase(endOfThis);
    return (endOfThis == lowerCasePattern);
  }

  return (endOfThis == pattern);
}
//-----------------------------------------------------------------------
std::string StringUtil::standardisePath(const std::string& init) {
  std::string path = init;

  std::replace(path.begin(), path.end(), '\\', '/');
  if (path[path.length() - 1] != '/') path += '/';

  return path;
}
//-----------------------------------------------------------------------
std::string StringUtil::normalizeFilePath(
    const std::string& init, bool makeLowerCase) {
  const char* bufferSrc = init.c_str();
  int pathLen = (int)init.size();
  int indexSrc = 0;
  int indexDst = 0;
  int metaPathArea = 0;

  char reservedBuf[1024];
  char* bufferDst = reservedBuf;
  bool isDestAllocated = false;
  if (pathLen > 1023) {
    // if source path is to long ensure we don't do a buffer overrun by
    // allocating some
    // new memory
    isDestAllocated = true;
    bufferDst = new char[pathLen + 1];
  }

  // The outer loop loops over directories
  while (indexSrc < pathLen) {
    if ((bufferSrc[indexSrc] == '\\') || (bufferSrc[indexSrc] == '/')) {
      // check if we have a directory delimiter if so skip it (we should already
      // have written such a delimiter by this point
      ++indexSrc;
      continue;
    } else {
      // check if there is a directory to skip of type ".\"
      if ((bufferSrc[indexSrc] == '.') &&
          ((bufferSrc[indexSrc + 1] == '\\') ||
              (bufferSrc[indexSrc + 1] == '/'))) {
        indexSrc += 2;
        continue;
      }

      // check if there is a directory to skip of type "..\"
      else if ((bufferSrc[indexSrc] == '.') &&
               (bufferSrc[indexSrc + 1] == '.') &&
               ((bufferSrc[indexSrc + 2] == '\\') ||
                   (bufferSrc[indexSrc + 2] == '/'))) {
        if (indexDst > metaPathArea) {
          // skip a directory backward in the destination path
          do {
            --indexDst;
          } while (
              (indexDst > metaPathArea) && (bufferDst[indexDst - 1] != '/'));
          indexSrc += 3;
          continue;
        } else {
          // we are about to write "..\" to the destination buffer
          // ensure we will not remove this in future "skip directories"
          metaPathArea += 3;
        }
      }
    }

    // transfer the current directory name from the source to the destination
    while (indexSrc < pathLen) {
      char curChar = bufferSrc[indexSrc];
      if (makeLowerCase) curChar = tolower(curChar);
      if ((curChar == '\\') || (curChar == '/')) curChar = '/';
      bufferDst[indexDst] = curChar;
      ++indexDst;
      ++indexSrc;
      if (curChar == '/') break;
    }
  }
  bufferDst[indexDst] = 0;

  std::string normalized(bufferDst);
  if (isDestAllocated) {
    delete[] bufferDst;
  }

  return normalized;
}
//-----------------------------------------------------------------------
void StringUtil::splitFilename(const std::string& qualifiedName,
    std::string& outBasename, std::string& outPath) {
  std::string path = qualifiedName;
  // Replace \ with / first
  std::replace(path.begin(), path.end(), '\\', '/');
  // split based on final /
  size_t i = path.find_last_of('/');

  if (i == std::string::npos) {
    outPath.clear();
    outBasename = qualifiedName;
  } else {
    outBasename = path.substr(i + 1, path.size() - i - 1);
    outPath = path.substr(0, i + 1);
  }
}

//------------------------------------------------------------------
std::string StringUtil::join(
    const StringVector& sv, const std::string& sep /*= " "*/) {
  return join(sv.begin(), sv.end(), sep);
}

//------------------------------------------------------------------
std::string StringUtil::join(
    SVCIter first, SVCIter last, const std::string& sep /*= " "*/) {
  if (first == last) return "";

  StringStream ss;
  ss << *first;
  std::for_each(
      first + 1, last, [&](const std::string& v) -> void { ss << sep << v; });

  return ss.str();
}

//------------------------------------------------------------------
std::string StringUtil::toFixedLength(
    const std::string& s, size_t length, char c /*= ' '*/) {
  // do nothing if it's long enough
  if (s.size() >= length) return s;

  std::string s1(length - s.size(), c);

  return s + s1;
}
//-----------------------------------------------------------------------
void StringUtil::splitBaseFilename(const std::string& fullName,
    std::string& outBasename, std::string& outExtention) {
  size_t i = fullName.find_last_of(".");
  if (i == std::string::npos) {
    outExtention.clear();
    outBasename = fullName;
  } else {
    outExtention = fullName.substr(i + 1);
    outBasename = fullName.substr(0, i);
  }
}
// ----------------------------------------------------------------------------------------------------------------------------------------------
void StringUtil::splitFullFilename(const std::string& qualifiedName,
    std::string& outBasename, std::string& outExtention, std::string& outPath) {
  std::string fullName;
  splitFilename(qualifiedName, fullName, outPath);
  splitBaseFilename(fullName, outBasename, outExtention);
}
//-----------------------------------------------------------------------
bool StringUtil::match(
    const std::string& str, const std::string& pattern, bool caseSensitive) {
  std::string tmpStr = str;
  std::string tmpPattern = pattern;
  if (!caseSensitive) {
    StringUtil::toLowerCase(tmpStr);
    StringUtil::toLowerCase(tmpPattern);
  }

  std::string::const_iterator strIt = tmpStr.begin();
  std::string::const_iterator patIt = tmpPattern.begin();
  std::string::const_iterator lastWildCardIt = tmpPattern.end();
  while (strIt != tmpStr.end() && patIt != tmpPattern.end()) {
    if (*patIt == '*') {
      lastWildCardIt = patIt;
      // Skip over looking for next character
      ++patIt;
      if (patIt == tmpPattern.end()) {
        // Skip right to the end since * matches the entire rest of the string
        strIt = tmpStr.end();
      } else {
        // scan until we find next pattern character
        while (strIt != tmpStr.end() && *strIt != *patIt) ++strIt;
      }
    } else {
      if (*patIt != *strIt) {
        if (lastWildCardIt != tmpPattern.end()) {
          // The last wildcard can match this incorrect sequence
          // rewind pattern to wildcard and keep searching
          patIt = lastWildCardIt;
          lastWildCardIt = tmpPattern.end();
        } else {
          // no wildwards left
          return false;
        }
      } else {
        ++patIt;
        ++strIt;
      }
    }
  }
  // If we reached the end of both the pattern and the string, we succeeded
  if (patIt == tmpPattern.end() && strIt == tmpStr.end()) {
    return true;
  } else {
    return false;
  }
}
//-----------------------------------------------------------------------
const std::string StringUtil::replaceAll(const std::string& source,
    const std::string& replaceWhat, const std::string& replaceWithWhat) {
  std::string result = source;
  std::string::size_type pos = 0;
  while (1) {
    pos = result.find(replaceWhat, pos);
    if (pos == std::string::npos) break;
    result.replace(pos, replaceWhat.size(), replaceWithWhat);
    pos += replaceWithWhat.size();
  }
  return result;
}

//------------------------------------------------------------------
std::string StringUtil::getHead(const std::string& path) {
  if (path.empty()) return path;

  if (path == pac::delim) return pac::delim;

  int pos = path.find_last_of(pac::delim);
  if (pos == -1)
    return ".";
  else if (pos == 0)
    return pac::delim;
  else
    return path.substr(0, pos);
}

//------------------------------------------------------------------
std::string StringUtil::getTail(const std::string& path) {
  if (path.empty()) return "";

  int pos = path.find_last_of(pac::delim);

  if (pos == -1) {
    return path;
  } else {
    return path.substr(pos + 1);
  }
}

//-----------------------------------------------------------------------
std::string StringUtil::toString(Real val, unsigned short precision,
    unsigned short width, char fill, std::ios::fmtflags flags) {
  StringStream stream;
  if (msUseLocale) stream.imbue(msLocale);
  stream.precision(precision);
  stream.width(width);
  stream.fill(fill);
  if (flags) stream.setf(flags);
  stream << val;

  return stream.str();
}
#if PAC_DOUBLE_PRECISION == 1
//-----------------------------------------------------------------------
std::string StringUtil::toString(float val, unsigned short precision,
    unsigned short width, char fill, std::ios::fmtflags flags) {
  StringStream stream;
  if (msUseLocale) stream.imbue(msLocale);
  stream.precision(precision);
  stream.width(width);
  stream.fill(fill);
  if (flags) stream.setf(flags);
  stream << val;
  return stream.str();
}
#else
//-----------------------------------------------------------------------
std::string StringUtil::toString(double val, unsigned short precision,
    unsigned short width, char fill, std::ios::fmtflags flags) {
  StringStream stream;
  if (msUseLocale) stream.imbue(msLocale);
  stream.precision(precision);
  stream.width(width);
  stream.fill(fill);
  if (flags) stream.setf(flags);
  stream << val;
  return stream.str();
}
#endif
//-----------------------------------------------------------------------
std::string StringUtil::toString(
    int val, unsigned short width, char fill, std::ios::fmtflags flags) {
  StringStream stream;
  if (msUseLocale) stream.imbue(msLocale);
  stream.width(width);
  stream.fill(fill);
  if (flags) stream.setf(flags);
  stream << val;
  return stream.str();
}
//-----------------------------------------------------------------------
#if PAC_PLATFORM != PAC_PLATFORM_NACL &&      \
    (PAC_ARCH_TYPE == PAC_ARCHITECTURE_64 ||  \
        PAC_PLATFORM == PAC_PLATFORM_APPLE || \
        PAC_PLATFORM == PAC_PLATFORM_APPLE_IOS)
std::string StringUtil::toString(unsigned int val, unsigned short width,
    char fill, std::ios::fmtflags flags) {
  StringStream stream;
  if (msUseLocale) stream.imbue(msLocale);
  stream.width(width);
  stream.fill(fill);
  if (flags) stream.setf(flags);
  stream << val;
  return stream.str();
}
//-----------------------------------------------------------------------
std::string StringUtil::toString(
    size_t val, unsigned short width, char fill, std::ios::fmtflags flags) {
  StringStream stream;
  if (msUseLocale) stream.imbue(msLocale);
  stream.width(width);
  stream.fill(fill);
  if (flags) stream.setf(flags);
  stream << val;
  return stream.str();
}
#if PAC_COMPILER == PAC_COMPILER_MSVC
//-----------------------------------------------------------------------
std::string StringUtil::toString(unsigned long val, unsigned short width,
    char fill, std::ios::fmtflags flags) {
  StringStream stream;
  if (msUseLocale) stream.imbue(msLocale);
  stream.width(width);
  stream.fill(fill);
  if (flags) stream.setf(flags);
  stream << val;
  return stream.str();
}

#endif
//-----------------------------------------------------------------------
#else
std::string StringUtil::toString(
    size_t val, unsigned short width, char fill, std::ios::fmtflags flags) {
  StringStream stream;
  if (msUseLocale) stream.imbue(msLocale);
  stream.width(width);
  stream.fill(fill);
  if (flags) stream.setf(flags);
  stream << val;
  return stream.str();
}
//-----------------------------------------------------------------------
std::string StringUtil::toString(unsigned long val, unsigned short width,
    char fill, std::ios::fmtflags flags) {
  StringStream stream;
  if (msUseLocale) stream.imbue(msLocale);
  stream.width(width);
  stream.fill(fill);
  if (flags) stream.setf(flags);
  stream << val;
  return stream.str();
}
//-----------------------------------------------------------------------
#endif
std::string StringUtil::toString(
    long val, unsigned short width, char fill, std::ios::fmtflags flags) {
  StringStream stream;
  if (msUseLocale) stream.imbue(msLocale);
  stream.width(width);
  stream.fill(fill);
  if (flags) stream.setf(flags);
  stream << val;
  return stream.str();
}
//-----------------------------------------------------------------------
std::string StringUtil::toString(bool val, bool yesNo) {
  if (val) {
    if (yesNo) {
      return "yes";
    } else {
      return "true";
    }
  } else if (yesNo) {
    return "no";
  } else {
    return "false";
  }
}
//-----------------------------------------------------------------------
std::string StringUtil::toString(const StringVector& val) {
  StringStream stream;
  if (msUseLocale) stream.imbue(msLocale);
  StringVector::const_iterator i, iend, ibegin;
  ibegin = val.begin();
  iend = val.end();
  for (i = ibegin; i != iend; ++i) {
    if (i != ibegin) stream << " ";

    stream << *i;
  }
  return stream.str();
}
//-----------------------------------------------------------------------
Real StringUtil::parseReal(const std::string& val, Real defaultValue) {
  // Use iStringStream for direct correspondence with toString
  StringStream str(val);
  if (msUseLocale) str.imbue(msLocale);
  Real ret = defaultValue;
  if (!(str >> ret)) return defaultValue;

  return ret;
}
//-----------------------------------------------------------------------
int StringUtil::parseInt(const std::string& val, int defaultValue) {
  // Use iStringStream for direct correspondence with toString
  StringStream str(val);
  if (msUseLocale) str.imbue(msLocale);
  int ret = defaultValue;
  if (!(str >> ret)) return defaultValue;

  return ret;
}
//-----------------------------------------------------------------------
unsigned int StringUtil::parseUnsignedInt(
    const std::string& val, unsigned int defaultValue) {
  // Use iStringStream for direct correspondence with toString
  StringStream str(val);
  if (msUseLocale) str.imbue(msLocale);
  unsigned int ret = defaultValue;
  if (!(str >> ret)) return defaultValue;

  return ret;
}
//-----------------------------------------------------------------------
long StringUtil::parseLong(const std::string& val, long defaultValue) {
  // Use iStringStream for direct correspondence with toString
  StringStream str(val);
  if (msUseLocale) str.imbue(msLocale);
  long ret = defaultValue;
  if (!(str >> ret)) return defaultValue;

  return ret;
}
//-----------------------------------------------------------------------
unsigned long StringUtil::parseUnsignedLong(
    const std::string& val, unsigned long defaultValue) {
  // Use iStringStream for direct correspondence with toString
  StringStream str(val);
  if (msUseLocale) str.imbue(msLocale);
  unsigned long ret = defaultValue;
  if (!(str >> ret)) return defaultValue;

  return ret;
}
//-----------------------------------------------------------------------
size_t StringUtil::parseSizeT(const std::string& val, size_t defaultValue) {
  // Use iStringStream for direct correspondence with toString
  StringStream str(val);
  if (msUseLocale) str.imbue(msLocale);
  size_t ret = defaultValue;
  if (!(str >> ret)) return defaultValue;

  return ret;
}
//-----------------------------------------------------------------------
bool StringUtil::parseBool(const std::string& val, bool defaultValue) {
  if ((StringUtil::startsWith(val, "true") ||
          StringUtil::startsWith(val, "yes") ||
          StringUtil::startsWith(val, "1")))
    return true;
  else if ((StringUtil::startsWith(val, "false") ||
               StringUtil::startsWith(val, "no") ||
               StringUtil::startsWith(val, "0")))
    return false;
  else
    return defaultValue;
}
//-----------------------------------------------------------------------
StringVector StringUtil::parseStringVector(const std::string& val) {
  return StringUtil::split(val);
}
//-----------------------------------------------------------------------
bool StringUtil::isNumber(const std::string& val) {
  StringStream str(val);
  if (msUseLocale) str.imbue(msLocale);
  float tst;
  str >> tst;
  return !str.fail() && str.eof();
}

//------------------------------------------------------------------
bool StringUtil::isAbsolutePath(const std::string& path) {
  return !path.empty() && StringUtil::startsWith(path, pac::delim);
}
}
