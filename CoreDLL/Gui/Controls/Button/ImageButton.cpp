#include "ImageButton.h"
#include "../../EventStructures/MouseStructure.h"

namespace Nk {

	void Core_ImageButtonDrawProc(Widget* widget, IPainter* painter);

	void PROC_CALL ImageButton_OnLMouseLeave(void* params);
	void PROC_CALL ImageButton_OnMouseMove(void* params);
	void PROC_CALL ImageButton_OnMouseLDown(void* params);
	void PROC_CALL ImageButton_OnLMouseLUp(void* params);

	ImageButton::ImageButton(IBitmap* image, Widget* parent) : IButton{ parent },
		IBorderElement{ this }, m_bitmap{image}
	{
		this->SetWindowDrawProc(Core_ImageButtonDrawProc);
		//Events
		SetCustomEvent(CustomEvents::ON_MOUSE_MOVE, ImageButton_OnMouseMove);
		SetCustomEvent(CustomEvents::ON_MOUSE_LDOWN, ImageButton_OnMouseLDown);
		SetCustomEvent(CustomEvents::ON_MOUSE_LUP, ImageButton_OnLMouseLUp);
		SetCustomEvent(CustomEvents::ON_MOUSE_LEAVE, ImageButton_OnLMouseLeave);
	}

	ImageButton::~ImageButton() {

	}


	void PROC_CALL ImageButton_OnMouseMove(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		ImageButton* btn = (ImageButton*)mouseStructure->sender;
		btn->Button_OnMouseMove(params);
	}

	void PROC_CALL ImageButton_OnMouseLDown(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		ImageButton* btn = (ImageButton*)mouseStructure->sender;
		btn->Button_OnMouseLDown(params);
	}


	void PROC_CALL ImageButton_OnLMouseLUp(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		ImageButton* btn = (ImageButton*)mouseStructure->sender;
		btn->Button_OnLMouseLUp(params);
	}


	void PROC_CALL ImageButton_OnLMouseLeave(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		ImageButton* btn = (ImageButton*)mouseStructure->sender;
		btn->Button_OnLMouseLeave(params);
	}

	void Core_ImageButtonDrawProc(Widget* widget, IPainter* painter) {
		ImageButton* btn = (ImageButton*)widget;
		if (btn->m_bitmap) {
			auto clientRect = widget->GetWidgetClientRect();
			float dx = clientRect.w * 0.05;
			float dy = clientRect.h * 0.05;
			clientRect.h -= dy * 2;
			clientRect.w -= dx * 2;
			clientRect.x = dx;
			clientRect.y = dy;
			painter->DrawBitmapCentric(btn->m_bitmap, clientRect);
		}
	}
}