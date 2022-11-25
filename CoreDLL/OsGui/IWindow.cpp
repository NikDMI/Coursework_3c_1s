#define DLL_EXPORT_SIGNATURE

#include "IWindow.h"
#include "../Gui/Widget.h"


namespace Nk {

	IWindow::IWindow(Widget* widget, WindowType windowType, PainterType painterType, IWindow* parent) :
		m_painterType{ painterType }, m_windowType{ windowType }, m_correspondingWidget{widget}
	{
		
	}


	IWindow::~IWindow() {

	}
}