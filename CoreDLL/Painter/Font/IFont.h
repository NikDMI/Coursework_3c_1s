#ifndef IFONT_PAINTER_DLL
#define IFONT_PAINTER_DLL

#include <string>
#include "../../Bean/Config.h"

namespace Nk {

	class IFont {
	public:

		enum class HorizontalAlignment { LEADING, TRAILING, CENTER };
		enum class VerticalAlignment { TOP, BOTTOM, CENTER };

		virtual ~IFont() {};

		/*
		* <return>Themself to use pattern Builder</return>
		*/
		virtual IFont* SetSizeInPt(int sizeInPt) = 0;
		virtual IFont* SetSizeInPixels(int sizeInPixels) = 0;
		virtual IFont* SetFamily(std::wstring familyName) = 0;
		virtual IFont* SetHorizontalAlignment(HorizontalAlignment alignment) = 0;
		virtual IFont* SetVerticalAlignment(VerticalAlignment alignment) = 0;
		virtual IFont* SetMultilineState(bool isMultiline) = 0;
		

		//virtual Rect_t GetTextMetrix(const std::wstring& text, FLOAT maxWidth) = 0;
	};

}

#endif	//#ifndef IFONT_PAINTER_DLL

//У каждого окна должен быть свой связанный "художник"
