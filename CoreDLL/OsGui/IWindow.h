#ifndef IWINDOW_OS_GUI_DLL
#define IWINDOW_OS_GUI_DLL

#include "../Painter/IPainter.h"

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
		IWindow(OsType osType, WindowType windowType, PainterType painterType);
		virtual ~IWindow();
		//MoveWindows
		//ShowWindow
		//....
	protected:
		OsType m_osType;
		WindowType m_windowType;
		PainterType m_painterType;
	};
}

#endif	//#ifndef IWINDOW_OS_GUI_DLL

//У каждого окна должен быть свой связанный "художник"
