#ifndef LABEL_IMAGE_BUTTON_GUI_DLL
#define LABEL_IMAGE_BUTTON_GUI_DLL

#include "IButton.h"
#include "../../../Tools/Bitmap/IBitmap.h"
#include "../IBorderElement.h"
#include "../ITextElement.h"
#include <string>

namespace Nk {

	CLASS_PARAMS class LabelImageButton : public IButton, public IBorderElement, public ITextElement {
	public:

		CLASS_METHOD LabelImageButton(IBitmap* image, std::wstring buttonText, Widget* parent = nullptr);

		CLASS_METHOD ~LabelImageButton() override;

	private:
		IBitmap* m_bitmap;

		static const Coord_t LABEL_MAX_OFFSET;

		friend void PROC_CALL LabelImageButton_OnLMouseLeave(void* params);
		friend void PROC_CALL LabelImageButton_OnMouseMove(void* params);
		friend void PROC_CALL LabelImageButton_OnMouseLDown(void* params);
		friend void PROC_CALL LabelImageButton_OnLMouseLUp(void* params);
		friend void Core_LabelImageButtonDrawProc(Widget* widget, IPainter* painter);
	};

}

#endif	//#ifndef LABEL_IMAGE_BUTTON_GUI_DLL