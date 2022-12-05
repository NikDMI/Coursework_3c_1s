#ifndef MOUSE_STRUCTURE_GUI_DLL
#define MOUSE_STRUCTURE_GUI_DLL

#include <numeric>
#include <Windows.h>
#include "../Widget.h"
#include "BasicWidgetStructure.h"

namespace Nk {

	struct MouseStructure: public BasicWidgetStructure {
		int32_t xCoord_Px = 0;
		int32_t yCoord_Px = 0;

		MouseStructure() {

		}

		MouseStructure(int32_t x, int32_t y, Widget* sender) : xCoord_Px{ x }, yCoord_Px{ y }{
			this->sender = sender;
		}
	};
}

#endif	//#ifndef MOUSE_STRUCTURE_GUI_DLL
