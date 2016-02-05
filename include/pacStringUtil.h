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
                beginning or the end of the std::string ( the default action is
                to trim both).
        */
        static void trim( std::string& str, bool left = true, bool right = true );

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
		static std::vector<std::string> split( const std::string& str, const std::string& delims = "\t\n ", unsigned int maxSplits = 0, bool preserveDelims = false);

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
		static std::vector<std::string> tokenise( const std::string& str, const std::string& delims = "\t\n ", const std::string& doubleDelims = "\"", unsigned int maxSplits = 0);

		/** Lower-cases all the characters in the string.
        */
        static void toLowerCase( std::string& str );

        /** Upper-cases all the characters in the string.
        */
        static void toUpperCase( std::string& str );


        /** Returns whether the string begins with the pattern passed in.
        @param pattern The pattern to compare with.
        @param lowerCase If true, the start of the string will be lower cased before
            comparison, pattern should also be in lower case.
        */
        static bool startsWith(const std::string& str, const std::string& pattern, bool lowerCase = true);

        /** Returns whether the string ends with the pattern passed in.
        @param pattern The pattern to compare with.
        @param lowerCase If true, the end of the string will be lower cased before
            comparison, pattern should also be in lower case.
        */
        static bool endsWith(const std::string& str, const std::string& pattern, bool lowerCase = true);

        /** Method for standardising paths - use forward slashes only, end with slash.
        */
        static std::string standardisePath( const std::string &init);
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
       static std::string normalizeFilePath(const std::string& init, bool makeLowerCase = true);


        /** Method for splitting a fully qualified filename into the base name
            and path.
        @remarks
            Path is standardised as in standardisePath
        */
        static void splitFilename(const std::string& qualifiedName,
            std::string& outBasename, std::string& outPath);

		/** Method for splitting a fully qualified filename into the base name,
		extension and path.
		@remarks
		Path is standardised as in standardisePath
		*/
		static void splitFullFilename(const std::string& qualifiedName, 
			std::string& outBasename, std::string& outExtention, 
			std::string& outPath);

		/** Method for splitting a filename into the base name
		and extension.
		*/
		static void splitBaseFilename(const std::string& fullName, 
			std::string& outBasename, std::string& outExtention);

		/**
		 * Join string vection into a string by sep 
		 * @param sv : string vector
		 * @param sep : separater 
		 * @return : joined string 
		 */
		static std::string join(const StringVector& sv, const std::string& sep = " ");

		/**
		 * Extend string to fixed length 
		 * @param s : strign 
		 * @param length : fixed length
		 * @param c : char used to extend 
		 * @return : new string in fixed length 
		 */
		static std::string toFixedLength(const std::string& s, size_t length,  char c = ' ');
		

        /** Simple pattern-matching routine allowing a wildcard pattern.
        @param str std::string to test
        @param pattern Pattern to match against; can include simple '*' wildcards
        @param caseSensitive Whether the match is case sensitive or not
        */
        static bool match(const std::string& str, const std::string& pattern, bool caseSensitive = true);


		/** replace all instances of a sub-string with a another sub-string.
		@param source Source string
		@param replaceWhat Sub-string to find and replace
		@param replaceWithWhat Sub-string to replace with (the new sub-string)
		@return An updated string with the sub-string replaced
		*/
		static const std::string replaceAll(const std::string& source, const std::string& replaceWhat, const std::string& replaceWithWhat);

        /** Converts a Real to a std::string. */
        static std::string toString(Real val, unsigned short precision = 6, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0));
#if PAC_DOUBLE_PRECISION == 1
        /** Converts a float to a std::string. */
        static std::string toString(float val, unsigned short precision = 6,
                               unsigned short width = 0, char fill = ' ',
                               std::ios::fmtflags flags = std::ios::fmtflags(0));
#else
        /** Converts a double to a std::string. */
        static std::string toString(double val, unsigned short precision = 6,
                               unsigned short width = 0, char fill = ' ',
                               std::ios::fmtflags flags = std::ios::fmtflags(0));
#endif
        /** Converts an int to a std::string. */
        static std::string toString(int val, unsigned short width = 0, 
            char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0));
#if PAC_PLATFORM != PAC_PLATFORM_NACL &&  ( PAC_ARCH_TYPE == PAC_ARCHITECTURE_64 || PAC_PLATFORM == PAC_PLATFORM_APPLE || PAC_PLATFORM == PAC_PLATFORM_APPLE_IOS )
        /** Converts an unsigned int to a std::string. */
        static std::string toString(unsigned int val, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0));
        /** Converts a size_t to a std::string. */
        static std::string toString(size_t val, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0));
        #if PAC_COMPILER == PAC_COMPILER_MSVC
        /** Converts an unsigned long to a std::string. */
        static std::string toString(unsigned long val, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0));
        #endif
