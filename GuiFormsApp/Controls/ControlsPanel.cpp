#include "ControlsPanel.h"
#include "../../CoreDLL/Gui/ResizeManager/RectangleResizer.h"
#include "../../CoreDLL/Gui/Layout/DefaultLayout.h"
#include "../GuiConfigs.h"
#include "../Bean/UserControls.h"

const Color_t INNER_PANEL_COLOR = { 0.6, 0.6 , 0.6, 1.0 };
const Color_t INNER_PANEL_BORDER_COLOR = { 0.5, 0.5 , 0.5, 1.0 };

void CustomListItem(LabelImageButton* lblImage);

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
		Rect_t listRect = { DELTA_WIDTH, DELTA_HEIGHT + LABEL_HEIGHT + SPACE_BETWEEN_CONTROLS,
			availableWidth, listHeight };
		panelWidget->m_widgetsList->SetWindowGeometry(listRect.x, listRect.y,
			listRect.w, listRect.h);
		if (m_lastListRect.w != listRect.w) {
			panelWidget->m_widgetsList->GetIList()->RecomputeListLayout();
			m_lastListRect = listRect;
		}
	}

private:
	const float DELTA_HEIGHT = 10;
	const float DELTA_WIDTH = 10;
	const float LABEL_HEIGHT = 25;
	const float SPACE_BETWEEN_CONTROLS = 10;
	Rect_t m_lastListRect = {0, 0, 0, 0};

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
	ConfigScrollBarColor(m_widgetsList->GetVerticalScrollBar());
	//Add list items

	UserControl* labelControl = new UserControl(L"Images/1.jpg", L"Label", m_widgetsList->GetIList());
	CustomListItem(labelControl->labelImageButton);
	m_widgetsList->GetIList()->AddListItem(labelControl->labelImageButton);

	UserControl* btnControl = new UserControl(L"Images/1.jpg", L"Button", m_widgetsList->GetIList());
	CustomListItem(btnControl->labelImageButton);
	m_widgetsList->GetIList()->AddListItem(btnControl->labelImageButton);

	UserControl* editBoxControl = new UserControl(L"Images/1.jpg", L"EditBox", m_widgetsList->GetIList());
	CustomListItem(editBoxControl->labelImageButton);
	m_widgetsList->GetIList()->AddListItem(editBoxControl->labelImageButton);

	UserControl* scrollBarControl = new UserControl(L"Images/1.jpg", L"ScrollBar", m_widgetsList->GetIList());
	CustomListItem(scrollBarControl->labelImageButton);
	m_widgetsList->GetIList()->AddListItem(scrollBarControl->labelImageButton);

	m_widgetsList->GetIList()->SetWindowGeometry(0, 0, 300, 300);
	m_widgetsList->GetIList()->RecomputeListLayout();
	
}


const Color_t ITEM_LIST_STATIC_COLOR = { 0.5, 0.5 , 0.5, 1.0 };
const Color_t ITEM_LIST_MOVE_COLOR = { 0.6, 0.6 , 0.5, 1.0 };
const Color_t ITEM_LIST_PUSH_COLOR = { 0.4, 0.4 , 0.5, 1.0 };
const Color_t ITEM_LIST_BORDER_COLOR = { 0.4, 0.4 , 0.4, 1.0 };



void CustomListItem(LabelImageButton* lblImage) {
	//lblImage->SetNormalBorder(ITEM_LIST_BORDER_COLOR);
	lblImage->SetButtonColor(IColorElement::ElementState::STATIC, ITEM_LIST_STATIC_COLOR);
	lblImage->SetButtonColor(IColorElement::ElementState::HOVER, ITEM_LIST_MOVE_COLOR);
	lblImage->SetButtonColor(IColorElement::ElementState::PUSH, ITEM_LIST_PUSH_COLOR);
	lblImage->SetBackgroundColor(ITEM_LIST_STATIC_COLOR);
	SetDefaultFont(lblImage->GetElementFont());
}