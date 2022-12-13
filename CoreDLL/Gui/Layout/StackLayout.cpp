#include "StackLayout.h"


namespace Nk {

	StackLayout::StackLayout(StackType stackType, StackAlignment stackAlignment, StackSpace stackSpace) : ILayout{}, m_stackType{stackType},
		m_stackAlignment{ stackAlignment }, m_stackSpace{stackSpace}
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
		{
			int semiSpaceBetween = 0;
			if (m_stackSpace == StackLayout::StackSpace::SEMI_SPACE_AROUND) {
				int spaceCount = m_layoutWidgets.size() + 1;
				Coord_t widgetsW = 0;
				for (auto widget : m_layoutWidgets) {
					widgetsW += widget->GetWidgetRect().w;
				}
				Coord_t freeSpace = parentRect.w - widgetsW;
				if (freeSpace > 0) {
					semiSpaceBetween = freeSpace / (float)spaceCount;
					currentMainCoord = semiSpaceBetween;//First offset
				}
			}
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
				currentMainCoord += childRect.w + m_spaceBetweenWidgets + semiSpaceBetween;
			}
		}
			break;

		case StackType::VERTICAL:
			int semiSpaceBetween = 0;
			if (m_stackSpace == StackLayout::StackSpace::SEMI_SPACE_AROUND) {
				int spaceCount = m_layoutWidgets.size() + 1;
				Coord_t widgetsH = 0;
				for (auto widget : m_layoutWidgets) {
					widgetsH += widget->GetWidgetRect().h;
				}
				Coord_t freeSpace = parentRect.h - widgetsH;
				if (freeSpace > 0) {
					semiSpaceBetween = freeSpace / (float)spaceCount;
					currentMainCoord = semiSpaceBetween;//First offset
				}
			}
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
				currentMainCoord += childRect.h + m_spaceBetweenWidgets + semiSpaceBetween;
			}
			break;
		}
	}
}