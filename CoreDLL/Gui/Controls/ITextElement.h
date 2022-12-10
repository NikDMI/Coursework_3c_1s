#ifndef ITEXTELEMENT_GUI_DLL
#define ITEXTELEMENT_GUI_DLL

#include "../../Bean/Config.h"
#include "../Widget.h"
#include "../../Painter/Font/IFont.h"
#include <string>

namespace Nk {

	CLASS_PARAMS class ITextElement {
	public:
		ITextElement(Widget* widget, std::wstring text = L"");

		virtual ~ITextElement() {}

		/*
		* Sets text to label
		*/
		CLASS_METHOD void SetText(const std::wstring&) noexcept;

		/*
		* Get reference to the text
		*/
		CLASS_METHOD std::wstring& GetTextReference() noexcept;

		/*
		* Returns font, that can be used, while drawing control
		*/
		CLASS_METHOD IFont* GetElementFont();

	protected:
		std::wstring m_elementText;
		IFont* m_elementFont;
	};

}

#endif	//#ifndef ITEXTELEMENT_GUI_DLL