#ifndef PACUICONSOLE_H
#define PACUICONSOLE_H
#include "pacStringInterface.h"

namespace pac {

class ConsoleUI : public StringInterface {
public:
  ConsoleUI();
  ~ConsoleUI(){};

  /**
   * Fake stdout and stderr. Wrap line automatically.
   * @param output : outout content
   * @param type : 1 stdout, 2 stderr
   * @return : *this
   */
  virtual void output(const std::string& output, int type = 1){};

  /**
   * Fake stdout and stderr. Wrap line automatically.
   * @param output : outout content
   * @param type : 1 stdout, 2 stderr
   * @return : *this
   */
  virtual void outputLine(const std::string& output, int type = 1);

  virtual std::string getLineBreak() { return "\n"; }

  /**
   * Complete current typing.
   * @param s : content
   */
  void complete(const std::string& s);
  virtual void setCwd(const std::string& cwd) = 0;
  virtual std::string getCwd() { return ""; }
  virtual void setCmdLine(const std::string& cmdLine) = 0;
  virtual std::string getCmdLine() = 0;
  virtual Real getAlpha() const = 0;
  virtual void setAlpha(Real v) = 0;
  virtual bool getVisible() const = 0;
  virtual void setVisible(bool v) = 0;
  virtual void setFocus(bool v) = 0;
  virtual Real getOutputWidgetWidth() const = 0;
  virtual Real getFontWidth() const = 0;

  /**
   * get line text width
   * @return : line text width
   */
  int getTextWidth() const { return getOutputWidgetWidth() / getFontWidth(); }

  class Alpha : public ParamCmd {
  public:
    Alpha() : ParamCmd("npreal") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  static Alpha msAlpha;

protected:
  void initParams();
};
}

#endif /* PACUICONSOLE_H */
