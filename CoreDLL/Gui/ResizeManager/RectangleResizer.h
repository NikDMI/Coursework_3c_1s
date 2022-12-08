#ifndef RECTANGLE_RESIZE_GUI_DLL
#define RECTANGLE_RESIZE_GUI_DLL

#include "IResizeManager.h"

namespace Nk {

	/*
	* Interface to get resize configuration
	*/
	CLASS_PARAMS class RectangleResizer : public IResizeManager {
	public:

		/*
		* Get size of resize rectangle
		*/
		CLASS_METHOD RectangleResizer(Coord_t dLeft = 15, Coord_t dRight = 15, Coord_t dUp = 15, Coord_t dDown = 15);

		/*
		* Returns resize type of widget in specified rectangle area
		*/
		ResizeType GetResizeType(Rect_t clientRect, Point_t cursorPosition) override;

	private:
		Coord_t m_dLeft;
		Coord_t m_dRight;
		Coord_t m_dUp;
		Coord_t m_dDown;
	};

}

#endif	//#ifndef RECTANGLE_RESIZE_GUI_DLL

