#define DLL_EXPORT_SIGNATURE

#include "Widget.h"
#include "../Bean/Config.h"
#include "../Bean/Exception.h"
#include "../OsGui/WindowFactory.h"
#include "Layout/DefaultLayout.h"

namespace Nk {
	using namespace std;

	const ClassId Widget::m_classId = Object::RegisterNewClass("Gui::Widget");
	const char* Widget::EventsNames[Events::_LAST_] = { "Core_Repaint_Window_Buffer", "Core_Draw_Window_Buffer",
		"Core_OnMouseMove", "Core_OnMouseRDown"};



	Widget::Widget(Widget* parent, Color_t backgroundColor) :m_parentWidget{ parent }, m_userDrawProc{this->BasicDrawProc},
		m_backgroundColor{backgroundColor}
	{
		//Register basic gui events
		m_correspondingEventIndexes.resize((int)Events::_LAST_);
		for (int i = 0; i < Events::_LAST_; ++i) {
			m_correspondingEventIndexes[i] = Object::m_eventHandler->RegisterEvent(EventsNames[i]);
		}
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

	Widget::Widget(Widget* widget = nullptr) :Widget{widget, { 0.7, 0.7, 0.7, 1.0 } } {

	}



	Widget::~Widget() {
		if (m_parentWidget != nullptr) {
			m_parentWidget->RemoveChildWidget(this);
		}
		delete m_windowOs;
		delete m_widgetLayout;
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
	}


	void Widget::ShowWindow() {
		if (!m_isVisible) {
			m_windowOs->ShowWindow();
			for (auto child : m_childWidgetList) {
				child->ShowWindow();
			}
			m_isVisible = true;
		}
	}


	void Widget::HideWindow() {
		if (m_isVisible) {
			m_windowOs->HideWindow();
			m_isVisible = false;
		}
	}

	////////////////////////////////CUSTOM EVENT HANDLERS

	void Widget::OnRepaintWindow(void* params) {
		m_windowOs->DrawWindow();
	}


	void Widget::OnDrawWindow(void* params) {
		m_windowOs->BeginDrawWindowBuffer();
		//Call user draw proc
		m_windowOs->EndDrawWindowBuffer();
	}


	void Widget::BasicDrawProc(Widget* widget, IPainter* painter) {

	}


	//Config methods 
	EventIndex Widget::GetEventIndex(Events eventType) const {
		if (eventType < 0 || eventType >= Events::_LAST_ || eventType == Events::ON_DRAW) {
			throw Exception("Invalid eventType");
		}
		return m_correspondingEventIndexes[eventType];
	}
}