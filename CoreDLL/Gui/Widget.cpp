#define DLL_EXPORT_SIGNATURE

#include "Widget.h"
#include "../Bean/Config.h"
#include "../Bean/Exception.h"
#include "../OsGui/WindowFactory.h"
#include "Layout/DefaultLayout.h"
#include "../Application/NkApplication.h"

namespace Nk {
	using namespace std;

	Widget* Widget::m_captureWidget = nullptr;


	const ClassId Widget::m_classId = Object::RegisterNewClass("Gui::Widget");
	const char* Widget::EventsNames[Events::_LAST_] = { "Core_Repaint_Window_Buffer", "Core_Draw_Window_Buffer",
		"Core_OnMouseMove", "Core_OnMouseLDown", "Core_OnMouseLUp" };



	Widget::Widget(Widget* parent, Color_t backgroundColor) :m_parentWidget{ parent }, m_userDrawProc{this->BasicDrawProc},
		m_backgroundColor{backgroundColor}
	{
		InitializeCriticalSectionAndSpinCount(&m_drawLockObject, 2000);
		//Register basic gui events
		m_correspondingEventIndexes.resize((int)Events::_LAST_);
		for (int i = 0; i < Events::_LAST_; ++i) {
			m_correspondingEventIndexes[i] = Object::m_eventHandler->RegisterEvent(EventsNames[i]);
		}
		m_eventHandler->AddEventHandler(m_correspondingEventIndexes[Events::ON_REPAINT], { OnRepaintWindow, true });
		m_eventHandler->AddEventHandler(m_correspondingEventIndexes[Events::ON_DRAW], { OnDrawWindow, true });
		//Add widget to parent list
		if (parent != nullptr) {
			parent->AddChildWidget(this);
		}
		//Create default widget layout
		m_widgetLayout = new DefaultLayout{};
		//Create window
		IWindow* parentWindow = parent == nullptr ? nullptr : parent->m_windowOs;
		m_windowOs = WindowFactory::CreateWindow(this, WindowType::OVERLAPPED_WINDOW, parentWindow);
		m_defaultLayout = new DefaultLayout{};
		m_widgetLayout = m_defaultLayout;
	}

	Widget::Widget(Widget* widget) :Widget{widget, { 0.7, 0.7, 0.7, 1.0 } } {

	}



	Widget::~Widget() {
		if (m_parentWidget != nullptr) {
			m_parentWidget->RemoveChildWidget(this);
		}
		delete m_windowOs;
		delete m_widgetLayout;
		delete m_defaultLayout;
		DeleteCriticalSection(&m_drawLockObject);
	}

	void Widget::AddChildWidget(Widget* childWidget) {
		if (find(m_childWidgetList.cbegin(), m_childWidgetList.cend(), childWidget) != m_childWidgetList.end()) {
			throw Exception{"Can't register child window twice"};
		}
		m_childWidgetList.push_back(childWidget);
		if (m_widgetLayout == m_defaultLayout) {
			m_widgetLayout->AddWidget(childWidget);
		}
	}


	void Widget::RemoveChildWidget(Widget* childWidget) {
		auto childWidgetIter = find(m_childWidgetList.cbegin(), m_childWidgetList.cend(), childWidget);
		if (childWidgetIter == m_childWidgetList.cend()) {
			throw Exception{ "No such child widget" };
		}
		m_childWidgetList.erase(childWidgetIter);
		if (m_widgetLayout == m_defaultLayout) {
			m_widgetLayout->RemoveWidget(childWidget);
		}
	}


	void Widget::SetWindowDrawProc(WindowDrawProc drawProc) {
		if (drawProc == nullptr) {
			throw Exception {"Nullptr argument"};
		}
		EnterCriticalSection(&m_drawLockObject);
		m_userDrawProc = drawProc;
		LeaveCriticalSection(&m_drawLockObject);
	}


	IPainter* Widget::GetPainter() {
		return m_windowOs->GetPainter();
	}


	Rect_t Widget::GetWidgetClientRect() {
		return { m_x, m_y, m_w, m_h };
	}


	void Widget::SetMouseCapture() {
		if (m_captureWidget == nullptr) {
			m_captureWidget = this;
			m_windowOs->SetMouseCapture();
		}
		else {
			throw Exception{"Only one window can be captured"};
		}
	}


	void Widget::ReleaseMouseCapture() {
		if (m_captureWidget == nullptr) {
			throw Exception{ "There is no captured window" };
		}
		m_captureWidget = nullptr;
		m_windowOs->ReleaseMouseCapture();
	}


	Widget* Widget::GetParentWidget() {
		return m_parentWidget;
	}

	/////////////////////////////////////////////////////////////Graphics methods

	void Widget::SetWindowGeometry(Coord_t x, Coord_t y, Coord_t w, Coord_t h) {
		EnterCriticalSection(&m_drawLockObject);
		m_windowOs->SetWindowGeometry(x, y, w, h);
		m_x = x; m_y = y; m_w = w; m_h = h;
		m_isBackBufferActive = false;
		m_isNeedTotalRedraw = true;
		LeaveCriticalSection(&m_drawLockObject);
		//this->Repaint();
	}


