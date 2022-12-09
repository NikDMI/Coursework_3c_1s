#include "PushButton.h"
#include "../../EventStructures/MouseStructure.h"

namespace Nk {

	void Core_PushButtonDrawProc(Widget* widget, IPainter* painter);
	void PROC_CALL PushButton_OnLMouseLeave(void* params);
	void PROC_CALL PushButton_OnMouseMove(void* params);
	void PROC_CALL PushButton_OnMouseLDown(void* params);
	void PROC_CALL PushButton_OnLMouseLUp(void* params);

	const Color_t PushButton::DEFAULT_COLOR = { 0.5, 0.6, 0.3, 1.0 };;
	const Color_t PushButton::DEFAULT_HOVER_COLOR = { 0.6, 0.7, 0.3, 1.0 };;
	const Color_t PushButton::DEFAULT_PUSH_COLOR = { 0.3, 0.4, 0.6, 1.0 };;

	PushButton::PushButton(const std::wstring& buttonText, Widget* parent) : IButton{ parent }, ITextElement{ this, buttonText } {
		SetBackgroundColor(DEFAULT_COLOR);
		//Configs
		SetButtonColor(IButton::ButtonState::HOVER, DEFAULT_HOVER_COLOR);
		SetButtonColor(IButton::ButtonState::PUSH, DEFAULT_PUSH_COLOR);
		SetButtonColor(IButton::ButtonState::STATIC, DEFAULT_COLOR);
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
		if (!btn->m_isMoved) {
			btn->SetBackgroundColor(btn->GetButtonColor(IButton::ButtonState::HOVER));
			btn->m_isMoved = true;
			btn->Repaint();
		}
	}

	void PROC_CALL PushButton_OnMouseLDown(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		PushButton* btn = (PushButton*)mouseStructure->sender;
		btn->SetBackgroundColor(btn->GetButtonColor(IButton::ButtonState::PUSH));
		btn->Repaint();
	}


	void PROC_CALL PushButton_OnLMouseLUp(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		PushButton* btn = (PushButton*)mouseStructure->sender;
		btn->SetBackgroundColor(btn->GetButtonColor(IButton::ButtonState::HOVER));
		btn->Repaint();
	}


	void PROC_CALL PushButton_OnLMouseLeave(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		PushButton* btn = (PushButton*)mouseStructure->sender;
		btn->SetBackgroundColor(btn->GetButtonColor(IButton::ButtonState::STATIC));
		btn->m_isMoved = false;
		btn->Repaint();
	}

	void Core_PushButtonDrawProc(Widget* widget, IPainter* painter) {
		PushButton* btn = (PushButton*)widget;
		painter->DrawText(btn->GetWidgetClientRect(), btn->m_elementText);
	}
}