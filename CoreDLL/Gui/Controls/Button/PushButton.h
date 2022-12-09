#ifndef PUSH_BUTTON_GUI_DLL
#define PUSH_BUTTON_GUI_DLL

#include "IButton.h"
#include "../../../Painter/Font/IFont.h"
#include "../../../Painter/IPainter.h"

namespace Nk {

	CLASS_PARAMS class PushButton : public IButton, public ITextElement {
	public:

		CLASS_METHOD PushButton(const std::wstring& buttonText = {}, Widget* parent = nullptr);

		//IFont* GetElementFont() override;

		~PushButton() override;

	private:
		static const Color_t DEFAULT_COLOR;
		static const Color_t DEFAULT_HOVER_COLOR;
		static const Color_t DEFAULT_PUSH_COLOR;

		bool m_isMoved = false;

		friend void Core_PushButtonDrawProc(Widget* widget, IPainter* painter);
		friend void PROC_CALL PushButton_OnMouseMove(void* params);
		friend void PROC_CALL PushButton_OnLMouseLeave(void* params);
	};

}

#endif	//#ifndef PUSH_BUTTON_GUI_DLL