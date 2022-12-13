#ifndef RESIZE_STRUCTURE_GUI_DLL
#define RESIZE_STRUCTURE_GUI_DLL

#include <numeric>
#include <Windows.h>
#include "../Widget.h"
#include "BasicWidgetStructure.h"

namespace Nk {

	struct ResizeWindowStructure : public BasicWidgetStructure {
		int32_t xCoord_Px = 0;
		int32_t yCoord_Px = 0;
		int32_t wCoord_Px = 0;
		int32_t hCoord_Px = 0;

		ResizeWindowStructure() {

		}

		ResizeWindowStructure(int32_t x, int32_t y, int32_t w, int32_t h, Widget* sender) : xCoord_Px{ x }, yCoord_Px{ y }, wCoord_Px{ w }, hCoord_Px{ h }{
			this->sender = sender;
		}
	};
}

#endif	//#ifndef RESIZE_STRUCTURE_GUI_DLL
