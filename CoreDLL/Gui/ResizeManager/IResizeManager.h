#ifndef IRESIZE_GUI_DLL
#define IRESIZE_GUI_DLL

#include "../../Bean/Config.h"

namespace Nk {

	/*
	* Interface to get resize configuration
	*/
	CLASS_PARAMS class IResizeManager {
	public:
		//Types of resizing controls
		enum class ResizeType {NONE, UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT};

		/*
		* Returns resize type of widget in specified rectangle area
		*/
		CLASS_METHOD virtual ResizeType GetResizeType(Rect_t clientRect, Point_t cursorPosition) = 0;

		virtual ~IResizeManager() {};

	protected:
		IResizeManager() {};
	};
	
}

#endif	//#ifndef IRESIZE_GUI_DLL

