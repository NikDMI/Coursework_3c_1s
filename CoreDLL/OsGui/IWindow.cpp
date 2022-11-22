#include "IWindow.h"

namespace Nk {

	IWindow::IWindow(OsType osType, WindowType windowType, PainterType painterType) : m_osType{osType}, 
		m_painterType{ painterType }, m_windowType{windowType}
	{
		
	}


	IWindow::~IWindow() {

	}
}