#pragma once
#include "../../CoreDLL/Gui/Controls/Window/PanelWindow.h"
#include "../../CoreDLL/Gui/Controls/Caption/ICaption.h"

using namespace Nk;

class ControlElement : public PanelWindow {
public:

	ControlElement(Widget* parentWidget, Widget* innerWidget);

private:
	Widget* m_innerWidget;
	Widget* m_outerPanel;

	Coord_t m_borderWidth = 10;
};
