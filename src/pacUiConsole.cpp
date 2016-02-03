#include "pacUiConsole.h"

namespace pac
{

//------------------------------------------------------------------
UiConsole::UiConsole():
	mTextWidth(80)
{
}

//------------------------------------------------------------------
void UiConsole::init()
{
	setUpTextMetric();
}

//------------------------------------------------------------------
void UiConsole::setUpTextMetric()
{
	mTextWidth = 80;
}

//------------------------------------------------------------------
UiConsole& UiConsole::output(const std::string& output, int type /*=1*/)
{
	this->outputNoAutoWrap(output, type);
	return *this;
}

//------------------------------------------------------------------
UiConsole& UiConsole::outputLine(const std::string& output, int type /*=1*/)
{
	return this->endl().output(output, type);
}

}

