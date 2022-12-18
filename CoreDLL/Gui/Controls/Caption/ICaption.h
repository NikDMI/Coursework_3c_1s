#ifndef ICAPTION_GUI_DLL
#define ICAPTION_GUI_DLL

#include "../../Widget.h"
#include "../../../Bean/Config.h"
#include <string>
#include "../ITextElement.h"

namespace Nk {

	/*
	* This class represent control, that can capture mouse events
	*/
	CLASS_PARAMS class ICaption : public Widget, public ITextElement{
	public:
		CLASS_METHOD ICaption(Widget* parent, const std::wstring& text);

		CLASS_METHOD virtual ~ICaption() {};

	private:
		Point_t m_lastCursorPosition;
		bool m_isCaptured = false;

		friend void PROC_CALL OnMouseDown(void* params);
		friend void PROC_CALL OnMouseMove(void* params);
		friend void PROC_CALL OnMouseUp(void* params);

	};

}

#endif	//#ifndef ICAPTION_GUI_DLL