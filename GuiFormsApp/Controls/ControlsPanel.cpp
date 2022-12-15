#include "ControlsPanel.h"
#include "../../CoreDLL/Gui/ResizeManager/RectangleResizer.h"
#include "../../CoreDLL/Gui/Layout/DefaultLayout.h"
#include "../GuiConfigs.h"

const Color_t INNER_PANEL_COLOR = { 0.6, 0.6 , 0.6, 1.0 };
const Color_t INNER_PANEL_BORDER_COLOR = { 0.5, 0.5 , 0.5, 1.0 };

class ControlsPanelLayout : public DefaultLayout {
public:

	ControlsPanelLayout(): DefaultLayout() {

	}

	void ComputeWidgetsPositions() {
		Widget* listWidget = m_layoutWidgets.back();
		ControlsPanel* panelWidget = (ControlsPanel*)listWidget->GetParentWidget();
		Rect_t panelRect = panelWidget->GetWidgetClientRect();
		Coord_t availableWidth = panelRect.w - DELTA_WIDTH * 2;
		//
		panelWidget->m_captionLabel->SetWindowGeometry(DELTA_WIDTH, DELTA_HEIGHT, availableWidth, LABEL_HEIGHT);
		Coord_t listHeight = panelRect.h - DELTA_HEIGHT - LABEL_HEIGHT - SPACE_BETWEEN_CONTROLS - DELTA_HEIGHT;
		panelWidget->m_widgetsList->SetWindowGeometry(DELTA_WIDTH, DELTA_HEIGHT + LABEL_HEIGHT + SPACE_BETWEEN_CONTROLS,
			availableWidth, listHeight);
	}

private:
	const float DELTA_HEIGHT = 10;
	const float DELTA_WIDTH = 10;
	const float LABEL_HEIGHT = 25;
	const float SPACE_BETWEEN_CONTROLS = 10;

};


ControlsPanel::ControlsPanel(Widget* widget) : PanelWindow (widget), IBorderElement(this){
	RectangleResizer* rectangleResizer = new RectangleResizer();
	AddNewLayout(new ControlsPanelLayout());
	SetWindowGeometry(0, 0, 300, 300);
	SetBackgroundColor(INNER_PANEL_COLOR);
	SetResizeManager(rectangleResizer);
	SetNormalBorder(INNER_PANEL_BORDER_COLOR);
	//Caption
	m_captionLabel = new Label(L"Контролы", this);
	m_captionLabel->SetBackgroundColor(INNER_PANEL_COLOR);
	SetDefaultCenterFont(m_captionLabel->GetElementFont());
	//WidgetList
	m_widgetsList = new ScrolledList(this);
	m_widgetsList->SetWindowGeometry(0, 0, 300, 300);
	
}