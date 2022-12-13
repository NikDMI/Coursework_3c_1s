#pragma once
#include "../CoreDLL/Gui/Controls/Window/PanelWindow.h"

using namespace Nk;

class MainControlPanel : public PanelWindow {
public:
	MainControlPanel(Widget* parent);
private:
	PanelWindow* m_controlPanel;
	PanelWindow* m_formPanel;
	PanelWindow* m_propertiesPanel;
};