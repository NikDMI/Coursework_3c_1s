#ifndef IWINDOW_OS_GUI_DLL
#define IWINDOW_OS_GUI_DLL

#include "../Painter/IPainter.h"
#include "../Gui/Widget.h"

namespace Nk {
	/*
	* Common types of windows
	*/
	enum class WindowType {PLAIN_WINDOW, OVERLAPPED_WINDOW};

	enum class OsType { Windows, Linux };

	/*
	* This interface reperesents abstraction of phisical windows in different OS
	*/
	class IWindow {
	public:
		IWindow(Widget* widget, WindowType windowType, PainterType painterType, IWindow* parent);
		virtual ~IWindow();
		//MoveWindows
		//ShowWindow
		//....
	protected:
		WindowType m_windowType;
		PainterType m_painterType;
		//IWindow* m_parentWindow;
	};
}

#endif	//#ifndef IWINDOW_OS_GUI_DLL

//У каждого окна должен быть свой связанный "художник"
