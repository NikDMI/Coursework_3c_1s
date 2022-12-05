#ifndef MAIN_CAPTION_GUI_DLL
#define MAIN_CAPTION_GUI_DLL

#include "ICaption.h"

namespace Nk {

	/*
	* This class represent control, that can capture mouse events
	*/
	CLASS_PARAMS class MainCaption : public ICaption {
	public:
		CLASS_METHOD MainCaption(Widget* parent, const std::wstring& caption);

		~MainCaption() override {};

	private:
		static const int DEFAULT_CAPTION_HEIGHT = 25;
		static const int DEFAULT_CAPTION_WIDTH = 200;

		float m_captionHeight = DEFAULT_CAPTION_HEIGHT;

		Coord_t m_xTextOffset = 20;
		Coord_t m_yTextOffset = 10;
		Widget* m_parentWidget;

		Point_t m_lastCursorPosition;
		bool m_isCaptured = false;

		friend void PROC_CALL CaptionDrawProc(Widget* widget, IPainter* painter);
		friend void PROC_CALL OnMouseDown(void* params);
		friend void PROC_CALL OnMouseMove(void* params);
		friend void PROC_CALL OnMouseUp(void* params);
		friend void PROC_CALL MainCaption_OnParentResize(void* params);
	};

}

#endif	//#ifndef MAIN_CAPTION_GUI_DLL