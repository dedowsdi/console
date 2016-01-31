#ifndef PACSTRINGUTIL_H
#define PACSTRINGUTIL_H

#include "pacConsolePreRequisite.h"
#include "pacException.h"

namespace pac {
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup General
	*  @{
	*/

    /** Utility class for manipulating Strings.  */
    class _PacExport StringUtil
    {
	public:
		typedef StringStream StrStreamType;

        /** Removes any whitespace characters, be it standard space or
            TABs and so on.
            @remarks
                The user may specify whether they want to trim only the
                beginning or the end of the String ( the default action is
                to trim both).
        */
        static void trim( String& str, bool left = true, bool right = true );

        /** Returns a StringVector that contains all the substrings delimited
            by the characters in the passed <code>delims</code> argument.
            @param
                delims A list of delimiter characters to split by
            @param
                maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
                parameters is > 0, the splitting process will stop after this many splits, left to right.
            @param
                preserveDelims Flag to determine if delimiters should be saved as substrings
        */
		static std::vector<String> split( const String& str, const String& delims = "\t\n ", unsigned int maxSplits = 0, bool preserveDelims = false);

		/** Returns a StringVector that contains all the substrings delimited
            by the characters in the passed <code>delims</code> argument, 
			or in the <code>doubleDelims</code> argument, which is used to include (normal) 
			delimeters in the tokenised string. For example, "strings like this".
            @param
                delims A list of delimiter characters to split by
			@param
                doubleDelims A list of double delimeters characters to tokenise by
            @param
                maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
                parameters is > 0, the splitting process will stop after this many splits, left to right.
        */
		static std::vector<String> tokenise( const String& str, const String& delims = "\t\n ", const String& doubleDelims = "\"", unsigned int maxSplits = 0);

		/** Lower-cases all the characters in the string.
        */
        static void toLowerCase( String& str );

        /** Upper-cases all the characters in the string.
        */
        static void toUpperCase( String& str );


        /** Returns whether the string begins with the pattern passed in.
        @param pattern The pattern to compare with.
        @param lowerCase If true, the start of the string will be lower cased before
            comparison, pattern should also be in lower case.
        */
        static bool startsWith(const String& str, const String& pattern, bool lowerCase = true);

        /** Returns whether the string ends with the pattern passed in.
        @param pattern The pattern to compare with.
        @param lowerCase If true, the end of the string will be lower cased before
            comparison, pattern should also be in lower case.
        */
        static bool endsWith(const String& str, const String& pattern, bool lowerCase = true);

        /** Method for standardising paths - use forward slashes only, end with slash.
        */
        static String standardisePath( const String &init);
		/** Returns a normalized version of a file path
		This method can be used to make file path strings which point to the same directory  
		but have different texts to be normalized to the same text. The function:
		- Transforms all backward slashes to forward slashes.
		- Removes repeating slashes.
		- Removes initial slashes from the beginning of the path.
		- Removes ".\" and "..\" meta directories.
		- Sets all characters to lowercase (if requested)
		@param init The file path to normalize.
		@param makeLowerCase If true, transforms all characters in the string to lowercase.
		*/
       static String normalizeFilePath(const String& init, bool makeLowerCase = true);


        /** Method for splitting a fully qualified filename into the base name
            and path.
        @remarks
            Path is standardised as in standardisePath
        */
        static void splitFilename(const String& qualifiedName,
            String& outBasename, String& outPath);

		/** Method for splitting a fully qualified filename into the base name,
		extension and path.
		@remarks
		Path is standardised as in standardisePath
		*/
		static void splitFullFilename(const String& qualifiedName, 
			String& outBasename, String& outExtention, 
			String& outPath);

		/** Method for splitting a filename into the base name
		and extension.
		*/
		static void splitBaseFilename(const String& fullName, 
			String& outBasename, String& outExtention);

		/**
		 * Join string vection into a string by sep 
		 * @param sv : string vector
		 * @param sep : separater 
		 * @return : joined string 
		 */
		static String join(const StringVector& sv, const String& sep = " ");

		/**
		 * Extend string to fixed length 
		 * @param s : strign 
		 * @param length : fixed length
		 * @param c : char used to extend 
		 * @return : new string in fixed length 
		 */
		static String toFixLength(const String& s, size_t length,  char c = ' ');
		

        /** Simple pattern-matching routine allowing a wildcard pattern.
        @param str String to test
        @param pattern Pattern to match against; can include simple '*' wildcards
        @param caseSensitive Whether the match is case sensitive or not
        */
        static bool match(const String& str, const String& pattern, bool caseSensitive = true);


		/** replace all instances of a sub-string with a another sub-string.
		@param source Source string
		@param replaceWhat Sub-string to find and replace
		@param replaceWithWhat Sub-string to replace with (the new sub-string)
		@return An updated string with the sub-string replaced
		*/
		static const String replaceAll(const String& source, const String& replaceWhat, const String& replaceWithWhat);

        /** Converts a Real to a String. */
        static String toString(Real val, unsigned short precision = 6, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0));
#if OGRE_DOUBLE_PRECISION == 1
        /** Converts a float to a String. */
        static String toString(float val, unsigned short precision = 6,
                               unsigned short width = 0, char fill = ' ',
                               std::ios::fmtflags flags = std::ios::fmtflags(0));
#else
        /** Converts a double to a String. */
        static String toString(double val, unsigned short precision = 6,
                               unsigned short width = 0, char fill = ' ',
                               std::ios::fmtflags flags = std::ios::fmtflags(0));
#endif
        /** Converts a Radian to a String. */
        static String toString(Radian val, unsigned short precision = 6, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0))
        {
            return toString(val.valueAngleUnits(), precision, width, fill, flags);
        }
        /** Converts a Degree to a String. */
        static String toString(Degree val, unsigned short precision = 6, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0))
        {
            return toString(val.valueAngleUnits(), precision, width, fill, flags);
        }
        /** Converts an int to a String. */
        static String toString(int val, unsigned short width = 0, 
            char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0));
