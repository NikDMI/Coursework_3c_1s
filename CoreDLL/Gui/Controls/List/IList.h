#ifndef ILIST_GUI_DLL
#define ILIST_GUI_DLL

#include "../../Widget.h"
#include "../../../Bean/Config.h"
#include <list>

namespace Nk {

	CLASS_PARAMS class IList : public Widget {
	public:
		CLASS_METHOD IList(Widget* parent);

		CLASS_METHOD virtual ~IList();

		/*
		* Add new list item to the end of the list
		*/
		CLASS_METHOD void AddListItem(Widget* widget);


		/*
		* Removes list item if it's exists
		*/
		CLASS_METHOD void RemoveListItem(Widget* widget);


		/*
		* Sets scrolling space
		*/
		CLASS_METHOD void SetScrolling(float scrollState);


		/*
		* should be called, when user change height of list elements
		*/
		CLASS_METHOD void RecomputeListLayout();


		//Use can change this config colors 
		CLASS_FIELDS static Color_t DEFAULT_COLOR;

	private:

		void ComputeListItemsPositions();

		struct ListItemInfo {
			Widget* widget;
			Coord_t height = 0;
			Coord_t aboveElementsTotalHeight = 0;
		};

		std::list<ListItemInfo> m_listElements;
		float m_listScrolling = 0.0f; // 0-1 how much space should be scrolled

		friend void PROC_CALL IListDrawProc(Widget* widget, IPainter* painter);

	};

}

#endif	//#ifndef ILIST_GUI_DLL