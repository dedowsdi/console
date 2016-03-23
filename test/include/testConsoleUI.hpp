#ifndef TESTPACUICONSOLE_H
#define TESTPACUICONSOLE_H
#include "pacConsoleUI.h"

namespace pac {

class ImplConsoleUI : public ConsoleUI {
public:
  ImplConsoleUI() : mAlpha(1.0f) {}

  virtual void setCmdLine(const std::string& cmdLine) { mCmdLine = cmdLine; }
  virtual std::string getCmdLine() { return mCmdLine; }

  virtual void setCwd(const std::string& cwd) { mCwd = cwd; }
  virtual std::string getCwd() { return mCwd; }
  virtual Real getOutputWidgetWidth() const { return 800; }
  virtual Real getFontWidth() const { return 10; }

  virtual Real getAlpha() const { return mAlpha; }
  virtual void setAlpha(Real v) { mAlpha = v; }
  virtual bool getVisible() const { return true; }
  virtual void setVisible(bool v) { (void)v; }
  virtual void setFocus(bool v) { (void)v; }

  virtual void output(const std::string& output, int type = 1) {
    mLastOutput = output;
  }

  const std::string& getLastOutput() const { return mLastOutput; }
  void setLastOutput(const std::string& v) { mLastOutput = v; }

  std::string mCmdLine;
  std::string mCwd;
  std::string mLastOutput;
  Real mAlpha;
};
}

#endif  // TESTPACUICONSOLE_H
