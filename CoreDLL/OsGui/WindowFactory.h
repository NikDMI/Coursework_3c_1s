#ifndef WINDOW_FACTORY_OS_GUI_DLL
#define WINDOW_FACTORY_OS_GUI_DLL

#include "IWindow.h"
#include "../Painter/IPainter.h"
#include "../Gui/Widget.h"

namespace Nk {

	class WindowFactory {
	public:

		/*
		* Creates phisical window in the system
		*/
		static IWindow* CreateWindow(Widget* widget, OsType osType, WindowType windowType,
			PainterType painterType, IWindow* parent);

		//������� ������� ��������� ���� ��� �������� � ����� ���������

		WindowFactory() = delete;
	};
}

#endif	//#ifndef WINDOW_FACTORY_OS_GUI_DLL

//� ������� ���� ������ ���� ���� ��������� "��������"