#else
        /** Converts a size_t to a std::string. */
        static std::string toString(size_t val, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0));
        /** Converts an unsigned long to a std::string. */
        static std::string toString(unsigned long val, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0));
#endif
        /** Converts a long to a std::string. */
        static std::string toString(long val, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0));
        /** Converts a boolean to a std::string. 
        @param yesNo If set to true, result is 'yes' or 'no' instead of 'true' or 'false'
        */
        static std::string toString(bool val, bool yesNo = false);
        /** Converts a StringVector to a string.
        @remarks
            Strings must not contain spaces since space is used as a delimiter in
            the output.
        */
        static std::string toString(const StringVector& val);

        /** Converts a std::string to a Real. 
        @return
            0.0 if the value could not be parsed, otherwise the Real version of the std::string.
        */
        static Real parseReal(const std::string& val, Real defaultValue = 0);
        /** Converts a std::string to a whole number. 
        @return
            0.0 if the value could not be parsed, otherwise the numeric version of the std::string.
        */
        static int parseInt(const std::string& val, int defaultValue = 0);
        /** Converts a std::string to a whole number. 
        @return
            0.0 if the value could not be parsed, otherwise the numeric version of the std::string.
        */
        static unsigned int parseUnsignedInt(const std::string& val, unsigned int defaultValue = 0);
        /** Converts a std::string to a whole number. 
        @return
            0.0 if the value could not be parsed, otherwise the numeric version of the std::string.
        */
        static long parseLong(const std::string& val, long defaultValue = 0);
        /** Converts a std::string to a whole number. 
        @return
            0.0 if the value could not be parsed, otherwise the numeric version of the std::string.
        */
        static unsigned long parseUnsignedLong(const std::string& val, unsigned long defaultValue = 0);
        /** Converts a std::string to size_t. 
        @return
            defaultValue if the value could not be parsed, otherwise the numeric version of the std::string.
        */
        static size_t parseSizeT(const std::string& val, size_t defaultValue = 0);
        /** Converts a std::string to a boolean. 
        @remarks
            Returns true if case-insensitive match of the start of the string
            matches "true", "yes" or "1", false otherwise.
        */
        static bool parseBool(const std::string& val, bool defaultValue = 0);

        /** Parses a StringVector from a string.
        @remarks
            Strings must not contain spaces since space is used as a delimiter in
            the output.
        */
        static StringVector parseStringVector(const std::string& val);
        /** Checks the std::string is a valid number value. */
        static bool isNumber(const std::string& val);

		/**
		 * check if path starts with delim 
		 * @param path : path 
		 * @return : true if it's absolute path 
		 */
		static bool isAbsolutePath(const std::string& path);

		/**
		 * Checks the std::string is a valid decimal type . This should work from
		 * unsigned short, short ..... until long long.
		 * @param val : string value 
		 */
		template<class T> 
		static bool isPrimitiveDecimal(const std::string& val)
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
		template<class T> 
		static T parsePrimitiveDecimal(const std::string& val)
		{
			StringStream str(val);
			if (msUseLocale)
				str.imbue(msLocale);

			T t;
			str >> t;

			if(str.fail() || !str.eof())
				PAC_EXCEPT(Exception::ERR_INVALIDPARAMS, val +  " is not paseable");

			return t;
		}

        //-----------------------------------------------------------------------
        static void setDefaultStringLocale(std::string loc)
        {
            msDefaultStringLocale = loc;
            msLocale = std::locale(msDefaultStringLocale.c_str());
        }
        //-----------------------------------------------------------------------
        static std::string getDefaultStringLocale(void) { return msDefaultStringLocale; }
        //-----------------------------------------------------------------------
        static void setUseLocale(bool useLocale) { msUseLocale = useLocale; }
        //-----------------------------------------------------------------------
        static bool isUseLocale() { return msUseLocale; }
        //-----------------------------------------------------------------------
		
		/**
		 * Remove last path component. If path ends with pac::delim, only the
		 * delim will be removed. If path has no pac::delim, . will be be
		 * returned. if path equals to pac::delim, pac::delim will be returned.
		 * If path is empty return empty.
		 * @param path : path
		 * @return : path without last component
		 */
		static std::string getHead(const std::string& path);

		/**
		 * Get last part of path component, If path ends with pac::delim, you
		 * get blank. If path has no pac:delim you get original path
		 * @param path : 
		 * @return : tail component
		 */
		static std::string getTail(const std::string& path);

    protected:
        static std::string msDefaultStringLocale;
        static std::locale msLocale;
        static bool msUseLocale;
        /// Constant blank string, useful for returning by ref where local does not exist
        static const std::string BLANK;
    };

	/** @} */
	/** @} */

} // namespace Ogre


#endif // PACSTRINGUTIL_H
