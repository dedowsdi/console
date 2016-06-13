// copied from ogre:http://www.ogre3d.org/

#ifndef PACEXCEPTION_H
#define PACEXCEPTION_H

#include "pacConsolePreRequisite.h"
#include <exception>

// Check for PAC assert mode

// RELEASE_EXCEPTIONS mode
#if PAC_ASSERT_MODE == 1
#if PAC_DEBUG_MODE
#define PacAssert(a, b) assert((a) && (b))
#define PacAssertS(a, b) assert((a) && ((b).c_str()))
#else
#if PAC_COMP != PAC_COMPILER_BORL
#define PacAssert(a, b) \
  if (!(a))             \
  PAC_EXCEPT(Pac::Exception::ERR_RT_ASSERTION_FAILED, (b), "no function info")
#else
#define PacAssert(a, b) \
  if (!(a)) PAC_EXCEPT(Pac::Exception::ERR_RT_ASSERTION_FAILED, (b), __FUNC__)
#endif
#endif

// EXCEPTIONS mode
#elif PAC_ASSERT_MODE == 2
#if PAC_COMP != PAC_COMPILER_BORL
#define PacAssert(a, b) \
  if (!(a))             \
  PAC_EXCEPT(Pac::Exception::ERR_RT_ASSERTION_FAILED, (b), "no function info")
#else
#define PacAssert(a, b) \
  if (!(a)) PAC_EXCEPT(Pac::Exception::ERR_RT_ASSERTION_FAILED, (b), __FUNC__)
#endif

// STANDARD mode
#else
#define PacAssert(a, b) assert((a) && (b))
#define PacAssertS(a, b) assert((a) && ((b).c_str()))
#endif

namespace pac {
/** \addtogroup Core
*  @{
*/
/** \addtogroup General
*  @{
*/
/** When thrown, provides information about an error that has occurred inside
   the engine.
      @remarks
          OGRE never uses return values to indicate errors. Instead, if an
          error occurs, an exception is thrown, and this is the object that
          encapsulates the detail of the problem. The application using
          OGRE should always ensure that the exceptions are caught, so all
          OGRE engine functions should occur within a
          try{} catch(Pac::Exception& e) {} block.
      @par
          The user application should never create any instances of this
          object unless it wishes to unify its error handling using the
          same object.
  */
class _PacExport Exception : public std::exception {
protected:
  long line;
  int number;
  std::string typeName;
  std::string description;
  std::string source;
  std::string file;
  mutable std::string fullDesc;

public:
  /** Static definitions of error codes.
      @todo
          Add many more exception codes, since we want the user to be able
          to catch most of them.
  */
  enum ExceptionCodes {
    ERR_CANNOT_WRITE_TO_FILE,
    ERR_INVALID_STATE,
    ERR_INVALIDPARAMS,
    ERR_DUPLICATE_ITEM,
    ERR_ITEM_NOT_FOUND,
    ERR_FILE_NOT_FOUND,
    ERR_INTERNAL_ERROR,
    ERR_RT_ASSERTION_FAILED,
    ERR_NOT_IMPLEMENTED
  };

  /** Default constructor.
  */
  Exception(
      int number, const std::string& description, const std::string& source);

  /** Advanced constructor.
  */
  Exception(int number, const std::string& description,
      const std::string& source, const char* type, const char* file, long line);

  /** Copy constructor.
  */
  Exception(const Exception& rhs);

  /// Needed for compatibility with std::exception
  ~Exception() throw() {}

  /** Assignment operator.
  */
  Exception& operator=(const Exception& rhs);

  /** Returns a string with the full description of this error.
      @remarks
          The description contains the error number, the description
          supplied by the thrower, what routine threw the exception,
          and will also supply extra platform-specific information
          where applicable. For example - in the case of a rendering
          library error, the description of the error will include both
          the place in which OGRE found the problem, and a text
          description from the 3D rendering library, if available.
  */
  virtual const std::string& getFullDescription(void) const;

  /** Gets the error code.
  */
  virtual int getNumber(void) const throw();

  /** Gets the source function.
  */
  virtual const std::string& getSource() const { return source; }

  /** Gets source file name.
  */
  virtual const std::string& getFile() const { return file; }

  /** Gets line number.
  */
  virtual long getLine() const { return line; }

  /** Returns a string with only the 'description' field of this exception. Use
    getFullDescriptionto get a full description of the error including line
    number,
    error number and what function threw the exception.
      */
  virtual const std::string& getDescription(void) const { return description; }

