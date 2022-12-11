#ifndef SCROLL_STRUCTURE_GUI_DLL
#define SCROLL_STRUCTURE_GUI_DLL

#include <numeric>
#include <Windows.h>
#include "../Widget.h"
#include "BasicWidgetStructure.h"

namespace Nk {

	struct ScrollStructure : public BasicWidgetStructure {
		int32_t lowerValue = 0;
		int32_t upperValue = 0;
		int32_t currentValue = 0;

		ScrollStructure() {

		}

		ScrollStructure(int32_t lowerValue ,int32_t upperValue, int32_t currentValue, Widget* sender) : lowerValue{ lowerValue },
			upperValue{ upperValue }, currentValue{ currentValue }{
			this->sender = sender;
		}
	};
}

#endif	//#ifndef SCROLL_STRUCTURE_GUI_DLL
