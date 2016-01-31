/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2014 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "pacStringUtil.h"

namespace pac {

	//-----------------------------------------------------------------------
	const String StringUtil::BLANK;
    String StringUtil::msDefaultStringLocale = "C";
    std::locale StringUtil::msLocale = std::locale(msDefaultStringLocale.c_str());
	bool StringUtil::msUseLocale = false;

	//-----------------------------------------------------------------------
    void StringUtil::trim(String& str, bool left, bool right)
    {
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
        static const String delims = " \t\r";
        if(right)
            str.erase(str.find_last_not_of(delims)+1); // trim right
        if(left)
            str.erase(0, str.find_first_not_of(delims)); // trim left
    }

    //-----------------------------------------------------------------------
    StringVector StringUtil::split( const String& str, const String& delims, unsigned int maxSplits, bool preserveDelims)
    {
        StringVector ret;
        // Pre-allocate some space for performance
        ret.reserve(maxSplits ? maxSplits+1 : 10);    // 10 is guessed capacity for most case

        unsigned int numSplits = 0;

        // Use STL methods 
        size_t start, pos;
        start = 0;
        do 
        {
            pos = str.find_first_of(delims, start);
            if (pos == start)
            {
                // Do nothing
                start = pos + 1;
            }
            else if (pos == String::npos || (maxSplits && numSplits == maxSplits))
            {
                // Copy the rest of the string
                ret.push_back( str.substr(start) );
                break;
            }
            else
            {
                // Copy up to delimiter
                ret.push_back( str.substr(start, pos - start) );

                if(preserveDelims)
                {
                    // Sometimes there could be more than one delimiter in a row.
                    // Loop until we don't find any more delims
                    size_t delimStart = pos, delimPos;
                    delimPos = str.find_first_not_of(delims, delimStart);
                    if (delimPos == String::npos)
                    {
                        // Copy the rest of the string
                        ret.push_back( str.substr(delimStart) );
                    }
                    else
                    {
                        ret.push_back( str.substr(delimStart, delimPos - delimStart) );
                    }
                }

                start = pos + 1;
            }
            // parse up to next real data
            start = str.find_first_not_of(delims, start);
            ++numSplits;

        } while (pos != String::npos);



        return ret;
    }
	//-----------------------------------------------------------------------
	StringVector StringUtil::tokenise( const String& str, const String& singleDelims, const String& doubleDelims, unsigned int maxSplits)
	{
        StringVector ret;
        // Pre-allocate some space for performance
        ret.reserve(maxSplits ? maxSplits+1 : 10);    // 10 is guessed capacity for most case

        unsigned int numSplits = 0;
		String delims = singleDelims + doubleDelims;

		// Use STL methods 
        size_t start, pos;
		char curDoubleDelim = 0;
        start = 0;
        do 
        {
			if (curDoubleDelim != 0)
			{
				pos = str.find(curDoubleDelim, start);
			}
			else
			{
				pos = str.find_first_of(delims, start);
			}

            if (pos == start)
            {
				char curDelim = str.at(pos);
				if (doubleDelims.find_first_of(curDelim) != String::npos)
				{
					curDoubleDelim = curDelim;
				}
                // Do nothing
                start = pos + 1;
            }
            else if (pos == String::npos || (maxSplits && numSplits == maxSplits))
            {
				if (curDoubleDelim != 0)
				{
					//Missing closer. Warn or throw exception?
				}
                // Copy the rest of the string
                ret.push_back( str.substr(start) );
                break;
            }
            else
            {
				if (curDoubleDelim != 0)
				{
					curDoubleDelim = 0;
				}

				// Copy up to delimiter
				ret.push_back( str.substr(start, pos - start) );
				start = pos + 1;
            }
			if (curDoubleDelim == 0)
			{
				// parse up to next real data
				start = str.find_first_not_of(singleDelims, start);
			}
            
            ++numSplits;

        } while (start != String::npos);

        return ret;
    }
    //-----------------------------------------------------------------------
    void StringUtil::toLowerCase(String& str)
    {
        std::transform(
            str.begin(),
            str.end(),
            str.begin(),
			tolower);
    }

