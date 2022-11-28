#ifndef IFONT_PAINTER_DLL
#define IFONT_PAINTER_DLL

#include <string>

namespace Nk {

	class IFont {
	public:

		enum class HorizontalAlignment { LEADING, TRAILING, CENTER };
		enum class VerticalAlignment { TOP, BOTTOM, CENTER };

		virtual ~IFont() {};

		virtual void SetSizeInPt(int sizeInPt) = 0;
		virtual void SetSizeInPixels(int sizeInPixels) = 0;
		virtual void SetFamily(std::wstring familyName) = 0;
		virtual void SetHorizontalAlignment(HorizontalAlignment alignment) = 0;
		virtual void SetVerticalAlignment(VerticalAlignment alignment) = 0;
		

		//virtual FLOAT GetTextMaxHeight(const std::wstring& text, FLOAT maxWidth) = 0;
	};

}

#endif	//#ifndef IFONT_PAINTER_DLL

//У каждого окна должен быть свой связанный "художник"
