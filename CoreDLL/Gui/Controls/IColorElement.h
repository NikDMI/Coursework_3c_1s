#ifndef ICOLORELEMENT_GUI_DLL
#define ICOLORELEMENT_GUI_DLL

#include "../../Bean/Config.h"
#include "../Widget.h"
#include "../../Painter/Font/IFont.h"
#include <string>

namespace Nk {

	/*
	* This class used to save colors in different control states
	*/
	CLASS_PARAMS class IColorElement {
	public:

		enum ElementState { STATIC, HOVER, PUSH, CHECKED };

		IColorElement(Widget* widget);

		virtual ~IColorElement() {}

		/*
		*
		*/
		CLASS_METHOD void SetButtonColor(ElementState buttonState, Color_t color);

		CLASS_METHOD Color_t GetButtonColor(ElementState buttonState);

	protected:
		Color_t m_buttonColors[(int)ElementState::CHECKED + 1];

		Widget* m_widget;
		bool m_isMoved = false;

		//Basic button proc
		void PROC_CALL ColorElement_OnLMouseLeave(void* params);
		void PROC_CALL ColorElement_OnMouseMove(void* params);
		void PROC_CALL ColorElement_OnMouseLDown(void* params);
		void PROC_CALL ColorElement_OnLMouseLUp(void* params);

	};

}

#endif	//#ifndef ICOLORELEMENT_GUI_DLL