	void Widget::OffsetWindow(Coord_t dx, Coord_t dy) {
		EnterCriticalSection(&m_drawLockObject);
		m_x += dx;
		m_y += dy;
		m_windowOs->SetWindowGeometry(m_x, m_y, m_w, m_h);
		LeaveCriticalSection(&m_drawLockObject);
	}


	void Widget::ShowWindow() {
		if (!m_isVisible) {
			EnterCriticalSection(&m_drawLockObject);
			m_windowOs->ShowWindow();
			m_isVisible = true;
			LeaveCriticalSection(&m_drawLockObject);
			this->Repaint();
		}
		for (auto child : m_childWidgetList) {
			child->ShowWindow();
		}
	}


	void Widget::HideWindow() {
		if (m_isVisible) {
			EnterCriticalSection(&m_drawLockObject);
			m_windowOs->HideWindow();
			m_isVisible = false;
			LeaveCriticalSection(&m_drawLockObject);
			this->Repaint();
		}
	}


	void Widget::SetBackgroundColor(Color_t bkColor) {
		EnterCriticalSection(&m_drawLockObject);
		m_backgroundColor = bkColor;
		LeaveCriticalSection(&m_drawLockObject);
	}

	////////////////////////////////CUSTOM EVENT HANDLERS

	void Widget::OnRepaintWindow(void* widget) {	//Event from the system (like WM_PAINT) - try redraw buffer
		Widget* senderWidget = (Widget*)widget;
		EnterCriticalSection(&senderWidget->m_drawLockObject);
		if (senderWidget->m_parentWidget == nullptr) { //Repaint only root elements
			senderWidget->m_windowOs->DrawWindow();
		}
		LeaveCriticalSection(&senderWidget->m_drawLockObject);
	}


	void Widget::OnDrawWindow(void* widget) {	//Need redraw the window
		Widget* senderWidget = (Widget*)widget;
		EnterCriticalSection(&senderWidget->m_drawLockObject);
		senderWidget->m_widgetLayout->ComputeWidgetsPositions();
		senderWidget->m_windowOs->BeginDrawWindowBuffer();
		senderWidget->m_userDrawProc(senderWidget, senderWidget->m_windowOs->GetPainter());	//Call user draw proc
		for (auto child : senderWidget->m_childWidgetList) {
			if (child->m_isVisible) {
				EnterCriticalSection(&child->m_drawLockObject);
				if (!senderWidget->m_isNeedTotalRedraw && child->m_isBackBufferActive) {
					child->m_windowOs->DrawWindow();
					LeaveCriticalSection(&child->m_drawLockObject);
				}
				else {
					if (senderWidget->m_isNeedTotalRedraw) child->m_isNeedTotalRedraw = true;
					LeaveCriticalSection(&child->m_drawLockObject);
					OnDrawWindow(child);
				}
			}
		}
		senderWidget->m_windowOs->EndDrawWindowBuffer();
		senderWidget->m_isBackBufferActive = true;	//Note, that back buffer is valid
		senderWidget->m_isNeedTotalRedraw = false;
		LeaveCriticalSection(&senderWidget->m_drawLockObject);
	}


	void Widget::Repaint() {
		//this vector can help avoid deadlocks
		vector<Widget*> widgetChain{ this };
		Widget* currentWidget = this;
		while (currentWidget->m_parentWidget != nullptr) {
			currentWidget = currentWidget->m_parentWidget;
			widgetChain.push_back(currentWidget);
		}
		EnterCriticalSection(&currentWidget->m_drawLockObject);
		currentWidget->m_isBackBufferActive = false;
		if (currentWidget == this) currentWidget->m_isNeedTotalRedraw = true;
		auto iter = widgetChain.rbegin();
		while (++iter != widgetChain.rend()) {
			EnterCriticalSection(&(*iter)->m_drawLockObject);
			(*iter)->m_isBackBufferActive = false;
			if (*iter == this) (*iter)->m_isNeedTotalRedraw = true;
			LeaveCriticalSection(&(*iter)->m_drawLockObject);
		}
		LeaveCriticalSection(&currentWidget->m_drawLockObject);
		OnDrawWindow(currentWidget);
	}


	void Widget::SendRepaintEvent() {
		m_windowOs->RefreshWindow();
		NkApplication::GetEventManager()->PushEvent(this,
			this->GetEventIndex(Widget::Events::ON_DRAW), this);
	}


	void Widget::BasicDrawProc(Widget* widget, IPainter* painter) {
		painter->ClearTarget(widget->m_backgroundColor);
	}


	//Config methods 
	EventIndex Widget::GetEventIndex(Events eventType) const {
		if (eventType < 0 || eventType >= Events::_LAST_) {
			throw Exception("Invalid eventType");
		}
		return m_correspondingEventIndexes[eventType];
	}
}