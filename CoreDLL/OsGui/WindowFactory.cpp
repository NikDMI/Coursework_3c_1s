#define DLL_EXPORT_SIGNATURE

#include "WindowFactory.h"
#include "Windows/WindowWin32.h"

namespace Nk {
	#undef CreateWindow

	IWindow* WindowFactory::CreateWindow(Widget* widget, OsType osType, WindowType windowType,
		PainterType painterType, IWindow* parent)
	{
		IWindow* createdWindow = nullptr;
		switch (osType) {
		case OsType::Windows:
			createdWindow = new WindowWin32{ widget, windowType, painterType, parent };
			break;
		}
		return createdWindow;
	}
}