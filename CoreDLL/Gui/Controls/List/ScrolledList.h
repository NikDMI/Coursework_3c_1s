#ifndef SCROLLED_LIST_GUI_DLL
#define SCROLLED_LIST_GUI_DLL

#include "IList.h"
#include "../Slider/ScrollBar.h"

namespace Nk {

	CLASS_PARAMS class ScrolledList: public Widget{
	public:

		CLASS_METHOD ScrolledList(Widget* parent);

		CLASS_METHOD ~ScrolledList();

		CLASS_METHOD ScrollBar* GetVerticalScrollBar();

		CLASS_METHOD IList* GetIList();

		CLASS_METHOD void SetWindowGeometry(Coord_t x, Coord_t y, Coord_t w, Coord_t h) override;



	private:
		IList* m_listControl;
		ScrollBar* m_verticalScrollBar;
		ILayout* m_currentLayout;

		static const int SCROLL_CAPACITY = 1000;
		static const int SCROLL_BAR_WIDTH = 20;

		friend class ScrollListLayout;
	};

}

#endif	//#ifndef SCROLLED_LIST_GUI_DLL