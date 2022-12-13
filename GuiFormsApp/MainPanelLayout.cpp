#include "MainPanelLayout.h"

using namespace Nk;

const float MainPanelLayout::CHILDS_HEIGHT_COEFF = 0.75;


MainPanelLayout::MainPanelLayout() : StackLayout(StackType::HORIZONTAL, StackAlignment::NORMAL, StackSpace::SEMI_SPACE_AROUND) {

}

void MainPanelLayout::ComputeWidgetsPositions() {
	StackLayout::ComputeWidgetsPositions();
	Widget* lastPanel = m_layoutWidgets.back();
	Widget* parent = lastPanel->GetParentWidget();
	Rect_t parentRect = parent->GetWidgetClientRect();
	Coord_t childHeight = parentRect.h * CHILDS_HEIGHT_COEFF;
	if (childHeight < 400) childHeight = 400;
	for (Widget* child : m_layoutWidgets) {
		Rect_t childRect = child->GetWidgetRect();
		child->SetWindowGeometry(childRect.x, (parentRect.h - childHeight) / 2, childRect.w, childHeight);

	}
}