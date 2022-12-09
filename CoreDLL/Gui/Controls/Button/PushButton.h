#ifndef PUSH_BUTTON_GUI_DLL
#define PUSH_BUTTON_GUI_DLL

#include "IButton.h"
#include "../../../Painter/Font/IFont.h"
#include "../../../Painter/IPainter.h"
#include "../IBorderElement.h"

namespace Nk {

	CLASS_PARAMS class PushButton : public IButton, public ITextElement, public IBorderElement {
	public:

		CLASS_METHOD PushButton(const std::wstring& buttonText = {}, Widget* parent = nullptr);

		~PushButton() override;

	private:
		friend void PROC_CALL PushButton_OnLMouseLeave(void* params);
		friend void PROC_CALL PushButton_OnMouseMove(void* params);
		friend void PROC_CALL PushButton_OnMouseLDown(void* params);
		friend void PROC_CALL PushButton_OnLMouseLUp(void* params);
		friend void Core_PushButtonDrawProc(Widget* widget, IPainter* painter);
	};

}

#endif	//#ifndef PUSH_BUTTON_GUI_DLL