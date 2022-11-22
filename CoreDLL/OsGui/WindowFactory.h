#ifndef WINDOW_FACTORY_OS_GUI_DLL
#define WINDOW_FACTORY_OS_GUI_DLL

#include "IWindow.h"
#include "../Painter/IPainter.h"

namespace Nk {

	class WindowFactory {
	public:

		static IWindow* CreateWindow(OsType osType, WindowType windowType, PainterType painterType, IWindow* parent);

		WindowFactory() = delete;
	};
}

#endif	//#ifndef WINDOW_FACTORY_OS_GUI_DLL

//У каждого окна должен быть свой связанный "художник"
