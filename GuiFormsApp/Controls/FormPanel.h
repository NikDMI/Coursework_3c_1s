#pragma once
#include "../../CoreDLL/Gui/Controls/Window/PanelWindow.h"
#include "../../CoreDLL/Gui/Controls/IBorderElement.h"

using namespace Nk;

class FormPanel : public PanelWindow, public IBorderElement {
public:
	FormPanel(Widget* widget);
};
