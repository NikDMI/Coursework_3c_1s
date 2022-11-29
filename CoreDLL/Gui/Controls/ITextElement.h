#ifndef ITEXTELEMENT_GUI_DLL
#define ITEXTELEMENT_GUI_DLL

#include "../../Bean/Config.h"
#include "../../Painter/Font/IFont.h"

namespace Nk {

	CLASS_PARAMS class ITextElement {
	public:
		virtual ~ITextElement() {}

		/*
		* Returns font, that can be used, while drawing control
		*/
		CLASS_METHOD virtual IFont* GetElementFont() = 0;

	protected:
		
	};

}

#endif	//#ifndef ITEXTELEMENT_GUI_DLL