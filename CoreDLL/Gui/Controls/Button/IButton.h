#ifndef IBUTTON_GUI_DLL
#define IBUTTON_GUI_DLL

#include "../../Widget.h"
#include "../../../Bean/Config.h"
#include <string>
#include "../ITextElement.h"
#include "../IColorElement.h"

namespace Nk {

	CLASS_PARAMS class IButton : public Widget, public IColorElement {
	public:

		virtual ~IButton() {};

		//Use can change this config colors 
		CLASS_FIELDS static Color_t DEFAULT_COLOR;
		CLASS_FIELDS static Color_t DEFAULT_HOVER_COLOR;
		CLASS_FIELDS static Color_t DEFAULT_PUSH_COLOR;

		CLASS_METHOD void SetClickCallback(EventHandlerProc callback);

	protected:
		IButton(Widget* parent);

		//Basic button proc
		void PROC_CALL Button_OnLMouseLeave(void* params);
		void PROC_CALL Button_OnMouseMove(void* params);
		void PROC_CALL Button_OnMouseLDown(void* params);
		void PROC_CALL Button_OnLMouseLUp(void* params);

		EventHandlerProc m_userClickCallback = nullptr;

	private:
		

	};

}

#endif	//#ifndef IBUTTON_GUI_DLL