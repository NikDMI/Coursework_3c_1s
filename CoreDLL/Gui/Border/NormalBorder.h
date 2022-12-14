#ifndef NORMAL_BORDER_GUI_DLL
#define NORMAL_BORDER_GUI_DLL

#include "IBorder.h"
#include "../../Tools/IGeometry/IGeometry.h"
#include "../../Painter/Brush/IBrush.h"

namespace Nk {

	/*
	* This class represent border abstraction
	*/
	CLASS_PARAMS class NormalBorder : public IBorder {
	public:


		CLASS_METHOD NormalBorder(Widget* parent, BorderType borderType, float borderWidth);


	private:
		void InitBorderGeometry();
	};

}

#endif	//#ifndef NORMAL_BORDER_GUI_DLL