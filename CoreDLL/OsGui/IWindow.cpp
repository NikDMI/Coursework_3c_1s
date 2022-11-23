#define DLL_EXPORT_SIGNATURE

#include "IWindow.h"

namespace Nk {

	IWindow::IWindow(Widget* widget, WindowType windowType, PainterType painterType, IWindow* parent) :
		m_painterType{ painterType }, m_windowType{ windowType }
	{
		
	}


	IWindow::~IWindow() {

	}
}