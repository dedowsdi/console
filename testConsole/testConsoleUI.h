#ifndef TESTPACUICONSOLE_H
#define TESTPACUICONSOLE_H
#include "pacConsoleUI.h"
#include "pacStringUtil.h"
#include <gtest/gtest.h>

namespace pac {

StringVector getSortedVector(std::initializer_list<std::string> items) {
  StringVector sv(items.begin(), items.end());
  std::sort(sv.begin(), sv.end());
  return sv;
}

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

  /**
   * test last output items. don't use this, use getItems instead
   */
  void equalItems(std::initializer_list<std::string> items) {
    StringVector&& sv = StringUtil::split(mLastOutput);
    std::sort(sv.begin(), sv.end());
    StringVector sv1(items.begin(), items.end());
    std::sort(sv1.begin(), sv1.end());
    EXPECT_EQ(sv1, sv);
  }

  StringVector getItems(){
    StringVector&& sv = StringUtil::split(mLastOutput);
    std::sort(sv.begin(), sv.end());
    return sv;
  }

  std::string mCmdLine;
  std::string mCwd;
  std::string mLastOutput;
  Real mAlpha;
};
}

#endif  // TESTPACUICONSOLE_H
