#ifndef ILABLE_GUI_DLL
#define ILABLE_GUI_DLL

#include "../../Widget.h"
#include "../../../Bean/Config.h"
#include <string>
#include "../ITextElement.h"

namespace Nk {

	CLASS_PARAMS class ILabel : public Widget, public ITextElement {
	public:
		ILabel(Widget* parent);

		virtual ~ILabel() {};

	protected:
	};
	
}

#endif	//#ifndef ILABLE_GUI_DLL