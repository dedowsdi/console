#include "pacStable.h"
#include "MyguiConsoleUI.h"

namespace pac {

//------------------------------------------------------------------------------
MyguiConsoleUI::MyguiConsoleUI()
    : mwWnd(0), mwCmd(0), mwCwd(0), mwOutput(0), mFontWidth(0) {
  buildGui();
  // @TODO trying to get font width directly, but it's not precise, i must miss
  // someghing.
  //MyGUI::IFont* font =
      //MyGUI::FontManager::getInstance().getByName(mwOutput->getFontName());
  //auto glyph = font->getGlyphInfo('A');
  //auto swt = mwOutput->getSubWidgetText();
  //mFontWidth =
      //(glyph->advance) / (font->getDefaultHeight()) * swt->getFontHeight();

  // use textview to get font size, don't insert special character in output
  // before this!!!!
   MyGUI::IntSize textSize = mwOutput->getTextSize();
   mFontWidth =
   static_cast<Real>(textSize.width) / mwOutput->getCaption().size();

  mwOutput->setCaption("");
  mwCmd->setCaption("");
  mwCwd->setCaption("");
}

//------------------------------------------------------------------------------
void MyguiConsoleUI::output(const std::string& output, int type /*= 1*/) {
  mwOutput->setCaption(mwOutput->getCaption() + output);
}

//------------------------------------------------------------------------------
void MyguiConsoleUI::setCwd(const std::string& cwd) {
  //resize cwd
  mwCwd->setCaption(cwd);
  mwCwd->setSize(mwCwd->getTextSize());

  //resize cmd
  const MyGUI::IntSize& size_view =
      MyGUI::RenderManager::getInstance().getViewSize();
  mwCmd->setPosition(mwCwd->getPosition().left + mwCwd->getTextSize().width,
      mwCmd->getPosition().top);
  mwCmd->setSize(
      size_view.width - mwCwd->getSize().width, mwCmd->getSize().height);
}

//------------------------------------------------------------------------------
void MyguiConsoleUI::setCmdLine(const std::string& cmdLine) {
  mwCmd->setCaption(cmdLine);
}

//------------------------------------------------------------------------------
std::string MyguiConsoleUI::getCmdLine() { return mwCmd->getCaption(); }

//------------------------------------------------------------------------------
Real MyguiConsoleUI::getAlpha() const { return mwWnd->getAlpha(); }

//------------------------------------------------------------------------------
void MyguiConsoleUI::setAlpha(Real v) { mwWnd->setAlpha(v); }

//------------------------------------------------------------------------------
bool MyguiConsoleUI::getVisible() const { return mwWnd->getVisible(); }

//------------------------------------------------------------------------------
void MyguiConsoleUI::setVisible(bool v) { mwWnd->setVisible(v); }

//------------------------------------------------------------------------------
void MyguiConsoleUI::setFocus(bool v) {
  MyGUI::Widget* focus = v ? mwCmd : NULL;
  MyGUI::InputManager::getInstance().setKeyFocusWidget(focus);
}

//------------------------------------------------------------------------------
Real MyguiConsoleUI::getOutputWidgetWidth() const {
  return mwOutput->getWidth();
}

//------------------------------------------------------------------------------
Real MyguiConsoleUI::getFontWidth() const { return mFontWidth; }

//------------------------------------------------------------------------------
void MyguiConsoleUI::buildGui() {
  // load console layout
  MyGUI::VectorWidgetPtr widgets =
      MyGUI::LayoutManager::getInstance().loadLayout("console.layout");

  mwWnd = static_cast<MyGUI::Window*>(*widgets.begin());
  mwWnd->setVisible(false);

  mwCmd = static_cast<MyGUI::EditBox*>(mwWnd->findWidget("cmd"));
  mwCwd = static_cast<MyGUI::TextBox*>(mwWnd->findWidget("cwd"));
  mwOutput = static_cast<MyGUI::EditBox*>(mwWnd->findWidget("output"));
}
}
