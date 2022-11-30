#ifndef ICURSOR_TOOL_H_DLL
#define ICURSOR_TOOL_H_DLL

#include "../../Bean/Config.h"

namespace Nk {

	CLASS_PARAMS class ICursor {
	public:
		CLASS_METHOD static Point_t GetGlobalMouseCoord();
	};
}

#endif	//#ifndef ICURSOR_TOOL_H_DLL
