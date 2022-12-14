#pragma once
#include "../../CoreDLL/Gui/Controls/Window/PanelWindow.h"
#include "../../CoreDLL/Gui/Controls/IBorderElement.h"
#include "../../CoreDLL/Gui/Controls/List/ScrolledList.h"

using namespace Nk;

class ControlsPanel : public PanelWindow, public IBorderElement {
public:
	ControlsPanel(Widget* widget);

private:
	ScrolledList* m_widgetsList;
};
