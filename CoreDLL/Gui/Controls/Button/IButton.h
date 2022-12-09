#ifndef IBUTTON_GUI_DLL
#define IBUTTON_GUI_DLL

#include "../../Widget.h"
#include "../../../Bean/Config.h"
#include <string>
#include "../ITextElement.h"

namespace Nk {

	CLASS_PARAMS class IButton : public Widget {
	public:
		enum ButtonState {STATIC, HOVER, PUSH, CHECKED};

		/*
		* 
		*/
		CLASS_METHOD void SetButtonColor(ButtonState buttonState, Color_t color);
		CLASS_METHOD Color_t GetButtonColor(ButtonState buttonState);

		virtual ~IButton() {};

	protected:
		IButton(Widget* parent);

	private:
		Color_t m_buttonColors[(int)ButtonState::CHECKED + 1];
	};

}

#endif	//#ifndef IBUTTON_GUI_DLL