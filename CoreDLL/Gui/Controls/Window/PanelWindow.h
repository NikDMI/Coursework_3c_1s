#ifndef PANEL_WINDOW_WIDGET_GUI_DLL
#define PANEL_WINDOW_WIDGET_GUI_DLL

#include "IWindowWidget.h"
#include <string>

namespace Nk {

	/*
	* This class represent control, that can capture mouse events
	*/
	CLASS_PARAMS class PanelWindow : public IWindowWidget {
	public:
		CLASS_METHOD PanelWindow(Widget* parent = nullptr, Coord_t x = 0.0, Coord_t y = 0.0, Coord_t w = 300.0, Coord_t h = 300.0);

		CLASS_METHOD ~PanelWindow() override;

	private:
		static const Color_t DEFAULT_COLOR;
	};

}

#endif	//#ifndef PANEL_WINDOW_WIDGET_GUI_DLL