#include "ScrolledList.h"
#include "../../Layout/ILayout.h"

namespace Nk {

	class ScrollListLayout : public ILayout {

		void ComputeWidgetsPositions() {
			ScrolledList* scrolledList = (ScrolledList*)m_layoutWidgets.back()->GetParentWidget();
			IList* list = scrolledList->GetIList();
			ScrollBar* verticalScroll = scrolledList->GetVerticalScrollBar();
			Rect_t clientRect = scrolledList->GetWidgetClientRect();
			Coord_t verticalsScrollBarX = clientRect.w - ScrolledList::SCROLL_BAR_WIDTH;
			verticalScroll->SetWindowGeometry(verticalsScrollBarX, 0, ScrolledList::SCROLL_BAR_WIDTH, clientRect.h);
			list->SetWindowGeometry(0, 0, verticalsScrollBarX, clientRect.h);
		}
	};


	ScrolledList::ScrolledList(Widget* parent) : Widget{parent} {
		m_listControl = new IList(this);
		m_verticalScrollBar = new ScrollBar(this, 0, SCROLL_CAPACITY - 1);
		AddNewLayout(new ScrollListLayout());
	}


	ScrolledList::~ScrolledList() {

	}


	ScrollBar* ScrolledList::GetVerticalScrollBar() {
		return m_verticalScrollBar;
	}


	IList* ScrolledList::GetIList() {
		return m_listControl;
	}

}