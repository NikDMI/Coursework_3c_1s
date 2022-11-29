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

		/*
		* Sets text to label
		*/
		CLASS_METHOD void SetText(const std::wstring&) noexcept;


		virtual ~ILabel() {};

	protected:
		std::wstring m_labelText;
	};
	
}

#endif	//#ifndef ILABLE_GUI_DLL