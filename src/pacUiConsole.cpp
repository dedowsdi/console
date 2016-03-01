#include "pacUiConsole.h"
#include "pacStringUtil.h"
#include "pacArgHandler.h"

namespace pac {
UiConsole::Alpha UiConsole::msAlpha;

//------------------------------------------------------------------------------
UiConsole::UiConsole() : StringInterface("uiConsole"), mTextWidth(80) {
  if(createParamDict()){
    this->initParams();
  }
}

//------------------------------------------------------------------------------
void UiConsole::init() { setUpTextMetric(); }

//------------------------------------------------------------------------------
void UiConsole::setUpTextMetric() { mTextWidth = 80; }

//------------------------------------------------------------------------------
UiConsole& UiConsole::output(const std::string& output, int type /*=1*/) {
  this->outputNoAutoWrap(output, type);
  return *this;
}

//------------------------------------------------------------------------------
UiConsole& UiConsole::outputLine(const std::string& output, int type /*=1*/) {
  return this->endl().output(output, type);
}

//------------------------------------------------------------------------------
void UiConsole::complete(const std::string& s) { setCmdLine(getCmdLine() + s); }

//------------------------------------------------------------------------------
std::string UiConsole::Alpha::doGet(const void* target) const {
  const UiConsole* ui = static_cast<const UiConsole*>(target);
  return StringUtil::toString(ui->getAlpha());
}

//------------------------------------------------------------------------------
void UiConsole::Alpha::doSet(void* target, ArgHandler* handler) {
  UiConsole* ui = static_cast<UiConsole*>(target);
  ui->setAlpha(StringUtil::parsePrimitiveDecimal<Real>(handler->getValue()));
}

//------------------------------------------------------------------------------
void UiConsole::initParams()
{
  ParamDictionary* dict = this->getParamDict();
  dict->addParameter("alpha", &msAlpha);
}
}
