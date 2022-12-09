#include "IButton.h"


namespace Nk {

	IButton::IButton(Widget* parent) :Widget{ parent } {

	}

	void IButton::SetButtonColor(ButtonState buttonState, Color_t color) {
		m_buttonColors[buttonState] = color;
	}


	Color_t IButton::GetButtonColor(ButtonState buttonState) {
		return m_buttonColors[buttonState];
	}
}