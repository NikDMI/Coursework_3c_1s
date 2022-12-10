#ifndef KEYBOARD_STRUCTURE_GUI_DLL
#define KEYBOARD_STRUCTURE_GUI_DLL

#include <numeric>
#include <Windows.h>
#include "../Widget.h"
#include "BasicWidgetStructure.h"

namespace Nk {

	struct KeyboardStructure : public BasicWidgetStructure {
		enum class SystemKey {NONE_KEY, BACK_KEY, ENTER_KEY, SYSTEM_KEY};

		wchar_t charValue;
		SystemKey systemKey;

		KeyboardStructure() {

		}

		KeyboardStructure(wchar_t ch, SystemKey systemKey, Widget* sender) : charValue{ ch }, systemKey{systemKey} {
			this->sender = sender;
		}
	};
}

#endif	//#ifndef KEYBOARD_STRUCTURE_GUI_DLL
