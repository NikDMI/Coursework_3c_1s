#pragma once
#include "../../CoreDLL/Gui/Controls/Window/PanelWindow.h"
#include "../../CoreDLL/Gui/Controls/IBorderElement.h"

using namespace Nk;

class Form : public PanelWindow, public IBorderElement {
public:
	enum class Controls {LABEL, EDIT};

	Form(Widget* widget);

	void AddNewControl(Controls control);

private:

	Color_t m_formColor = {0.8, 0.8, 0.8, 1.0};
};
