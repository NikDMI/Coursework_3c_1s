#include "IButton.h"
#include "../../EventStructures/MouseStructure.h"


namespace Nk {

	const Color_t IButton::DEFAULT_COLOR = { 0.5, 0.6, 0.3, 1.0 };
	const Color_t IButton::DEFAULT_HOVER_COLOR = { 0.6, 0.7, 0.3, 1.0 };
	const Color_t IButton::DEFAULT_PUSH_COLOR = { 0.3, 0.4, 0.6, 1.0 };


	IButton::IButton(Widget* parent) :Widget{ parent } {
		SetBackgroundColor(DEFAULT_COLOR);
		//Configs
		SetButtonColor(IButton::ButtonState::HOVER, DEFAULT_HOVER_COLOR);
		SetButtonColor(IButton::ButtonState::PUSH, DEFAULT_PUSH_COLOR);
		SetButtonColor(IButton::ButtonState::STATIC, DEFAULT_COLOR);
	}

	void IButton::SetButtonColor(ButtonState buttonState, Color_t color) {
		m_buttonColors[buttonState] = color;
	}


	Color_t IButton::GetButtonColor(ButtonState buttonState) {
		return m_buttonColors[buttonState];
	}


	void PROC_CALL IButton::Button_OnMouseMove(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		IButton* btn = (IButton*)mouseStructure->sender;
		if (!btn->m_isMoved) {
			btn->SetBackgroundColor(btn->GetButtonColor(IButton::ButtonState::HOVER));
			btn->m_isMoved = true;
			btn->Repaint();
		}
	}

	void PROC_CALL IButton::Button_OnMouseLDown(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		IButton* btn = (IButton*)mouseStructure->sender;
		btn->SetBackgroundColor(btn->GetButtonColor(IButton::ButtonState::PUSH));
		btn->Repaint();
	}


	void PROC_CALL IButton::Button_OnLMouseLUp(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		IButton* btn = (IButton*)mouseStructure->sender;
		btn->SetBackgroundColor(btn->GetButtonColor(IButton::ButtonState::HOVER));
		btn->Repaint();
	}


	void PROC_CALL IButton::Button_OnLMouseLeave(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		IButton* btn = (IButton*)mouseStructure->sender;
		btn->SetBackgroundColor(btn->GetButtonColor(IButton::ButtonState::STATIC));
		btn->m_isMoved = false;
		btn->Repaint();
	}
}