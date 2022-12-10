#ifndef IEDIT_GUI_DLL
#define IEDIT_GUI_DLL

#include "../../Widget.h"
#include "../../../Bean/Config.h"
#include <string>
#include "../ITextElement.h"
#include "../IBorderElement.h"
#include "../IColorElement.h"

namespace Nk {

	CLASS_PARAMS class IEdit : public Widget, public ITextElement, public IBorderElement, public IColorElement {
	public:

		virtual ~IEdit() {};

	protected:
		IEdit(Widget* parent);

		//Basic button proc
		void PROC_CALL Edit_OnLMouseLeave(void* params);
		void PROC_CALL Edit_OnMouseMove(void* params);
		void PROC_CALL Edit_OnMouseLDown(void* params);
		void PROC_CALL Edit_OnLMouseLUp(void* params);

	private:
		static const Color_t DEFAULT_COLOR;
		static const Color_t DEFAULT_HOVER_COLOR;
		static const Color_t DEFAULT_PUSH_COLOR;
	};

}

#endif	//#ifndef IEDIT_GUI_DLL