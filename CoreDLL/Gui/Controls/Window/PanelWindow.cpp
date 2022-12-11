#include "PanelWindow.h"

namespace Nk {

	const Color_t PanelWindow::DEFAULT_COLOR = { 0.7, 0.7, 0.7, 1.0 };


	PanelWindow::PanelWindow(Widget* parent, Coord_t x, Coord_t y, Coord_t w, Coord_t h) :
		IWindowWidget{ parent, x, y, w, h }
	{
		this->SetBackgroundColor(DEFAULT_COLOR);
	}


	PanelWindow::~PanelWindow() {
	}

}