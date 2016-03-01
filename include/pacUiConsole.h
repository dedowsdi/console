#ifndef PACUICONSOLE_H
#define PACUICONSOLE_H
#include "pacStringInterface.h"

namespace pac {

class UiConsole : public StringInterface {
public:
  UiConsole();
  ~UiConsole(){};

  void init();

  /**
   * By default every line ows 80 characters + 1 \n . Override if you want to
   * change it.
   * @remark :
   * @return :
   */
  virtual void setUpTextMetric();

  /**
   * Fake stdout and stderr. Wrap line automatically.
   * @param output : outout content
   * @param type : 1 stdout, 2 stderr
   * @return : *this
   */
  virtual UiConsole& output(const std::string& output, int type = 1);

  /**
   * Fake stdout and stderr. Wrap line automatically.
   * @param output : outout content
   * @param type : 1 stdout, 2 stderr
   * @return : *this
   */
  virtual UiConsole& outputLine(const std::string& output, int type = 1);

  /**
   * end current line
   * @return : *this
   */
  virtual UiConsole& endl() = 0;
  /**
   * Fake stdout and stderr. No Wrap line.
   * @param output : outout content
   * @param type : 1 stdout, 2 stderr
   * @return : *this
   */
  virtual UiConsole& outputNoAutoWrap(
      const std::string& output, int type = 1) = 0;

  /**
   * Complete current typing.
   * @param s : content
   */
  void complete(const std::string& s);
  virtual void setCwd(const std::string& cwd) = 0;
  virtual std::string getCwd() { return ""; };
  virtual void setCmdLine(const std::string& cmdLine) = 0;
  virtual std::string getCmdLine() = 0;
  virtual Real getAlpha() const = 0;
  virtual void setAlpha(Real v) = 0;

  /**
   * line text width, default to 80. You can change it by overloading
   * setupTextMetric()
   * @return : line text width
   */
  int getTextWidth() const { return mTextWidth; }

  class Alpha : public ParamCmd {
  public:
    Alpha() : ParamCmd("npreal") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  static Alpha msAlpha;

protected:
  void setTextWidth(int v) { mTextWidth = v; }
  void initParams();

private:
  int mTextWidth;
};
}

#endif /* PACUICONSOLE_H */
