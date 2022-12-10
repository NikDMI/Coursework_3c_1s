#ifndef MAIN_CAPTION_GUI_DLL
#define MAIN_CAPTION_GUI_DLL

#include "ICaption.h"
#include "../Button/IButton.h"

namespace Nk {

	/*
	* This class represent control, that can capture mouse events
	*/
	CLASS_PARAMS class MainCaption : public ICaption {
	public:
		CLASS_METHOD MainCaption(Widget* parent, const std::wstring& caption);

		~MainCaption() override {};

	private:
		static const int DEFAULT_CAPTION_HEIGHT = 30;
		static const int DEFAULT_CAPTION_WIDTH = 200;
		static const std::wstring DEFAULT_CAPTION_FONT_FAMILY;
		static const int DEFAULT_CAPTION_FONT_WIDTH = 22;
		static const Color_t DEFAULT_CAPTION_COLOR;
		static const Color_t HOVER_BUTTON_COLOR;
		static const Color_t PUSH_BUTTON_COLOR;
		static const int DEFAULT_BUTTON_WIDTH = 30;


		float m_captionHeight = DEFAULT_CAPTION_HEIGHT;

		Coord_t m_xTextOffset = 20;
		Coord_t m_yTextOffset = 10;
		Widget* m_parentWidget;

		IButton* m_closeButton;

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