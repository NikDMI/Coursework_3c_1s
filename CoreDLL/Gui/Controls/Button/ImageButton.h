#ifndef IMAGE_BUTTON_GUI_DLL
#define IMAGE_BUTTON_GUI_DLL

#include "IButton.h"
#include "../../../Tools/Bitmap/IBitmap.h"
#include "../IBorderElement.h"

namespace Nk {

	CLASS_PARAMS class ImageButton : public IButton, public IBorderElement {
	public:

		CLASS_METHOD ImageButton(IBitmap* image, Widget* parent = nullptr);

		~ImageButton() override;

	private:
		IBitmap* m_bitmap;

		friend void PROC_CALL ImageButton_OnLMouseLeave(void* params);
		friend void PROC_CALL ImageButton_OnMouseMove(void* params);
		friend void PROC_CALL ImageButton_OnMouseLDown(void* params);
		friend void PROC_CALL ImageButton_OnLMouseLUp(void* params);
		friend void Core_ImageButtonDrawProc(Widget* widget, IPainter* painter);

		//Scroll bar
		friend void PROC_CALL OnMouseDownScrollElement(void* params);
		friend void PROC_CALL OnMouseUpScrollElement(void* params);
		friend void PROC_CALL OnMouseMoveScrollElement(void* params);
	};

}

#endif	//#ifndef IMAGE_BUTTON_GUI_DLL