#pragma once
#include "../CoreDLL/Gui/Controls/Window/PanelWindow.h"
#include "Controls/ControlsPanel.h"
#include "Controls/FormPanel.h"
#include "Controls/PropertiesPanel.h"

using namespace Nk;

class MainControlPanel : public PanelWindow {
public:
	MainControlPanel(Widget* parent);
private:
	ControlsPanel* m_controlPanel;
	FormPanel* m_formPanel;
	PropertiesPanel* m_propertiesPanel;
};