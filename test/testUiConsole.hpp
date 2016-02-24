#ifndef TESTPACUICONSOLE_H
#define TESTPACUICONSOLE_H
#include "pacUiConsole.h"

using namespace pac;

class ImplUiConsole : public UiConsole {
public:
  ImplUiConsole() : mAlpha(1.0f) {}
  virtual UiConsole& endl() {
    mLastOutput += "\n";
    return *this;
  }

  virtual UiConsole& outputNoAutoWrap(const std::string& output, int type = 1) {
    mLastOutput = output;
    return *this;
  }

  virtual void setCmdLine(const std::string& cmdLine) { mCmdLine = cmdLine; }
  virtual std::string getCmdLine() { return mCmdLine; }

  virtual void setCwd(const std::string& cwd) { mCwd = cwd; }
  virtual std::string getCwd() { return mCwd; }

  virtual Real getAlpha() const { return mAlpha; }
  virtual void setAlpha(Real v) { mAlpha = v; }

  const std::string& getLastOutput() const { return mLastOutput; }
  void setLastOutput(const std::string& v) { mLastOutput = v; }

  std::string mCmdLine;
  std::string mCwd;
  std::string mLastOutput;
  Real mAlpha;
};

#endif  // TESTPACUICONSOLE_H
