#include "MainWindow.h"

namespace Nk {

	const Color_t MainWindow::DEFAULT_COLOR = {0.6, 0.6, 0.6, 1.0};


	MainWindow::MainWindow(std::wstring windowName, Coord_t x, Coord_t y, Coord_t w, Coord_t h) :
		IWindowWidget{nullptr, x, y, w, h} 
	{
		m_caption = new MainCaption{ this, windowName };
		this->SetHeaderWidget(m_caption);
		this->SetBackgroundColor(DEFAULT_COLOR);
	}


	MainCaption* MainWindow::GetWindowCaption() {
		return m_caption;
	}


	MainWindow::~MainWindow() {
		delete m_caption;
	}

}