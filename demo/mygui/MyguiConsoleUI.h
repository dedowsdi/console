#ifndef MYGUIUICONSOLE_H
#define MYGUIUICONSOLE_H

#include "pacConsolePreRequisite.h"
#include <MyGUI.h>
#include <MyGUI_Platform.h>
#include "pacConsoleUI.h"

namespace pac {

class _PacExport MyguiConsoleUI : public ConsoleUI {
public:
  MyguiConsoleUI();
  virtual void output(const std::string& output, int type = 1);
  virtual void setCwd(const std::string& cwd);
  virtual void setCmdLine(const std::string& cmdLine);
  virtual std::string getCmdLine();
  virtual Real getAlpha() const;
  virtual void setAlpha(Real v);
  virtual bool getVisible() const;
  virtual void setVisible(bool v);
  virtual void setFocus(bool v);
  
  virtual Real getOutputWidgetWidth() const ;
  virtual Real getFontWidth() const ;

private:
  void buildGui();

protected:
  MyGUI::Window* mwWnd;
  MyGUI::EditBox* mwCmd;
  MyGUI::TextBox* mwCwd;
  MyGUI::EditBox *mwOutput;
  Real mFontWidth;
};
}

#endif /* MYGUIUICONSOLE_H */
