#include "MainControlPanel.h"
#include "MainPanelLayout.h"
#include "../CoreDLL/Gui/ResizeManager/RectangleResizer.h"

const Color_t INNER_PANEL_COLOR = { 0.6, 0.6 , 0.6, 1.0 };
const Color_t INNER_PANEL_BORDER_COLOR = { 0.5, 0.5 , 0.5, 1.0 };

MainControlPanel::MainControlPanel(Widget* parent): PanelWindow(parent, 0, 0, 0, 300) {
	RectangleResizer* rectangleResizer = new RectangleResizer();

	AddNewLayout(new MainPanelLayout());
	m_controlPanel = new ControlsPanel(this);

	m_formPanel = new FormPanel(this);

	m_propertiesPanel = new PropertiesPanel(this);
}