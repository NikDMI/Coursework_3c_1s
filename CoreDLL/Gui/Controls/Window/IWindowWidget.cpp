#include "IWindowWidget.h"

namespace Nk {

	IWindowWidget::IWindowWidget(Widget* parent, Coord_t x, Coord_t y, Coord_t w, Coord_t h) : Widget{parent} {
		SetWindowGeometry(x, y, w, h);
	}

}