#ifndef IBORDERELEMENT_GUI_DLL
#define IBORDERELEMENT_GUI_DLL

#include "../../Bean/Config.h"
#include "../Widget.h"
#include "../Border/AngleBorder.h"


namespace Nk {

	/*
	* Give interface to work with widgets, that can have borders
	*/
	CLASS_PARAMS class IBorderElement {
	public:
		IBorderElement(Widget* thisWidget);

		virtual ~IBorderElement() {}

		//CLASS_METHOD void DeleteBorders();

		CLASS_METHOD void SetNormalBorder(Color_t color);

	private:
		const float DEFAULT_WIDTH = 3;
		const float DEFAULT_ANGLE = 30;

		Widget* m_widget;
		IBorder* m_topBorder = nullptr;
		IBorder* m_rightBorder = nullptr;
	};

}

#endif	//#ifndef IBORDERELEMENT_GUI_DLL