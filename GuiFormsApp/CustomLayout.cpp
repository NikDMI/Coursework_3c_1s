#include "ICustomLayout.h"

using namespace Nk;

CustomLayout::CustomLayout(): StackLayout(StackType::VERTICAL, StackAlignment::NORMAL) {

}

void CustomLayout::ComputeWidgetsPositions() {
	StackLayout::ComputeWidgetsPositions();
	Widget* lastPanel = m_layoutWidgets.back();
	Widget* parent = lastPanel->GetParentWidget();
	Rect_t parentRect = parent->GetWidgetClientRect();
	Rect_t widgetRect = lastPanel->GetWidgetRect();
	lastPanel->SetWindowGeometry(widgetRect.x, widgetRect.y, parentRect.w, (parentRect.h - widgetRect.y - BOTTOM_OFFSET));
}