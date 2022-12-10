#include "IColorElement.h"
#include "../EventStructures/MouseStructure.h"

namespace Nk {

	IColorElement::IColorElement(Widget* widget) : m_widget{widget} {

	}

	void IColorElement::SetButtonColor(ElementState buttonState, Color_t color) {
		m_buttonColors[buttonState] = color;
	}


	Color_t IColorElement::GetButtonColor(ElementState buttonState) {
		return m_buttonColors[buttonState];
	}

	void PROC_CALL IColorElement::ColorElement_OnMouseMove(void* params) {
		static Widget* lastCalledWidget = nullptr;
		static IColorElement* lastColoredElement = nullptr;
		MouseStructure* mouseStructure = (MouseStructure*)params;
		if (lastCalledWidget != mouseStructure->sender) {
			lastCalledWidget = mouseStructure->sender;
			lastColoredElement = dynamic_cast<IColorElement*>(mouseStructure->sender);
		}
		Widget* btnWidget = lastColoredElement->m_widget;
		if (!lastColoredElement->m_isMoved) {
			btnWidget->SetBackgroundColor(lastColoredElement->GetButtonColor(IColorElement::ElementState::HOVER));
			lastColoredElement->m_isMoved = true;
			btnWidget->Repaint();
		}
	}

	void PROC_CALL IColorElement::ColorElement_OnMouseLDown(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		IColorElement* btn = dynamic_cast<IColorElement*>(mouseStructure->sender);
		Widget* btnWidget = btn->m_widget;
		btnWidget->SetBackgroundColor(btn->GetButtonColor(IColorElement::ElementState::PUSH));
		btnWidget->Repaint();
	}


	void PROC_CALL IColorElement::ColorElement_OnLMouseLUp(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		IColorElement* btn = dynamic_cast<IColorElement*>(mouseStructure->sender);
		Widget* btnWidget = btn->m_widget;
		btnWidget->SetBackgroundColor(btn->GetButtonColor(IColorElement::ElementState::HOVER));
		btnWidget->Repaint();
	}


	void PROC_CALL IColorElement::ColorElement_OnLMouseLeave(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		IColorElement* btn = dynamic_cast<IColorElement*>(mouseStructure->sender);
		Widget* btnWidget = btn->m_widget;
		btnWidget->SetBackgroundColor(btn->GetButtonColor(IColorElement::ElementState::STATIC));
		btn->m_isMoved = false;
		btnWidget->Repaint();
	}
}