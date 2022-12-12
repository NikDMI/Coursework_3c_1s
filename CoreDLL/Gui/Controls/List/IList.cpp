#include "IList.h"
#include "../../../Bean/Exception.h"

namespace Nk {

	void PROC_CALL IListDrawProc(Widget* widget, IPainter* painter);

	IList::IList(Widget* parent) : Widget{parent} {
		this->SetWindowDrawProc(IListDrawProc);
	}


	IList::~IList() {

	}


	/*
		* Add new list item to the end of the list
		*/
	void IList::AddListItem(Widget* widget) {
		if (widget->GetParentWidget() != this) {
			throw Exception{"Invalid parent"};
		}
		ListItemInfo listItem{ widget, widget->GetWidgetRect().h };
		if (m_listElements.size()) {
			ListItemInfo& lastItem = m_listElements.back();
			listItem.aboveElementsTotalHeight = lastItem.aboveElementsTotalHeight + lastItem.height;
		}
		m_listElements.push_back(listItem);
		//ComputeListItemsPositions();
	}


	/*
	* Removes list item if it's exists
	*/
	void IList::RemoveListItem(Widget* widget) {

	}


	/*
	* Sets scrolling space
	*/
	void IList::SetScrolling(float scrollState) {
		if (scrollState < 0) {
			m_listScrolling = 0;
		}
		else if (scrollState > 1) {
			m_listScrolling = 1;
		}
		else {
			m_listScrolling = scrollState;
		}
		ComputeListItemsPositions();
	}


	/*
	* should be called, when user change height of list elements
	*/
	void IList::RecomputeListLayout() {
		float totalHeight = 0;
		for (ListItemInfo& listItem : m_listElements) {
			listItem.aboveElementsTotalHeight = totalHeight;
			listItem.height = listItem.widget->GetWidgetRect().h;
			totalHeight += listItem.height;
		}
		ComputeListItemsPositions();
	}


	void IList::ComputeListItemsPositions() {
		if (!m_listElements.size()) return;
		ListItemInfo& lastItem = m_listElements.back();
		Rect_t listRect = GetWidgetClientRect();
		Coord_t freeSpace = (lastItem.aboveElementsTotalHeight + lastItem.height) - listRect.h;
		std::list<ListItemInfo>::const_iterator startElement = m_listElements.cbegin(), endElement = m_listElements.cend();
		Coord_t currentYCoord = 0;
		if (freeSpace > 0 && m_listScrolling > 0) {
			Coord_t scrollHeight = m_listScrolling * freeSpace; //How much space should ignore
			while (true) {
				scrollHeight -= (*startElement).height;
				if (scrollHeight > 0) {//SkipElement
					//(*startElement).widget->HideWindow();
					(*startElement).widget->OffsetWindow(-20000, -20000);	//Push item in infinity to prevent blicks
				}
				else {
					currentYCoord = -((*startElement).height + scrollHeight);
					break;
				}
				startElement++;
			}
		}
		Coord_t listArea = listRect.h;
		while (startElement != endElement && currentYCoord < listArea) {
			const ListItemInfo& currentElement = *startElement;
			currentElement.widget->SetWindowGeometry(0, currentYCoord, listRect.w, currentElement.height);
			currentElement.widget->ShowWindow();
			currentYCoord += currentElement.height;
			startElement++;
		}
		//Hide next widgets
		while (startElement != endElement) {
			//(*startElement).widget->HideWindow();
			(*startElement).widget->OffsetWindow(-20000, -20000);	//Push item in infinity to prevent blicks
			startElement++;
		}
	}


	void PROC_CALL IListDrawProc(Widget* widget, IPainter* painter) {
		IList* list = (IList*)widget;
	}
}