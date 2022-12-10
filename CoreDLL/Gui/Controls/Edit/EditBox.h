#ifndef EDIT_BOX_GUI_DLL
#define EDIT_BOX_GUI_DLL

#include "IEdit.h"
#include "../../../Painter/Font/IFont.h"
#include "../../../Painter/IPainter.h"
#include "../IBorderElement.h"

namespace Nk {

	CLASS_PARAMS class EditBox : public IEdit{
	public:

		CLASS_METHOD EditBox(const std::wstring& editText = {}, Widget* parent = nullptr);

		~EditBox() override;

	private:
		friend void PROC_CALL EditBox_OnLMouseLeave(void* params);
		friend void PROC_CALL EditBox_OnMouseMove(void* params);
		friend void PROC_CALL EditBox_OnMouseLDown(void* params);
		friend void PROC_CALL EditBox_OnLMouseLUp(void* params);
		friend void PROC_CALL EditBox_OnChar(void* params);
		friend void PROC_CALL EditBox_OnKeyDown(void* params);
		friend void Core_EditBoxDrawProc(Widget* widget, IPainter* painter);
	};

}

#endif	//#ifndef EDIT_BOX_GUI_DLL