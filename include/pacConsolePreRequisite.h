#ifndef PACCONSOLEPREREQUISITE_H
#define PACCONSOLEPREREQUISITE_H 

#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>
#include <assert.h>

#include "pacConfig.h"

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

namespace pac
{

	typedef float Real;
	typedef std::string String; 
	typedef std::map<String, String> StrStrMap;
	typedef std::vector<String> StringVector;
	typedef std::vector<int> IntVector;
	typedef std::vector<float> FloatVector;
	typedef std::set<std::string> StringSet;
	typedef std::stringstream StringStream;
	typedef std::map<String, String> NameValuePairList;

	class Console;
	class Command;
	class AbsDir;
	class UiConsole;
	class ArgHandler;
	class NodeArgHandler;
	class TreeArgHandler;
	class StringInterface;
	//easy singleton
}

#define sgConsole pac::Console::getSingleton()
#define sgArgLib pac::ArgHandlerLib::getSingleton()
#define sgDirRoot pac::RootDir::getSingleton()

#endif /* PACCONSOLEPREREQUISITE_H */
