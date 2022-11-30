#ifndef MOUSE_STRUCTURE_GUI_DLL
#define MOUSE_STRUCTURE_GUI_DLL

#include <numeric>
#include <Windows.h>
#include "../Widget.h"

namespace Nk {

	struct MouseStructure {
		int32_t xCoord_Px;
		int32_t yCoord_Px;
		Widget* sender;
	};
}

#endif	//#ifndef MOUSE_STRUCTURE_GUI_DLL
