#pragma once
#include "../../CoreDLL/Gui/Controls/Window/PanelWindow.h"
#include "../../CoreDLL/Gui/Controls/IBorderElement.h"

using namespace Nk;

class PropertiesPanel : public PanelWindow, public IBorderElement {
public:
	PropertiesPanel(Widget* widget);
};
