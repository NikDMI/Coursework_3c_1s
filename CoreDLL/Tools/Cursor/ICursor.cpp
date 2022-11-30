#include "ICursor.h"
#include <Windows.h>

namespace Nk {

	Point_t ICursor::GetGlobalMouseCoord() {
		//Now only for win32
		POINT p;
		GetCursorPos(&p);
		return { (float)p.x, (float)p.y };
	}
}