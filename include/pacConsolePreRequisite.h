#ifndef PACCONSOLEPREREQUISITE_H
#define PACCONSOLEPREREQUISITE_H 

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>
#include <assert.h>

#include "pacConfig.h"
#include "pacPlatform.h"

//@TODO, fix manual define PAC_STATIC_LIB
#define PAC_STATIC_LIB
// If we're not including this from a client build, specify that the stuff
// should get exported. Otherwise, import it.
#	if defined( PAC_STATIC_LIB )
		// Linux compilers don't have symbol import/export directives.
#   	define _PacExport
#   	define _PacPrivate
#   else
#   	if defined( PAC_NONCLIENT_BUILD )
#       	define _PacExport __declspec( dllexport )
#   	else
#           if defined( __MINGW32__ )
#               define _PacExport
#           else
#       	    define _PacExport __declspec( dllimport )
#           endif
#   	endif
#   	define _PacPrivate
#	endif


#define XSTR(x) STR(x)
#define STR(x) #x
#define VAR_NAME_VALUE(var) #var "=" XSTR(var)

//#pragma message(VAR_NAME_VALUE(PAC_PLATFORM))
//#pragma message(VAR_NAME_VALUE(PAC_COMPILER))

typedef std::map<std::string, std::string> StrStrMap;
typedef std::vector<std::string> StringVector;
typedef StringVector::iterator SVIter;
typedef std::vector<int> IntVector;
typedef std::vector<size_t> SizetVector;
typedef std::vector<float> FloatVector;
typedef std::set<std::string> StringSet;
typedef std::stringstream StringStream;
typedef std::map<std::string, std::string> NameValuePairList;

namespace pac
{

	typedef float Real;

	class Console;
	class Command;
	class AbsDir;
	class UiConsole;
	class ArgHandler;
	class NodeArgHandler;
	class TreeArgHandler;
	class StringInterface;
	class ConsolePattern;
	class DefaultPattern;
	//easy singleton
}

#define sgConsole pac::Console::getSingleton()
#define sgArgLib pac::ArgHandlerLib::getSingleton()
#define sgDirRoot pac::RootDir::getSingleton()

#endif /* PACCONSOLEPREREQUISITE_H */
