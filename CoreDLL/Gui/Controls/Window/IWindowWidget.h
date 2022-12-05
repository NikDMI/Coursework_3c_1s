#ifndef IWINDOW_WIDGET_GUI_DLL
#define IWINDOW_WIDGET_GUI_DLL

#include "../../Widget.h"
#include "../../../Bean/Config.h"
#include <string>

namespace Nk {

	/*
	* This class represent control, that can capture mouse events
	*/
	CLASS_PARAMS class IWindowWidget : public Widget {
	public:
		IWindowWidget(Widget* parent, Coord_t x = 0.0, Coord_t y = 0.0, Coord_t w = 300.0, Coord_t h = 300.0);

		virtual ~IWindowWidget() {};

	};

}

#endif	//#ifndef IWINDOW_WIDGET_GUI_DLL