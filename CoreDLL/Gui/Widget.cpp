#define DLL_EXPORT_SIGNATURE

#include "Widget.h"
#include "../Bean/Config.h"
#include "../Bean/Exception.h"
#include "../OsGui/WindowFactory.h"
#include "Layout/DefaultLayout.h"
#include "../Application/NkApplication.h"

namespace Nk {
	using namespace std;

	const ClassId Widget::m_classId = Object::RegisterNewClass("Gui::Widget");
	const char* Widget::EventsNames[Events::_LAST_] = { "Core_Repaint_Window_Buffer", "Core_Draw_Window_Buffer",
		"Core_OnMouseMove", "Core_OnMouseRDown"};



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
		m_windowOs = WindowFactory::CreateWindow(this, OsType::Windows, WindowType::OVERLAPPED_WINDOW, 
			PainterType::DirectX, parentWindow);
	}

	Widget::Widget(Widget* widget) :Widget{widget, { 0.7, 0.7, 0.7, 1.0 } } {

	}



	Widget::~Widget() {
		if (m_parentWidget != nullptr) {
			m_parentWidget->RemoveChildWidget(this);
		}
		delete m_windowOs;
		delete m_widgetLayout;
		DeleteCriticalSection(&m_drawLockObject);
	}

	void Widget::AddChildWidget(Widget* childWidget) {
		if (find(m_childWidgetList.cbegin(), m_childWidgetList.cend(), childWidget) != m_childWidgetList.end()) {
			throw Exception{"Can't register child window twice"};
		}
		m_childWidgetList.push_back(childWidget);
	}


	void Widget::RemoveChildWidget(Widget* childWidget) {
		auto childWidgetIter = find(m_childWidgetList.cbegin(), m_childWidgetList.cend(), childWidget);
		if (childWidgetIter == m_childWidgetList.cend()) {
			throw Exception{ "No such child widget" };
		}
		m_childWidgetList.erase(childWidgetIter);
	}

	/////////////////////////////////////////////////////////////Graphics methods

	void Widget::SetWindowGeometry(Coord_t x, Coord_t y, Coord_t w, Coord_t h) {
		m_windowOs->SetWindowGeometry(x, y, w, h);
		EnterCriticalSection(&m_drawLockObject);
		m_x = x; m_y = y; m_w = w; m_h = h;
		LeaveCriticalSection(&m_drawLockObject);
		this->Repaint();
	}


	void Widget::ShowWindow() {
		if (!m_isVisible) {
			m_windowOs->ShowWindow();
			m_isVisible = true;
			this->Repaint();
		}
		for (auto child : m_childWidgetList) {
			child->ShowWindow();
		}
	}


	void Widget::HideWindow() {
		if (m_isVisible) {
			m_windowOs->HideWindow();
			m_isVisible = false;
			this->Repaint();
		}
	}


	void Widget::SetBackgroundColor(Color_t bkColor) {
		m_backgroundColor = bkColor;
	}

	////////////////////////////////CUSTOM EVENT HANDLERS

	void Widget::OnRepaintWindow(void* widget) {	//Event from the system (like WM_PAINT) - try redraw buffer
		Widget* senderWidget = (Widget*)widget;
		if (senderWidget->m_parentWidget == nullptr) { //Repaint only root elements
			senderWidget->m_windowOs->DrawWindow();
		}
	}


	void Widget::OnDrawWindow(void* widget) {	//Need redraw the window
		Widget* senderWidget = (Widget*)widget;
		EnterCriticalSection(&senderWidget->m_drawLockObject);
		senderWidget->m_windowOs->BeginDrawWindowBuffer();
		senderWidget->m_userDrawProc(senderWidget, senderWidget->m_windowOs->GetPainter());	//Call user draw proc
		for (auto child : senderWidget->m_childWidgetList) {
			if (child->m_isVisible) {
				if (!senderWidget->m_isNeedTotalRedraw && child->m_isBackBufferActive) {
					child->m_windowOs->DrawWindow();
				}
				else {
					if (senderWidget->m_isNeedTotalRedraw) child->m_isNeedTotalRedraw = true;
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
		EnterCriticalSection(&m_drawLockObject);
		m_isNeedTotalRedraw = true;
		m_isBackBufferActive = false;
		Widget* currentWidget = this;
		while (currentWidget->m_parentWidget != nullptr) {
			currentWidget = currentWidget->m_parentWidget;
			EnterCriticalSection(&currentWidget->m_drawLockObject);
			currentWidget->m_isBackBufferActive = false;
			LeaveCriticalSection(&currentWidget->m_drawLockObject);
		}
		LeaveCriticalSection(&m_drawLockObject);
		//currentWidget->SendRepaintEvent();
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