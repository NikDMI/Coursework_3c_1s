#pragma once
#include "../../CoreDLL/Gui/Controls/Window/PanelWindow.h"
#include "../../CoreDLL/Gui/Controls/IBorderElement.h"

using namespace Nk;

class Form : public PanelWindow, public IBorderElement {
public:
	Form(Widget* widget);

private:

	Color_t m_formColor = {0.8, 0.8, 0.8, 1.0};
};