    //-----------------------------------------------------------------------
    void StringUtil::toUpperCase(String& str) 
    {
        std::transform(
            str.begin(),
            str.end(),
            str.begin(),
			toupper);
    }
    //-----------------------------------------------------------------------
    bool StringUtil::startsWith(const String& str, const String& pattern, bool lowerCase)
    {
        size_t thisLen = str.length();
        size_t patternLen = pattern.length();
        if (thisLen < patternLen || patternLen == 0)
            return false;

        String startOfThis = str.substr(0, patternLen);
        if (lowerCase)
		{
			String lowerCasePattern = pattern;
            StringUtil::toLowerCase(lowerCasePattern);
            StringUtil::toLowerCase(startOfThis);
			return (startOfThis == lowerCasePattern);
		}

        return (startOfThis == pattern);
    }
    //-----------------------------------------------------------------------
    bool StringUtil::endsWith(const String& str, const String& pattern, bool lowerCase)
    {
        size_t thisLen = str.length();
        size_t patternLen = pattern.length();
        if (thisLen < patternLen || patternLen == 0)
            return false;

        String endOfThis = str.substr(thisLen - patternLen, patternLen);
        if (lowerCase)
		{
			String lowerCasePattern = pattern;
            StringUtil::toLowerCase(lowerCasePattern);
            StringUtil::toLowerCase(endOfThis);
			return (endOfThis == lowerCasePattern);
		}

        return (endOfThis == pattern);
    }
    //-----------------------------------------------------------------------
    String StringUtil::standardisePath(const String& init)
    {
        String path = init;

        std::replace( path.begin(), path.end(), '\\', '/' );
        if( path[path.length() - 1] != '/' )
            path += '/';

        return path;
    }
	//-----------------------------------------------------------------------
	String StringUtil::normalizeFilePath(const String& init, bool makeLowerCase)
	{
		const char* bufferSrc = init.c_str();
		int pathLen = (int)init.size();
		int indexSrc = 0;
		int indexDst = 0;
		int metaPathArea = 0;

		char reservedBuf[1024];
		char* bufferDst = reservedBuf;
		bool isDestAllocated = false;
		if (pathLen > 1023)
		{
			//if source path is to long ensure we don't do a buffer overrun by allocating some
			//new memory
			isDestAllocated = true;
			bufferDst = new char[pathLen + 1];
		}

		//The outer loop loops over directories
		while (indexSrc < pathLen)
		{		
			if ((bufferSrc[indexSrc] == '\\') || (bufferSrc[indexSrc] == '/'))
			{
				//check if we have a directory delimiter if so skip it (we should already
				//have written such a delimiter by this point
				++indexSrc;
				continue;
			}
			else
			{
				//check if there is a directory to skip of type ".\"
				if ((bufferSrc[indexSrc] == '.') && 
					((bufferSrc[indexSrc + 1] == '\\') || (bufferSrc[indexSrc + 1] == '/')))
				{
					indexSrc += 2;
					continue;			
				}

				//check if there is a directory to skip of type "..\"
				else if ((bufferSrc[indexSrc] == '.') && (bufferSrc[indexSrc + 1] == '.') &&
					((bufferSrc[indexSrc + 2] == '\\') || (bufferSrc[indexSrc + 2] == '/')))
				{
					if (indexDst > metaPathArea)
					{
						//skip a directory backward in the destination path
						do {
							--indexDst;
						}
						while ((indexDst > metaPathArea) && (bufferDst[indexDst - 1] != '/'));
						indexSrc += 3;
						continue;
					}
					else
					{
						//we are about to write "..\" to the destination buffer
						//ensure we will not remove this in future "skip directories"
						metaPathArea += 3;
					}
				}
			}

			//transfer the current directory name from the source to the destination
			while (indexSrc < pathLen)
			{
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

		String normalized(bufferDst); 
		if (isDestAllocated)
		{
			delete[] bufferDst;
		}

		return normalized;		
	}
    //-----------------------------------------------------------------------
    void StringUtil::splitFilename(const String& qualifiedName, 
        String& outBasename, String& outPath)
    {
        String path = qualifiedName;
        // Replace \ with / first
        std::replace( path.begin(), path.end(), '\\', '/' );
        // split based on final /
        size_t i = path.find_last_of('/');

        if (i == String::npos)
        {
            outPath.clear();
			outBasename = qualifiedName;
        }
        else
        {
            outBasename = path.substr(i+1, path.size() - i - 1);
            outPath = path.substr(0, i+1);
        }

    }

	//------------------------------------------------------------------
	void StringUtil::splitFullFilename(const String& qualifiedName, 
			String& outBasename, String& outExtention, 
			String& outPath)
	{
		//@TODO implement
		throw new std::runtime_error("unimplemented function called");
	}

	//------------------------------------------------------------------
	void StringUtil::splitBaseFilename(const String& fullName, 
			String& outBasename, String& outExtention)
	{
		//@TODO implement
		throw new std::runtime_error("unimplemented function called");
	}

	//------------------------------------------------------------------
	String StringUtil::join(const StringVector& sv, const String& sep /*= " "*/)
	{
		String s;
		std::for_each(sv.begin(), sv.end(), [&](const String& v)->void
		{
			s += s.empty() ? v : sep + v;
		});

		return s; 
	}

	//------------------------------------------------------------------
	String StringUtil::toFixLength(const String& s, size_t length,  char c /*= ' '*/)
	{
		//do nothing if it's long enough
		if(s.size() >= length)
			return s;

		String s1(length - s.size(), c);

		return s + s1;
	}
	//-----------------------------------------------------------------------
	void StringUtil::splitBaseFilename(const Ogre::String& fullName, 
		Ogre::String& outBasename, Ogre::String& outExtention)
	{
		size_t i = fullName.find_last_of(".");
		if (i == Ogre::String::npos)
		{
			outExtention.clear();
			outBasename = fullName;
		}
		else
		{
			outExtention = fullName.substr(i+1);
			outBasename = fullName.substr(0, i);
		}
	}
	// ----------------------------------------------------------------------------------------------------------------------------------------------
	void StringUtil::splitFullFilename(	const Ogre::String& qualifiedName, 
		Ogre::String& outBasename, Ogre::String& outExtention, Ogre::String& outPath )
	{
		Ogre::String fullName;
		splitFilename( qualifiedName, fullName, outPath );
		splitBaseFilename( fullName, outBasename, outExtention );
	}
    //-----------------------------------------------------------------------
    bool StringUtil::match(const String& str, const String& pattern, bool caseSensitive)
    {
        String tmpStr = str;
		String tmpPattern = pattern;
        if (!caseSensitive)
        {
            StringUtil::toLowerCase(tmpStr);
            StringUtil::toLowerCase(tmpPattern);
        }

        String::const_iterator strIt = tmpStr.begin();
        String::const_iterator patIt = tmpPattern.begin();
		String::const_iterator lastWildCardIt = tmpPattern.end();
        while (strIt != tmpStr.end() && patIt != tmpPattern.end())
        {
            if (*patIt == '*')
            {
				lastWildCardIt = patIt;
                // Skip over looking for next character
                ++patIt;
                if (patIt == tmpPattern.end())
				{
					// Skip right to the end since * matches the entire rest of the string
					strIt = tmpStr.end();
				}
				else
                {
					// scan until we find next pattern character
                    while(strIt != tmpStr.end() && *strIt != *patIt)
                        ++strIt;
                }
            }
            else
            {
                if (*patIt != *strIt)
                {
					if (lastWildCardIt != tmpPattern.end())
					{
						// The last wildcard can match this incorrect sequence
						// rewind pattern to wildcard and keep searching
						patIt = lastWildCardIt;
						lastWildCardIt = tmpPattern.end();
					}
					else
					{
						// no wildwards left
						return false;
					}
                }
                else
                {
                    ++patIt;
                    ++strIt;
                }
            }

        }
		// If we reached the end of both the pattern and the string, we succeeded
		if (patIt == tmpPattern.end() && strIt == tmpStr.end())
		{
        	return true;
		}
		else
		{
			return false;
		}

    }
	//-----------------------------------------------------------------------
	const String StringUtil::replaceAll(const String& source, const String& replaceWhat, const String& replaceWithWhat)
	{
		String result = source;
        String::size_type pos = 0;
		while(1)
		{
			pos = result.find(replaceWhat,pos);
			if (pos == String::npos) break;
			result.replace(pos,replaceWhat.size(),replaceWithWhat);
            pos += replaceWithWhat.size();
		}
		return result;
	}

	//------------------------------------------------------------------
	String StringUtil::getHead(const String& path)
	{
		if(path == path::delim)
			return path::delim;

		int pos = path.find_last_of(pac::delim);
		if(pos == -1)
		{
			return "."
		}
		else
		{
			return path.substr(0, pos);
		}
	}

	//------------------------------------------------------------------
	String StringUtil::getTail(const String& path)
	{
		int pos = path.find_last_of(pac::delim);

		if(pos == -1)
		{
			return path;
		}
		else
		{
			return path.substr(pos + 1);
		}
	}

    //-----------------------------------------------------------------------
    String StringUtil::toString(Real val, unsigned short precision, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        StringStream stream;
        if (msUseLocale)
            stream.imbue(msLocale);
        stream.precision(precision);
        stream.width(width);
        stream.fill(fill);
        if (flags)
            stream.setf(flags);
        stream << val;

        return stream.str();
    }
#if OGRE_DOUBLE_PRECISION == 1
    //-----------------------------------------------------------------------
    String StringUtil::toString(float val, unsigned short precision,
                                     unsigned short width, char fill, std::ios::fmtflags flags)
    {
        StringStream stream;
        if (msUseLocale)
            stream.imbue(msLocale);
        stream.precision(precision);
        stream.width(width);
        stream.fill(fill);
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
#else
    //-----------------------------------------------------------------------
    String StringUtil::toString(double val, unsigned short precision,
                                     unsigned short width, char fill, std::ios::fmtflags flags)
    {
        StringStream stream;
        if (msUseLocale)
            stream.imbue(msLocale);
        stream.precision(precision);
        stream.width(width);
        stream.fill(fill);
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
#endif
    //-----------------------------------------------------------------------
    String StringUtil::toString(int val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        StringStream stream;
        if (msUseLocale)
            stream.imbue(msLocale);
        stream.width(width);
        stream.fill(fill);
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
    //-----------------------------------------------------------------------
#if OGRE_PLATFORM != OGRE_PLATFORM_NACL &&  ( OGRE_ARCH_TYPE == OGRE_ARCHITECTURE_64 || OGRE_PLATFORM == OGRE_PLATFORM_APPLE || OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS )
    String StringUtil::toString(unsigned int val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        StringStream stream;
        if (msUseLocale)
            stream.imbue(msLocale);
        stream.width(width);
        stream.fill(fill);
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
    //-----------------------------------------------------------------------
    String StringUtil::toString(size_t val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        StringStream stream;
        if (msUseLocale)
            stream.imbue(msLocale);
        stream.width(width);
        stream.fill(fill);
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
#if OGRE_COMPILER == OGRE_COMPILER_MSVC
    //-----------------------------------------------------------------------
    String StringUtil::toString(unsigned long val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        StringStream stream;
        if (msUseLocale)
            stream.imbue(msLocale);
        stream.width(width);
        stream.fill(fill);
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }

#endif
    //-----------------------------------------------------------------------
#else
    String StringUtil::toString(size_t val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        StringStream stream;
        if (msUseLocale)
            stream.imbue(msLocale);
        stream.width(width);
        stream.fill(fill);
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
    //-----------------------------------------------------------------------
    String StringUtil::toString(unsigned long val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        StringStream stream;
        if (msUseLocale)
            stream.imbue(msLocale);
        stream.width(width);
        stream.fill(fill);
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
    //-----------------------------------------------------------------------
#endif
    String StringUtil::toString(long val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        StringStream stream;
        if (msUseLocale)
            stream.imbue(msLocale);
        stream.width(width);
        stream.fill(fill);
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
    //-----------------------------------------------------------------------
    String StringUtil::toString(const Vector2& val)
    {
        StringStream stream;
        if (msUseLocale)
            stream.imbue(msLocale);
        stream << val.x << " " << val.y;
        return stream.str();
    }
    //-----------------------------------------------------------------------
    String StringUtil::toString(const Vector3& val)
    {
        StringStream stream;
        if (msUseLocale)
            stream.imbue(msLocale);
        stream << val.x << " " << val.y << " " << val.z;
        return stream.str();
    }
    //-----------------------------------------------------------------------
    String StringUtil::toString(const Vector4& val)
    {
        StringStream stream;
        if (msUseLocale)
            stream.imbue(msLocale);
        stream << val.x << " " << val.y << " " << val.z << " " << val.w;
        return stream.str();
    }
    //-----------------------------------------------------------------------
    String StringUtil::toString(const Matrix3& val)
    {
        StringStream stream;
        stream.imbue(msLocale);
        stream << val[0][0] << " "
            << val[0][1] << " "             
            << val[0][2] << " "             
            << val[1][0] << " "             
            << val[1][1] << " "             
            << val[1][2] << " "             
            << val[2][0] << " "             
            << val[2][1] << " "             
            << val[2][2];
        return stream.str();
    }
    //-----------------------------------------------------------------------
    String StringUtil::toString(bool val, bool yesNo)
    {
        if (val)
        {
            if (yesNo)
            {
                return "yes";
            }
            else
            {
                return "true";
            }
        }
        else
            if (yesNo)
            {
                return "no";
            }
            else
            {
                return "false";
            }
    }
    //-----------------------------------------------------------------------
    String StringUtil::toString(const Matrix4& val)
    {
        StringStream stream;
        stream.imbue(msLocale);
        stream << val[0][0] << " "
            << val[0][1] << " "             
            << val[0][2] << " "             
            << val[0][3] << " "             
            << val[1][0] << " "             
            << val[1][1] << " "             
            << val[1][2] << " "             
            << val[1][3] << " "             
            << val[2][0] << " "             
            << val[2][1] << " "             
            << val[2][2] << " "             
            << val[2][3] << " "             
            << val[3][0] << " "             
            << val[3][1] << " "             
            << val[3][2] << " "             
            << val[3][3];
        return stream.str();
    }
    //-----------------------------------------------------------------------
    String StringUtil::toString(const Quaternion& val)
    {
        StringStream stream;
        if (msUseLocale)
            stream.imbue(msLocale);
        stream  << val.w << " " << val.x << " " << val.y << " " << val.z;
        return stream.str();
    }
    //-----------------------------------------------------------------------
    String StringUtil::toString(const ColourValue& val)
    {
        StringStream stream;
        if (msUseLocale)
            stream.imbue(msLocale);
        stream << val.r << " " << val.g << " " << val.b << " " << val.a;
        return stream.str();
    }
    //-----------------------------------------------------------------------
    String StringUtil::toString(const StringVector& val)
    {
        StringStream stream;
        if (msUseLocale)
            stream.imbue(msLocale);
        StringVector::const_iterator i, iend, ibegin;
        ibegin = val.begin();
        iend = val.end();
        for (i = ibegin; i != iend; ++i)
        {
            if (i != ibegin)
                stream << " ";

            stream << *i; 
        }
        return stream.str();
    }
    //-----------------------------------------------------------------------
    Real StringUtil::parseReal(const String& val, Real defaultValue)
    {
        // Use iStringStream for direct correspondence with toString
        StringStream str(val);
        if (msUseLocale)
            str.imbue(msLocale);
        Real ret = defaultValue;
        if( !(str >> ret) )
            return defaultValue;

        return ret;
    }
    //-----------------------------------------------------------------------
    int StringUtil::parseInt(const String& val, int defaultValue)
    {
        // Use iStringStream for direct correspondence with toString
        StringStream str(val);
        if (msUseLocale)
            str.imbue(msLocale);
        int ret = defaultValue;
        if( !(str >> ret) )
            return defaultValue;

        return ret;
    }
    //-----------------------------------------------------------------------
    unsigned int StringUtil::parseUnsignedInt(const String& val, unsigned int defaultValue)
    {
        // Use iStringStream for direct correspondence with toString
        StringStream str(val);
        if (msUseLocale)
            str.imbue(msLocale);
        unsigned int ret = defaultValue;
        if( !(str >> ret) )
            return defaultValue;

        return ret;
    }
    //-----------------------------------------------------------------------
    long StringUtil::parseLong(const String& val, long defaultValue)
    {
        // Use iStringStream for direct correspondence with toString
        StringStream str(val);
        if (msUseLocale)
            str.imbue(msLocale);
        long ret = defaultValue;
        if( !(str >> ret) )
            return defaultValue;

        return ret;
    }
    //-----------------------------------------------------------------------
    unsigned long StringUtil::parseUnsignedLong(const String& val, unsigned long defaultValue)
    {
        // Use iStringStream for direct correspondence with toString
        StringStream str(val);
        if (msUseLocale)
            str.imbue(msLocale);
        unsigned long ret = defaultValue;
        if( !(str >> ret) )
            return defaultValue;

        return ret;
    }
    //-----------------------------------------------------------------------
    size_t StringUtil::parseSizeT(const String& val, size_t defaultValue)
    {
        // Use iStringStream for direct correspondence with toString
        StringStream str(val);
        if (msUseLocale)
            str.imbue(msLocale);
        size_t ret = defaultValue;
        if( !(str >> ret) )
            return defaultValue;

        return ret;
    }
    //-----------------------------------------------------------------------
    bool StringUtil::parseBool(const String& val, bool defaultValue)
    {
        if ((StringUtil::startsWith(val, "true") || StringUtil::startsWith(val, "yes")
            || StringUtil::startsWith(val, "1")))
            return true;
        else if ((StringUtil::startsWith(val, "false") || StringUtil::startsWith(val, "no")
            || StringUtil::startsWith(val, "0")))
            return false;
        else
            return defaultValue;
    }
    //-----------------------------------------------------------------------
    Vector2 StringUtil::parseVector2(const String& val, const Vector2& defaultValue)
    {
        // Split on space
        vector<String>::type vec = StringUtil::split(val);

        if (vec.size() != 2)
        {
            return defaultValue;
        }
        else
        {
            return Vector2(parseReal(vec[0], defaultValue[0]), parseReal(vec[1], defaultValue[1]));
        }
    }
    //-----------------------------------------------------------------------
    Vector3 StringUtil::parseVector3(const String& val, const Vector3& defaultValue)
    {
        // Split on space
        vector<String>::type vec = StringUtil::split(val);

        if (vec.size() != 3)
        {
            return defaultValue;
        }
        else
        {
            return Vector3(parseReal(vec[0], defaultValue[0]),
                           parseReal(vec[1], defaultValue[1]),
                           parseReal(vec[2], defaultValue[2]));
        }
    }
    //-----------------------------------------------------------------------
    Vector4 StringUtil::parseVector4(const String& val, const Vector4& defaultValue)
    {
        // Split on space
        vector<String>::type vec = StringUtil::split(val);

        if (vec.size() != 4)
        {
            return defaultValue;
        }
        else
        {
            return Vector4(parseReal(vec[0], defaultValue[0]),
                           parseReal(vec[1], defaultValue[1]),
                           parseReal(vec[2], defaultValue[2]),
                           parseReal(vec[3], defaultValue[3]));
        }
    }
    //-----------------------------------------------------------------------
    Matrix3 StringUtil::parseMatrix3(const String& val, const Matrix3& defaultValue)
    {
        // Split on space
        vector<String>::type vec = StringUtil::split(val);

        if (vec.size() != 9)
        {
            return defaultValue;
        }
        else
        {
            return Matrix3(parseReal(vec[0], defaultValue[0][0]),
                           parseReal(vec[1], defaultValue[0][1]),
                           parseReal(vec[2], defaultValue[0][2]),

                           parseReal(vec[3], defaultValue[1][0]),
                           parseReal(vec[4], defaultValue[1][1]),
                           parseReal(vec[5], defaultValue[1][2]),

                           parseReal(vec[6], defaultValue[2][0]),
                           parseReal(vec[7], defaultValue[2][1]),
                           parseReal(vec[8], defaultValue[2][2]));
        }
    }
    //-----------------------------------------------------------------------
    Matrix4 StringUtil::parseMatrix4(const String& val, const Matrix4& defaultValue)
    {
        // Split on space
        vector<String>::type vec = StringUtil::split(val);

        if (vec.size() != 16)
        {
            return defaultValue;
        }
        else
        {
            return Matrix4(parseReal(vec[0], defaultValue[0][0]),
                           parseReal(vec[1], defaultValue[0][1]),
                           parseReal(vec[2], defaultValue[0][2]),
                           parseReal(vec[3], defaultValue[0][3]),
                           
                           parseReal(vec[4], defaultValue[1][0]),
                           parseReal(vec[5], defaultValue[1][1]),
                           parseReal(vec[6], defaultValue[1][2]),
                           parseReal(vec[7], defaultValue[1][3]),
                           
                           parseReal(vec[8], defaultValue[2][0]),
                           parseReal(vec[9], defaultValue[2][1]),
                           parseReal(vec[10], defaultValue[2][2]),
                           parseReal(vec[11], defaultValue[2][3]),
                           
                           parseReal(vec[12], defaultValue[3][0]),
                           parseReal(vec[13], defaultValue[3][1]),
                           parseReal(vec[14], defaultValue[3][2]),
                           parseReal(vec[15], defaultValue[3][3]));
        }
    }
    //-----------------------------------------------------------------------
    Quaternion StringUtil::parseQuaternion(const String& val, const Quaternion& defaultValue)
    {
        // Split on space
        vector<String>::type vec = StringUtil::split(val);

        if (vec.size() != 4)
        {
            return defaultValue;
        }
        else
        {
            return Quaternion(parseReal(vec[0], defaultValue[0]),
                              parseReal(vec[1], defaultValue[1]),
                              parseReal(vec[2], defaultValue[2]),
                              parseReal(vec[3], defaultValue[3]));
        }
    }
    //-----------------------------------------------------------------------
    ColourValue StringUtil::parseColourValue(const String& val, const ColourValue& defaultValue)
    {
        // Split on space
        vector<String>::type vec = StringUtil::split(val);

        if (vec.size() == 4)
        {
            return ColourValue(parseReal(vec[0], defaultValue[0]),
                               parseReal(vec[1], defaultValue[1]),
                               parseReal(vec[2], defaultValue[2]),
                               parseReal(vec[3], defaultValue[3]));
        }
        else if (vec.size() == 3)
        {
            return ColourValue(parseReal(vec[0], defaultValue[0]),
                               parseReal(vec[1], defaultValue[1]),
                               parseReal(vec[2], defaultValue[2]),
                               1.0f);
        }
        else
        {
            return defaultValue;
        }
    }
    //-----------------------------------------------------------------------
    StringVector StringUtil::parseStringVector(const String& val)
    {
        return StringUtil::split(val);
    }
    //-----------------------------------------------------------------------
    bool StringUtil::isNumber(const String& val)
    {
        StringStream str(val);
        if (msUseLocale)
            str.imbue(msLocale);
        float tst;
        str >> tst;
        return !str.fail() && str.eof();
    }

	//------------------------------------------------------------------
	bool StringUtil::isAbsolutePath(const String& path)
	{
		return !path.empty() && path.startsWith(pac::delim);
	}

}
