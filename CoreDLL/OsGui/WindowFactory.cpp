#define DLL_EXPORT_SIGNATURE

#include "WindowFactory.h"
#include "Windows/WindowWin32.h"
#include "../Application/NkApplication.h"

namespace Nk {
	#undef CreateWindow

	IWindow* WindowFactory::CreateWindow(Widget* widget, WindowType windowType, IWindow* parent)
	{
		IWindow* createdWindow = nullptr;
		switch (NkApplication::GetNkApplication()->GetOsType()) {
		case OsType::Windows:
			createdWindow = new WindowWin32{ widget, windowType, NkApplication::GetNkApplication()->GetPainterType(), parent };
			break;
		}
		return createdWindow;
	}
}