#if OGRE_PLATFORM != OGRE_PLATFORM_NACL &&  ( OGRE_ARCH_TYPE == OGRE_ARCHITECTURE_64 || OGRE_PLATFORM == OGRE_PLATFORM_APPLE || OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS )
        /** Converts an unsigned int to a String. */
        static String toString(unsigned int val, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0));
        /** Converts a size_t to a String. */
        static String toString(size_t val, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0));
        #if OGRE_COMPILER == OGRE_COMPILER_MSVC
        /** Converts an unsigned long to a String. */
        static String toString(unsigned long val, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0));
        #endif
#else
        /** Converts a size_t to a String. */
        static String toString(size_t val, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0));
        /** Converts an unsigned long to a String. */
        static String toString(unsigned long val, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0));
#endif
        /** Converts a long to a String. */
        static String toString(long val, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0));
        /** Converts a boolean to a String. 
        @param yesNo If set to true, result is 'yes' or 'no' instead of 'true' or 'false'
        */
        static String toString(bool val, bool yesNo = false);
        /** Converts a Vector2 to a String. 
        @remarks
            Format is "x y" (i.e. 2x Real values, space delimited)
        */
        static String toString(const Vector2& val);
        /** Converts a Vector3 to a String. 
        @remarks
            Format is "x y z" (i.e. 3x Real values, space delimited)
        */
        static String toString(const Vector3& val);
        /** Converts a Vector4 to a String. 
        @remarks
            Format is "x y z w" (i.e. 4x Real values, space delimited)
        */
        static String toString(const Vector4& val);
        /** Converts a Matrix3 to a String. 
        @remarks
            Format is "00 01 02 10 11 12 20 21 22" where '01' means row 0 column 1 etc.
        */
        static String toString(const Matrix3& val);
        /** Converts a Matrix4 to a String. 
        @remarks
            Format is "00 01 02 03 10 11 12 13 20 21 22 23 30 31 32 33" where 
            '01' means row 0 column 1 etc.
        */
        static String toString(const Matrix4& val);
        /** Converts a Quaternion to a String. 
        @remarks
            Format is "w x y z" (i.e. 4x Real values, space delimited)
        */
        static String toString(const Quaternion& val);
        /** Converts a ColourValue to a String. 
        @remarks
            Format is "r g b a" (i.e. 4x Real values, space delimited). 
        */
        static String toString(const ColourValue& val);
        /** Converts a StringVector to a string.
        @remarks
            Strings must not contain spaces since space is used as a delimiter in
            the output.
        */
        static String toString(const StringVector& val);

        /** Converts a String to a Real. 
        @return
            0.0 if the value could not be parsed, otherwise the Real version of the String.
        */
        static Real parseReal(const String& val, Real defaultValue = 0);
        /** Converts a String to a Angle. 
        @return
            0.0 if the value could not be parsed, otherwise the Angle version of the String.
        */
        static inline Radian parseAngle(const String& val, Radian defaultValue = Radian(0)) {
            return Angle(parseReal(val, defaultValue.valueRadians()));
        }
        /** Converts a String to a whole number. 
        @return
            0.0 if the value could not be parsed, otherwise the numeric version of the String.
        */
        static int parseInt(const String& val, int defaultValue = 0);
        /** Converts a String to a whole number. 
        @return
            0.0 if the value could not be parsed, otherwise the numeric version of the String.
        */
        static unsigned int parseUnsignedInt(const String& val, unsigned int defaultValue = 0);
        /** Converts a String to a whole number. 
        @return
            0.0 if the value could not be parsed, otherwise the numeric version of the String.
        */
        static long parseLong(const String& val, long defaultValue = 0);
        /** Converts a String to a whole number. 
        @return
            0.0 if the value could not be parsed, otherwise the numeric version of the String.
        */
        static unsigned long parseUnsignedLong(const String& val, unsigned long defaultValue = 0);
        /** Converts a String to size_t. 
        @return
            defaultValue if the value could not be parsed, otherwise the numeric version of the String.
        */
        static size_t parseSizeT(const String& val, size_t defaultValue = 0);
        /** Converts a String to a boolean. 
        @remarks
            Returns true if case-insensitive match of the start of the string
            matches "true", "yes" or "1", false otherwise.
        */
        static bool parseBool(const String& val, bool defaultValue = 0);
        /** Parses a Vector2 out of a String.
        @remarks
            Format is "x y" ie. 2 Real components, space delimited. Failure to parse returns
            Vector2::ZERO.
        */
        static Vector2 parseVector2(const String& val, const Vector2& defaultValue = Vector2::ZERO);
        /** Parses a Vector3 out of a String.
        @remarks
            Format is "x y z" ie. 3 Real components, space delimited. Failure to parse returns
            Vector3::ZERO.
        */
        static Vector3 parseVector3(const String& val, const Vector3& defaultValue = Vector3::ZERO);
        /** Parses a Vector4 out of a String.
        @remarks
            Format is "x y z w" ie. 4 Real components, space delimited. Failure to parse returns
            Vector4::ZERO.
        */
        static Vector4 parseVector4(const String& val, const Vector4& defaultValue = Vector4::ZERO);
        /** Parses a Matrix3 out of a String.
        @remarks
            Format is "00 01 02 10 11 12 20 21 22" where '01' means row 0 column 1 etc.
            Failure to parse returns Matrix3::IDENTITY.
        */
        static Matrix3 parseMatrix3(const String& val, const Matrix3& defaultValue = Matrix3::IDENTITY);
        /** Parses a Matrix4 out of a String.
        @remarks
            Format is "00 01 02 03 10 11 12 13 20 21 22 23 30 31 32 33" where 
            '01' means row 0 column 1 etc. Failure to parse returns Matrix4::IDENTITY.
        */
        static Matrix4 parseMatrix4(const String& val, const Matrix4& defaultValue = Matrix4::IDENTITY);
        /** Parses a Quaternion out of a String. 
        @remarks
            Format is "w x y z" (i.e. 4x Real values, space delimited). 
            Failure to parse returns Quaternion::IDENTITY.
        */
        static Quaternion parseQuaternion(const String& val, const Quaternion& defaultValue = Quaternion::IDENTITY);
        /** Parses a ColourValue out of a String. 
        @remarks
            Format is "r g b a" (i.e. 4x Real values, space delimited), or "r g b" which implies
            an alpha value of 1.0 (opaque). Failure to parse returns ColourValue::Black.
        */
        static ColourValue parseColourValue(const String& val, const ColourValue& defaultValue = ColourValue::Black);

        /** Parses a StringVector from a string.
        @remarks
            Strings must not contain spaces since space is used as a delimiter in
            the output.
        */
        static StringVector parseStringVector(const String& val);
        /** Checks the String is a valid number value. */
        static bool isNumber(const String& val);

		/**
		 * check if path starts with delim 
		 * @param path : path 
		 * @return : true if it's absolute path 
		 */
		static bool isAbsolutePath(const String& path);

		/**
		 * Checks the String is a valid decimal type . This should work from
		 * unsigned short, short ..... until long long.
		 * @param val : string value 
		 */
		static template<class T> 
		bool isPrimitiveDecimal(const String& val)
		{
			StringStream str(val);
			if (msUseLocale)
				str.imbue(msLocale);
			T tst;
			str >> tst;
			return !str.fail() && str.eof();
		}

		/**
		 * parse string to decimal type . This should work from unsigned short,
		 * short ..... until long long.
		 * @param val : string value 
		 */
		static template<class T> 
		T parsePrimitiveDecimal(const String& val)
		{
			StringStream str(val);
			if (msUseLocale)
				str.imbue(msLocale);
			T t;
			str >> t;
			if(!str.fail() && str.eof())
				PAC_EXCEPT(Exception::ERR_INVALIDPARAMS,
						val +  " is not paseable", __FUNCTION__);
			return t;
		}

        //-----------------------------------------------------------------------
        static void setDefaultStringLocale(String loc)
        {
            msDefaultStringLocale = loc;
            msLocale = std::locale(msDefaultStringLocale.c_str());
        }
        //-----------------------------------------------------------------------
        static String getDefaultStringLocale(void) { return msDefaultStringLocale; }
        //-----------------------------------------------------------------------
        static void setUseLocale(bool useLocale) { msUseLocale = useLocale; }
        //-----------------------------------------------------------------------
        static bool isUseLocale() { return msUseLocale; }
        //-----------------------------------------------------------------------
		
		/**
		 * Remove last path component. If path ends with pac::delim, only the
		 * delim will be removed. If path has no pac::delim, . will be be
		 * returned. if path equals to pac::delim, pac::delim will be returned.
		 * @param path : path
		 */
		static String getHead(const String& path);

		/**
		 * Get last part of path component, If path ends with pac::delim, you
		 * get blank. If path has no pac:delim you get original path
		 * @param path : 
		 */
		static String getTail(const String& path);

    protected:
        static String msDefaultStringLocale;
        static std::locale msLocale;
        static bool msUseLocale;
        /// Constant blank string, useful for returning by ref where local does not exist
        static const String BLANK;
    };

	/** @} */
	/** @} */

} // namespace Ogre

#include "OgreHeaderSuffix.h"

#endif // PACSTRINGUTIL_H
