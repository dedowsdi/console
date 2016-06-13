#include "pacConsoleUI.h"
#include "pacStringUtil.h"
#include "pacArgHandler.h"

namespace pac {
ConsoleUI::Alpha ConsoleUI::msAlpha;

//------------------------------------------------------------------------------
ConsoleUI::ConsoleUI() : StringInterface("consoleUi", false) {
  if (createParamDict()) {
    this->initParams();
  }
}

//------------------------------------------------------------------------------
void ConsoleUI::outputLine(const std::string& output, int type /*= 1*/) {
  return this->output(output + getLineBreak(), type);
}

//------------------------------------------------------------------------------
void ConsoleUI::complete(const std::string& s) { setCmdLine(getCmdLine() + s); }

//------------------------------------------------------------------------------
int ConsoleUI::getTextWidth() const {
  // reserve 1 space?
  return getOutputWidgetWidth() / getFontWidth();
}

//------------------------------------------------------------------------------
std::string ConsoleUI::Alpha::doGet(const void* target) const {
  const ConsoleUI* ui = static_cast<const ConsoleUI*>(target);
  return StringUtil::toString(ui->getAlpha());
}

//------------------------------------------------------------------------------
void ConsoleUI::Alpha::doSet(void* target, ArgHandler* handler) {
  ConsoleUI* ui = static_cast<ConsoleUI*>(target);
  ui->setAlpha(StringUtil::parsePrimitiveDecimal<Real>(handler->getValue()));
}

//------------------------------------------------------------------------------
void ConsoleUI::initParams() {
  ParamDictionary* dict = this->getParamDict();
  dict->addParameter("alpha", &msAlpha);
}
}
