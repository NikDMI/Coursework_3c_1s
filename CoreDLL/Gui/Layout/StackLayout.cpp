#include "StackLayout.h"


namespace Nk {

	StackLayout::StackLayout(StackType stackType, StackAlignment stackAlignment) : ILayout{}, m_stackType{stackType},
		m_stackAlignment{stackAlignment}
	{

	}


	StackLayout::~StackLayout() {

	}

	/*
	* Draw layout widgets
	*/
	void StackLayout::ComputeWidgetsPositions() {
		if (!m_layoutWidgets.size()) {
			return;
		}
		//Get parent rect
		const Rect_t parentRect = m_layoutWidgets.back()->GetParentWidget()->GetWidgetClientRect();
		Coord_t currentMainCoord = 0;
		Coord_t currentHelpCoord = 0;	//coords by help dimension
		switch (m_stackType) {

		case StackType::HORIZONTAL:
			for (Widget* child : m_layoutWidgets) {
				const Rect_t& childRect = child->GetWidgetRect();
				//Choose coord by help dimension
				switch (m_stackAlignment) {
				case StackAlignment::NORMAL:
				case StackAlignment::ALL_SPACE:
					currentHelpCoord = 0;
					break;
				case StackAlignment::CENTER:
					currentHelpCoord = (parentRect.h - childRect.h) / 2;
					break;
				case StackAlignment::REVERSE:
					currentHelpCoord = (parentRect.h - childRect.h);
					break;
				}
				//Position widget
				Coord_t widgetH = m_stackAlignment == StackAlignment::ALL_SPACE ? parentRect.h : childRect.h;
				child->SetWindowGeometry(currentMainCoord, currentHelpCoord, childRect.w, widgetH);
				currentMainCoord += childRect.w + m_spaceBetweenWidgets;
			}
			break;

		case StackType::VERTICAL:
			for (Widget* child : m_layoutWidgets) {
				const Rect_t& childRect = child->GetWidgetRect();
				//Choose coord by help dimension
				switch (m_stackAlignment) {
				case StackAlignment::NORMAL:
				case StackAlignment::ALL_SPACE:
					currentHelpCoord = 0;
					break;
				case StackAlignment::CENTER:
					currentHelpCoord = (parentRect.w - childRect.w) / 2;
					break;
				case StackAlignment::REVERSE:
					currentHelpCoord = (parentRect.w - childRect.w);
					break;
				}
				//Position widget
				Coord_t widgetW = m_stackAlignment == StackAlignment::ALL_SPACE ? parentRect.w : childRect.w;
				child->SetWindowGeometry(currentHelpCoord, currentMainCoord, widgetW, childRect.h);
				currentMainCoord += childRect.h + m_spaceBetweenWidgets;
			}
			break;
		}
	}
}