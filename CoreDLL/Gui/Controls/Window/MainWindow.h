#ifndef MAIN_WINDOW_WIDGET_GUI_DLL
#define MAIN_WINDOW_WIDGET_GUI_DLL

#include "IWindowWidget.h"
#include "../Caption/MainCaption.h"
#include <string>

namespace Nk {

	/*
	* This class represent control, that can capture mouse events
	*/
	CLASS_PARAMS class MainWindow : public IWindowWidget {
	public:
		CLASS_METHOD MainWindow(std::wstring windowName, Coord_t x = 0.0, Coord_t y = 0.0, Coord_t w = 300.0, Coord_t h = 300.0);

		CLASS_METHOD ~MainWindow() override;

		CLASS_METHOD MainCaption* GetWindowCaption();


	private:
		static const Color_t DEFAULT_COLOR;

		MainCaption* m_caption;

	};

}

#endif	//#ifndef MAIN_WINDOW_WIDGET_GUI_DLL