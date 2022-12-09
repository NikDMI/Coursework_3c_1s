#ifndef IWINDOW_OS_GUI_DLL
#define IWINDOW_OS_GUI_DLL

#include "../Painter/IPainter.h"


namespace Nk {
	class Widget;

	/*
	* Common types of windows
	*/
	enum class WindowType {PLAIN_WINDOW, OVERLAPPED_WINDOW};

	//enum class OsType { Windows, Linux };

	/*
	* This interface reperesents abstraction of phisical windows in different OS
	*/
	class IWindow {
	public:


		IWindow(Widget* widget, WindowType windowType, PainterType painterType, IWindow* parent);
		virtual ~IWindow();

		virtual IPainter* GetPainter() = 0;

		/*
		* Set size and position of the window in px (1/96 inch)
		*/
		virtual void SetWindowGeometry(Coord_t x, Coord_t y, Coord_t w, Coord_t h, Point_t originPoint = {0, 0}) = 0;

		virtual void ShowWindow() = 0;
		virtual void HideWindow() = 0;
		
		/*
		* ON_REPAINT evetn
		* Draw window in special rect(). If we have saved buffer, than draw him
		* If window draw the first time, then Iwindow send ON_DRAW event to widget (fill window buffer)
		*/
		virtual void DrawWindow() = 0;

		/*
		* Creates new window buffer, wait commands on IPainter
		*/
		virtual void BeginDrawWindowBuffer() = 0;

		/*
		* Save window buffer, redraw window
		*/
		virtual void EndDrawWindowBuffer() = 0;

		/*
		* Returns offset from the most parent window
		*/
		virtual Rect_t GetRootClientRect() = 0;

		/*
		* Returns part of control that seen in parent
		*/
		virtual Rect_t GetParentClipRect(Rect_t& rootClipRect) = 0;

		/*
		* Invalidate rect
		*/
		virtual void RefreshWindow() = 0;

		virtual void SetMouseCapture() = 0;

		virtual void ReleaseMouseCapture() = 0;


	protected:
		WindowType m_windowType;
		PainterType m_painterType;
		Widget* m_correspondingWidget;
		//IWindow* m_parentWindow;
	};
}

#endif	//#ifndef IWINDOW_OS_GUI_DLL

//У каждого окна должен быть свой связанный "художник"
