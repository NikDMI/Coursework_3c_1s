#ifndef ICAPTION_GUI_DLL
#define ICAPTION_GUI_DLL

#include "../../Widget.h"
#include "../../../Bean/Config.h"
#include <string>
#include "../ITextElement.h"

namespace Nk {

	/*
	* This class represent control, that can capture mouse events
	*/
	CLASS_PARAMS class ICaption : public Widget, public ITextElement {
	public:
		ICaption(Widget* parent, const std::wstring& text);

		/*
		* Sets text to label
		*/
		CLASS_METHOD void SetText(const std::wstring&) noexcept;

		virtual IFont* GetElementFont() = 0;



		virtual ~ICaption() {};

	protected:
		std::wstring m_captionText;
	};

}

#endif	//#ifndef ICAPTION_GUI_DLL