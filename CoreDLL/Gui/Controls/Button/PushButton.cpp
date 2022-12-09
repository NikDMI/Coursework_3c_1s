#include "PushButton.h"
#include "../../EventStructures/MouseStructure.h"

namespace Nk {

	void Core_PushButtonDrawProc(Widget* widget, IPainter* painter);

	void PROC_CALL PushButton_OnLMouseLeave(void* params);
	void PROC_CALL PushButton_OnMouseMove(void* params);
	void PROC_CALL PushButton_OnMouseLDown(void* params);
	void PROC_CALL PushButton_OnLMouseLUp(void* params);

	PushButton::PushButton(const std::wstring& buttonText, Widget* parent) : IButton{ parent }, ITextElement{ this, buttonText },
		IBorderElement{this}
	{
		m_elementFont->SetHorizontalAlignment(IFont::HorizontalAlignment::CENTER);
		m_elementFont->SetVerticalAlignment(IFont::VerticalAlignment::CENTER);
		this->SetWindowDrawProc(Core_PushButtonDrawProc);
		//Events
		SetCustomEvent(CustomEvents::ON_MOUSE_MOVE, PushButton_OnMouseMove);
		SetCustomEvent(CustomEvents::ON_MOUSE_LDOWN, PushButton_OnMouseLDown);
		SetCustomEvent(CustomEvents::ON_MOUSE_LUP, PushButton_OnLMouseLUp);
		SetCustomEvent(CustomEvents::ON_MOUSE_LEAVE, PushButton_OnLMouseLeave);
	}

	PushButton::~PushButton() {
		
	}


	void PROC_CALL PushButton_OnMouseMove(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		PushButton* btn = (PushButton*)mouseStructure->sender;
		btn->Button_OnMouseMove(params);
	}

	void PROC_CALL PushButton_OnMouseLDown(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		PushButton* btn = (PushButton*)mouseStructure->sender;
		btn->Button_OnMouseLDown(params);
	}


	void PROC_CALL PushButton_OnLMouseLUp(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		PushButton* btn = (PushButton*)mouseStructure->sender;
		btn->Button_OnLMouseLUp(params);
	}


	void PROC_CALL PushButton_OnLMouseLeave(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		PushButton* btn = (PushButton*)mouseStructure->sender;
		btn->Button_OnLMouseLeave(params);
	}

	void Core_PushButtonDrawProc(Widget* widget, IPainter* painter) {
		PushButton* btn = (PushButton*)widget;
		painter->DrawText(btn->GetWidgetClientRect(), btn->m_elementText);
	}
}