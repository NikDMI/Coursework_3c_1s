#include "LabelImageButton.h"
#include "../../EventStructures/MouseStructure.h"

namespace Nk {

	const Coord_t LabelImageButton::LABEL_MAX_OFFSET = 20;


	void Core_LabelImageButtonDrawProc(Widget* widget, IPainter* painter);

	void PROC_CALL LabelImageButton_OnLMouseLeave(void* params);
	void PROC_CALL LabelImageButton_OnMouseMove(void* params);
	void PROC_CALL LabelImageButton_OnMouseLDown(void* params);
	void PROC_CALL LabelImageButton_OnLMouseLUp(void* params);

	LabelImageButton::LabelImageButton(IBitmap* image, std::wstring buttonText, Widget* parent) : IButton{ parent },
		IBorderElement{ this }, ITextElement{ this, buttonText }, m_bitmap{image}
	{
		this->SetWindowDrawProc(Core_LabelImageButtonDrawProc);
		//Events
		SetCustomEvent(CustomEvents::ON_MOUSE_MOVE, LabelImageButton_OnMouseMove);
		SetCustomEvent(CustomEvents::ON_MOUSE_LDOWN, LabelImageButton_OnMouseLDown);
		SetCustomEvent(CustomEvents::ON_MOUSE_LUP, LabelImageButton_OnLMouseLUp);
		SetCustomEvent(CustomEvents::ON_MOUSE_LEAVE, LabelImageButton_OnLMouseLeave);
	}

	LabelImageButton::~LabelImageButton() {

	}


	void PROC_CALL LabelImageButton_OnMouseMove(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		LabelImageButton* btn = (LabelImageButton*)mouseStructure->sender;
		btn->Button_OnMouseMove(params);
	}

	void PROC_CALL LabelImageButton_OnMouseLDown(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		LabelImageButton* btn = (LabelImageButton*)mouseStructure->sender;
		btn->Button_OnMouseLDown(params);
	}


	void PROC_CALL LabelImageButton_OnLMouseLUp(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		LabelImageButton* btn = (LabelImageButton*)mouseStructure->sender;
		btn->Button_OnLMouseLUp(params);
	}


	void PROC_CALL LabelImageButton_OnLMouseLeave(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		LabelImageButton* btn = (LabelImageButton*)mouseStructure->sender;
		btn->Button_OnLMouseLeave(params);
	}

	void Core_LabelImageButtonDrawProc(Widget* widget, IPainter* painter) {
		LabelImageButton* btn = (LabelImageButton*)widget;
		auto clientRect = widget->GetWidgetClientRect();
		Rect_t bmpRect = { 0, 0, 0, 0 };
		if (btn->m_bitmap) {
			Coord_t dx = 0.05 * clientRect.w;
			if (dx > LabelImageButton::LABEL_MAX_OFFSET) {
				dx = LabelImageButton::LABEL_MAX_OFFSET;
			}
			if (clientRect.w > clientRect.h) {
				bmpRect.h = clientRect.h * 0.8;
				bmpRect.w = bmpRect.h;
				bmpRect.y = clientRect.h * 0.1;
			}
			else {
				bmpRect.w = 0.2 * clientRect.w;
				bmpRect.h = bmpRect.w;
				bmpRect.y = (clientRect.h - bmpRect.h) / 2;
			}
			painter->DrawBitmapCentric(btn->m_bitmap, bmpRect);
		}
		Coord_t labelX = bmpRect.x + bmpRect.w + 5;
		Rect_t labelRect = { labelX , 0, clientRect.w - labelX, clientRect.h };
		if (labelRect.w > 0 && labelRect.h > 0) {
			painter->DrawText(labelRect, btn->GetTextReference());
		}
	}
}