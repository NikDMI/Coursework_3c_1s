#ifndef IBORDER_GUI_DLL
#define IBORDER_GUI_DLL

#include "../Widget.h"
#include "../../Bean/Config.h"
#include "../../Tools/IGeometry/IGeometry.h"
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

		CLASS_METHOD IBrush* GetBorderBrush();

		//CLASS_METHOD void SetBorderWidth(float width);

		void SetBorderOffset(float offset);

		float GetBorderOffset();


	protected:
		/*
		* Set border geometry
		*/
		virtual void InitBorderGeometry() = 0;

		float m_width;
		BorderType m_borderType;
		IBrush* m_borderBrush;
		float m_offset = 0; //Border offset from the start of the client rect
		IGeometry* m_borderGeometry;

		void SetBorderSize();

		friend void PROC_CALL OnParentResize(void* params);
		friend void PROC_CALL BorderDrawProc(Widget* widget, IPainter* painter);
	};

}

#endif	//#ifndef IBORDER_GUI_DLL