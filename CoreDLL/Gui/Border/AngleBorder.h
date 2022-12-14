#ifndef ANGLE_BORDER_GUI_DLL
#define ANGLE_BORDER_GUI_DLL

#include "IBorder.h"
#include "../../Tools/IGeometry/IGeometry.h"
#include "../../Painter/Brush/IBrush.h"

namespace Nk {

	/*
	* This class represent border abstraction
	*/
	CLASS_PARAMS class AngleBorder : public IBorder {
	public:


		CLASS_METHOD AngleBorder(Widget* parent, BorderType borderType, float borderWidth, float angleOxInDeegre);

		//днаюбхрэ янашрхе мю хглемемхе пюглепнб пдхрекэяйнцн нймю, врнаш оепеявхрюрэ йннмдхмюрш ценлерпхх
	protected:
		void InitBorderGeometry() override;

		float m_andleOxRadians;

		friend void PROC_CALL AngleDrawProc(Widget* widget, IPainter* painter);
	};

}

#endif	//#ifndef ANGLE_BORDER_GUI_DLL