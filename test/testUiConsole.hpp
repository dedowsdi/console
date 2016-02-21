#ifndef TESTPACUICONSOLE_H
#define TESTPACUICONSOLE_H
#include "pacUiConsole.h"

using namespace pac;

class ImplUiConsole : public UiConsole {
public:
  virtual UiConsole& endl() {
    std::cout << std::endl;
    return *this;
  }

  virtual UiConsole& outputNoAutoWrap(const std::string& output, int type = 1) {
    std::cout << output;
    return *this;
  }

  virtual UiConsole& complete(const std::string& s) {
    std::cout << "complete" << s;
    return *this;
  }

  virtual void setCmdLine(const std::string& cmdLine = "") {
    std::cout << "updateCmdLine:" << cmdLine << std::endl;
  }
  virtual std::string getCmdLine() { return mCmdLine; }

  virtual void setCwd(const std::string& cwd) {
    std::cout << "setCwd:" << cwd << std::endl;
  }

  virtual Real getAlpha() const { return 1.0; }

  virtual void setAlpha(Real v) const {}

  std::string mCmdLine;
};

#endif  // TESTPACUICONSOLE_H
