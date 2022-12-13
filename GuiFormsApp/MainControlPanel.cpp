#include "MainControlPanel.h"
#include "MainPanelLayout.h"
#include "../CoreDLL/Gui/ResizeManager/RectangleResizer.h"

const Color_t INNER_PANEL_COLOR = { 0.6, 0.6 , 0.6, 1.0 };

MainControlPanel::MainControlPanel(Widget* parent): PanelWindow(parent, 0, 0, 0, 300) {
	RectangleResizer* rectangleResizer = new RectangleResizer();
	AddNewLayout(new MainPanelLayout());
	m_controlPanel = new PanelWindow(this, 0, 0, 300, 300);
	m_controlPanel->SetBackgroundColor(INNER_PANEL_COLOR);
	m_controlPanel->SetResizeManager(rectangleResizer);

	m_formPanel = new PanelWindow(this, 0, 0, 500, 300);
	m_formPanel->SetBackgroundColor(INNER_PANEL_COLOR);
	m_formPanel->SetResizeManager(rectangleResizer);

	m_propertiesPanel = new PanelWindow(this, 0, 0, 300, 300);
	m_propertiesPanel->SetBackgroundColor(INNER_PANEL_COLOR);
	m_propertiesPanel->SetResizeManager(rectangleResizer);
}