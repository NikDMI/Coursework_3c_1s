#ifndef WINDOW_FACTORY_OS_GUI_DLL
#define WINDOW_FACTORY_OS_GUI_DLL

#include "IWindow.h"
#include "../Painter/IPainter.h"
#include "../Gui/Widget.h"

namespace Nk {

#undef CreateWindow

	CLASS_PARAMS class WindowFactory {
	public:

		/*
		* Creates phisical window in the system
		*/
		CLASS_METHOD static IWindow* CreateWindow(Widget* widget, WindowType windowType, IWindow* parent);

		//сделать очередь созданных окон дл€ удалени€ в конце программы

		WindowFactory() = delete;
	};
}

#endif	//#ifndef WINDOW_FACTORY_OS_GUI_DLL

//” каждого окна должен быть свой св€занный "художник"
