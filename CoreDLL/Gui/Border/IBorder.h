#ifndef IBORDER_GUI_DLL
#define IBORDER_GUI_DLL

#include "../Widget.h"
#include "../../Bean/Config.h"
#include <string>

namespace Nk {

	/*
	* This class represent border abstraction
	*/
	CLASS_PARAMS class IBorder : public Widget {
	public:

		enum class BorderType { TOP, LEFT, BOTTOM, RIGHT };

		IBorder(Widget* parent, BorderType borderType, float borderWidth);

		float GetBorderWidth();

		BorderType GetBorderType();

		IBrush* GetBorderBrush();

	protected:

		float m_width;
		BorderType m_borderType;
		IBrush* m_borderBrush;

		void SetBorderSize();

		friend void PROC_CALL OnParentResize(void* params);
	};

}

#endif	//#ifndef IBORDER_GUI_DLL