  /// Override std::exception::what
  const char* what() const throw() { return getFullDescription().c_str(); }
};

// Specialised exceptions allowing each to be caught specifically
// backwards-compatible since exception codes still used

class _PacExport UnimplementedException : public Exception {
public:
  UnimplementedException(int inNumber, const std::string& inDescription,
      const std::string& inSource, const char* inFile, long inLine)
      : Exception(inNumber, inDescription, inSource, "UnimplementedException",
            inFile, inLine) {}
};
class _PacExport FileNotFoundException : public Exception {
public:
  FileNotFoundException(int inNumber, const std::string& inDescription,
      const std::string& inSource, const char* inFile, long inLine)
      : Exception(inNumber, inDescription, inSource, "FileNotFoundException",
            inFile, inLine) {}
};
class _PacExport IOException : public Exception {
public:
  IOException(int inNumber, const std::string& inDescription,
      const std::string& inSource, const char* inFile, long inLine)
      : Exception(
            inNumber, inDescription, inSource, "IOException", inFile, inLine) {}
};
class _PacExport InvalidStateException : public Exception {
public:
  InvalidStateException(int inNumber, const std::string& inDescription,
      const std::string& inSource, const char* inFile, long inLine)
      : Exception(inNumber, inDescription, inSource, "InvalidStateException",
            inFile, inLine) {}
};
class _PacExport InvalidParametersException : public Exception {
public:
  InvalidParametersException(int inNumber, const std::string& inDescription,
      const std::string& inSource, const char* inFile, long inLine)
      : Exception(inNumber, inDescription, inSource,
            "InvalidParametersException", inFile, inLine) {}
};
class _PacExport ItemIdentityException : public Exception {
public:
  ItemIdentityException(int inNumber, const std::string& inDescription,
      const std::string& inSource, const char* inFile, long inLine)
      : Exception(inNumber, inDescription, inSource, "ItemIdentityException",
            inFile, inLine) {}
};
class _PacExport InternalErrorException : public Exception {
public:
  InternalErrorException(int inNumber, const std::string& inDescription,
      const std::string& inSource, const char* inFile, long inLine)
      : Exception(inNumber, inDescription, inSource, "InternalErrorException",
            inFile, inLine) {}
};
class _PacExport RuntimeAssertionException : public Exception {
public:
  RuntimeAssertionException(int inNumber, const std::string& inDescription,
      const std::string& inSource, const char* inFile, long inLine)
      : Exception(inNumber, inDescription, inSource,
            "RuntimeAssertionException", inFile, inLine) {}
};

/** Class implementing dispatch method in order to construct by-value
  exceptions of a derived type based on an exception code.
*/
class ExceptionFactory {
private:
  /// Private constructor, no construction
  ExceptionFactory() {}

public:
  static PAC_NORETURN void throwException(Exception::ExceptionCodes code,
      int number, const std::string& desc, const std::string& src,
      const char* file, long line) {
    switch (code) {
      case Exception::ERR_CANNOT_WRITE_TO_FILE:
        throw IOException(number, desc, src, file, line);
      case Exception::ERR_INVALID_STATE:
        throw InvalidStateException(number, desc, src, file, line);
      case Exception::ERR_INVALIDPARAMS:
        throw InvalidParametersException(number, desc, src, file, line);
      case Exception::ERR_DUPLICATE_ITEM:
        throw ItemIdentityException(number, desc, src, file, line);
      case Exception::ERR_ITEM_NOT_FOUND:
        throw ItemIdentityException(number, desc, src, file, line);
      case Exception::ERR_FILE_NOT_FOUND:
        throw FileNotFoundException(number, desc, src, file, line);
      case Exception::ERR_INTERNAL_ERROR:
        throw InternalErrorException(number, desc, src, file, line);
      case Exception::ERR_RT_ASSERTION_FAILED:
        throw RuntimeAssertionException(number, desc, src, file, line);
      case Exception::ERR_NOT_IMPLEMENTED:
        throw UnimplementedException(number, desc, src, file, line);
      default:
        throw Exception(number, desc, src, "Exception", file, line);
    }
  }
};

#ifndef PAC_EXCEPT
#define PAC_EXCEPT(code, desc)           \
  pac::ExceptionFactory::throwException( \
      code, code, desc, __FUNCTION__, __FILE__, __LINE__)
#define PAC_EXCEPT_EX(code, num, desc)   \
  pac::ExceptionFactory::throwException( \
      code, num, desc, __FUNCTION__, __FILE__, __LINE__)
#else
#define PAC_EXCEPT_EX(code, num, desc) PAC_EXCEPT(code, desc)
#endif

}  // Namespace Ogrec

#endif
