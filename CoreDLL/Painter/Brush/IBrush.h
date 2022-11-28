#ifndef IBRUSH_PAINTER_DLL
#define IBRUSH_PAINTER_DLL

#include "../../Bean/Config.h"

namespace Nk {

	class IBrush {
	public:
		virtual ~IBrush() {};

		/*
		* Set current brush color
		*/
		virtual void SetColor(Color_t color) = 0;

		//virtual void SetBitmapPattert()

	};

}

#endif	//#ifndef IBRUSH_PAINTER_DLL

