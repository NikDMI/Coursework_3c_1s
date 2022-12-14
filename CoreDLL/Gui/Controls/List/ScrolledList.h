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


	private:
		IList* m_listControl;
		ScrollBar* m_verticalScrollBar;
		static const int SCROLL_CAPACITY = 1000;
		static const int SCROLL_BAR_WIDTH = 20;

		friend class ScrollListLayout;
	};

}

#endif	//#ifndef SCROLLED_LIST_GUI_DLL