#define DLL_EXPORT_SIGNATURE

#include "IButton.h"
#include "../../EventStructures/MouseStructure.h"


namespace Nk {

	Color_t IButton::DEFAULT_COLOR = { 0.5, 0.6, 0.3, 1.0 };
	Color_t IButton::DEFAULT_HOVER_COLOR = { 0.6, 0.7, 0.3, 1.0 };
	Color_t IButton::DEFAULT_PUSH_COLOR = { 0.3, 0.4, 0.6, 1.0 };


	IButton::IButton(Widget* parent) :Widget{ parent }, IColorElement{this} {
		SetBackgroundColor(DEFAULT_COLOR);
		//Configs
		SetButtonColor(IColorElement::ElementState::HOVER, DEFAULT_HOVER_COLOR);
		SetButtonColor(IColorElement::ElementState::PUSH, DEFAULT_PUSH_COLOR);
		SetButtonColor(IColorElement::ElementState::STATIC, DEFAULT_COLOR);
	}


	void IButton::SetClickCallback(EventHandlerProc callback) {
		m_userClickCallback = callback;
	}


	void PROC_CALL IButton::Button_OnMouseMove(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		IButton* btn = (IButton*)mouseStructure->sender;
		btn->ColorElement_OnMouseMove(params);
	}

	void PROC_CALL IButton::Button_OnMouseLDown(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		IButton* btn = (IButton*)mouseStructure->sender;
		btn->ColorElement_OnMouseLDown(params);
	}


	void PROC_CALL IButton::Button_OnLMouseLUp(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		IButton* btn = (IButton*)mouseStructure->sender;
		btn->ColorElement_OnLMouseLUp(params);
		if (btn->m_userClickCallback) {
			btn->m_userClickCallback(params);
		}
	}


	void PROC_CALL IButton::Button_OnLMouseLeave(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		IButton* btn = (IButton*)mouseStructure->sender;
		btn->ColorElement_OnLMouseLeave(params);
	}
}