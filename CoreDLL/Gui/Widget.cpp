#define DLL_EXPORT_SIGNATURE

#include "Widget.h"
#include "../Bean/Config.h"
#include "../Bean/Exception.h"
#include "../OsGui/WindowFactory.h"
#include "Layout/DefaultLayout.h"
#include "../Application/NkApplication.h"
#include "EventStructures/MouseStructure.h"
#include "EventStructures/BasicWidgetStructure.h"
#include "Border/IBorder.h"

namespace Nk {
	using namespace std;

	Widget* Widget::m_captureWidget = nullptr;


	const ClassId Widget::m_classId = Object::RegisterNewClass("Gui::Widget");
	const char* Widget::EventsNames[Events::_LAST_] = { "Core_Repaint_Window_Buffer", "Core_Draw_Window_Buffer",
		"Core_OnMouseMove", "Core_OnMouseLDown", "Core_OnMouseLUp" };



	Widget::Widget(Widget* parent, Color_t backgroundColor) :m_parentWidget{ parent }, m_userDrawProc{nullptr},
		m_backgroundColor{backgroundColor}
	{
		InitializeCriticalSectionAndSpinCount(&m_drawLockObject, 2000);
		//Register basic gui events
		EventHandler* widgetEventHandler = GetEventHandler();
		m_correspondingEventIndexes.resize((int)Events::_LAST_);
		for (int i = 0; i < Events::_LAST_; ++i) {
			m_correspondingEventIndexes[i] = widgetEventHandler->RegisterEvent(EventsNames[i]);
		}
		//Set core events proc (user can't override this proc)
		widgetEventHandler->AddEventHandler(m_correspondingEventIndexes[Events::ON_REPAINT], { OnRepaintWindow, true });
		widgetEventHandler->AddEventHandler(m_correspondingEventIndexes[Events::ON_DRAW], { OnDrawWindow, true });
		widgetEventHandler->AddEventHandler(m_correspondingEventIndexes[Events::ON_MOUSE_MOVE], { WidgetOnMouseMove, true });
		widgetEventHandler->AddEventHandler(m_correspondingEventIndexes[Events::ON_MOUSE_LDOWN], { WidgetOnLMouseDown, true });
		widgetEventHandler->AddEventHandler(m_correspondingEventIndexes[Events::ON_MOUSE_LUP], { WidgetOnLMouseUp, true });
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
		Rect_t clientRect { 0, 0, m_w, m_h };
		if (m_headerWidget != nullptr) {
			auto headerRect = m_headerWidget->GetWidgetRect();
			clientRect.h -= headerRect.h;
			clientRect.y += headerRect.y;
		}
		return clientRect;
	}


	Rect_t Widget::GetWidgetRect() {
		return { m_x, m_y, m_w, m_h };
	}


	Rect_t Widget::ComputeHeaderRect() {
		if (m_headerWidget == nullptr) {
			throw Exception{"No header widget"};
		}
		//Rect_t headerRect = m_headerWidget->GetWidgetRect();
		Rect_t headerRect = this->GetWidgetRect();
		headerRect.x = (m_leftBorder != nullptr) ? (m_leftBorder->GetBorderWidth()) : 0;
		headerRect.y = (m_topBorder != nullptr) ? m_topBorder->GetBorderWidth() : 0;
		//Compute width of the header
		headerRect.w -= (m_leftBorder != nullptr) ? (m_leftBorder->GetBorderWidth()) : 0;
		headerRect.w -= (m_rightBorder != nullptr) ? (m_rightBorder->GetBorderWidth()) : 0;
		return headerRect;
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


	void Widget::SetHeaderWidget(Widget* headerWidget) {
		auto iterWidget = std::find(m_childWidgetList.cbegin(), m_childWidgetList.cend(), headerWidget);
		if (iterWidget == m_childWidgetList.cend()) {
			throw Exception{ "Can't set header widget that is not a child" };
		}
		m_headerWidget = headerWidget;
	}


	void Widget::SetParentNotification() {
		m_isNeedParentEventsMessages = true;
	}


	void Widget::NotifyChilds(CustomEvents eventType, BasicWidgetStructure* params) {
		for (auto child : m_childWidgetList) {
			if (child->m_isNeedParentEventsMessages) {
				params->sender = child;
				CallUserCallback(child, eventType, params);
			}
		}
	}

	/////////////////////////////////////////////////////////////Graphics methods

	void Widget::SetWindowGeometry(Coord_t x, Coord_t y, Coord_t w, Coord_t h) {
		EnterCriticalSection(&m_drawLockObject);
		m_windowOs->SetWindowGeometry(x, y, w, h);
		m_x = x; m_y = y; m_w = w; m_h = h;
		m_isBackBufferActive = false;
		m_isNeedTotalRedraw = true;
		BasicWidgetStructure bs;
		NotifyChilds(CustomEvents::ON_PARENT_RESIZE, &bs);
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


	void Widget::SetBorder(IBorder* border) {
		switch (border->GetBorderType()) {

		case IBorder::BorderType::BOTTOM:
			m_bottomBorder = border;
			if (m_leftBorder) {
				border->SetBorderOffset(m_leftBorder->GetBorderOffset());
			}
			break;

		case IBorder::BorderType::TOP:
			m_topBorder = border;
			if (m_leftBorder) {
				border->SetBorderOffset(m_leftBorder->GetBorderOffset());
			}
			break;

		case IBorder::BorderType::LEFT:
			m_leftBorder = border;
			if (m_topBorder) {
				m_topBorder->SetBorderOffset(border->GetBorderOffset());
			}
			if (m_bottomBorder) {
				m_topBorder->SetBorderOffset(border->GetBorderOffset());
			}
			break;

		case IBorder::BorderType::RIGHT:
			m_rightBorder = border;
			break;
		}
		if (m_headerWidget) {
			//Send event to header control to change size
			BasicWidgetStructure bs; bs.sender = m_headerWidget;
			CallUserCallback(m_headerWidget, CustomEvents::ON_PARENT_RESIZE, &bs);
		}
	}

	////////////////////////////////CUSTOM EVENT HANDLERS

	void Widget::OnRepaintWindow(void* widget) {	//Event from the system (like WM_PAINT) - try redraw buffer
		Widget* senderWidget = (Widget*)widget;
		EnterCriticalSection(&senderWidget->m_drawLockObject);
		if (senderWidget->m_parentWidget == nullptr) { //Repaint only root elements
			senderWidget->m_windowOs->GetPainter()->ClearTarget({ 0,0,0,0 });
			senderWidget->m_windowOs->DrawWindow();
		}
		LeaveCriticalSection(&senderWidget->m_drawLockObject);
	}


	void Widget::CheckIsHeaderWidget(Widget* widget) {
		Widget* parent = widget->m_parentWidget;
		if (parent && (widget == parent->m_headerWidget || widget == parent->m_topBorder ||
			widget == parent->m_bottomBorder || widget == parent->m_leftBorder || widget == parent->m_rightBorder)) {
			static Point_t staticViewport = {};
			parent->GetPainter()->SetStartViewportPoint(staticViewport);
		}
	}


	void Widget::ResumeIfHeaderWidget(Widget* widget) {
		Widget* parent = widget->m_parentWidget;
		if (widget == parent->m_headerWidget) {
			parent->GetPainter()->SetStartViewportPoint(parent->m_viewportPoint);
		}
	}


	void Widget::OnDrawWindow(void* widget) {	//Need redraw the window
		Widget* senderWidget = (Widget*)widget;
		EnterCriticalSection(&senderWidget->m_drawLockObject);
		senderWidget->m_widgetLayout->ComputeWidgetsPositions();
		senderWidget->m_windowOs->BeginDrawWindowBuffer();
		//DrawingProcess
		BeginBasicDrawProc(senderWidget, senderWidget->m_windowOs->GetPainter());
		if (senderWidget->m_userDrawProc) senderWidget->m_userDrawProc(senderWidget, senderWidget->m_windowOs->GetPainter());	//Call user draw proc
		for (auto child : senderWidget->m_childWidgetList) {
			if (child->m_isVisible) {
				EnterCriticalSection(&child->m_drawLockObject);
				if (!senderWidget->m_isNeedTotalRedraw && child->m_isBackBufferActive) {
					//Check if this is header control of the parent (return static viewport)
					CheckIsHeaderWidget(child);
					child->m_windowOs->DrawWindow();
					//Return viewport
					ResumeIfHeaderWidget(child);
					LeaveCriticalSection(&child->m_drawLockObject);
				}
				else {
					if (senderWidget->m_isNeedTotalRedraw) child->m_isNeedTotalRedraw = true;
					LeaveCriticalSection(&child->m_drawLockObject);
					//Check if this is header control of the parent (return static viewport)
					//CheckIsHeaderWidget(child);
					OnDrawWindow(child);
					//Return viewport
					//ResumeIfHeaderWidget(child);
				}
			}
		}
		senderWidget->m_windowOs->EndDrawWindowBuffer();
		EndBasicDrawProc(senderWidget, senderWidget->m_windowOs->GetPainter());
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
			m_correspondingEventIndexes[Widget::Events::ON_DRAW], this);
	}


	/*
	* Set configs at the start of the drawing
	*/
	void Widget::BeginBasicDrawProc(Widget* widget, IPainter* painter) {
		painter->ClearTarget(widget->m_backgroundColor);
		//Get viewport coord
		Point_t viewportPoint{};
		if (widget->m_headerWidget != nullptr) {
			Rect_t headerWidgetRect = widget->m_headerWidget->GetWidgetRect();
			viewportPoint.y += headerWidgetRect.h;
		}
		if (widget->m_topBorder) {
			viewportPoint.y += widget->m_topBorder->GetBorderWidth();
		}
		if (widget->m_leftBorder) {
			viewportPoint.x += widget->m_leftBorder->GetBorderWidth();
		}
		painter->SetStartViewportPoint(viewportPoint);
		widget->m_viewportPoint = viewportPoint;
		//Check if this is header control of the parent (return static viewport)
		Widget* parent = widget->m_parentWidget;
		if (parent != nullptr && (widget == parent->m_headerWidget || widget == parent->m_topBorder ||
			widget == parent->m_bottomBorder || widget == parent->m_leftBorder || widget == parent->m_rightBorder)) {
			static Point_t staticViewport = {};
			parent->GetPainter()->SetStartViewportPoint(staticViewport);
		}
	}


	void Widget::EndBasicDrawProc(Widget* widget, IPainter* painter) {
		//return viewport
		Point_t viewportPoint{};
		if (widget->m_headerWidget != nullptr) {
			painter->SetStartViewportPoint(viewportPoint);
			widget->m_viewportPoint = viewportPoint;
		}
		//Check if this is header control of the parent (return static viewport)
		Widget* parent = widget->m_parentWidget;
		if (parent != nullptr && (widget == parent->m_headerWidget || widget == parent->m_topBorder ||
			widget == parent->m_bottomBorder || widget == parent->m_leftBorder || widget == parent->m_rightBorder)) {
			//Set last viewport
			parent->GetPainter()->SetStartViewportPoint(widget->m_parentWidget->m_viewportPoint);
		}
	}


	//Config methods 
	EventIndex Widget::GetEventIndex(Events eventType) const {
		if (eventType < 0 || eventType >= Events::_LAST_) {
			throw Exception("Invalid eventType");
		}
		return m_correspondingEventIndexes[eventType];
	}

	void Widget::SetCustomEvent(CustomEvents eventType, EventHandlerProc callback) {
		if ((int)eventType < 0 || eventType >= CustomEvents::_LAST_) {
			throw Exception { "Invalid event type" };
		}
		m_userEventCallbacks[(int)eventType] = callback;
	}


	inline void Widget::CallUserCallback(Widget* senderWidget, CustomEvents customEvent, void* params) {
		EventHandlerProc userCallback = senderWidget->m_userEventCallbacks[(int)customEvent];
		if (userCallback) {
			userCallback(params);
		}
	}


	////////////////////////////////////////////   BASIC EVENT HANDLERS

	void PROC_CALL Widget::WidgetOnMouseMove(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		Widget* senderWidget = mouseStructure->sender;
		//Call user callback function
		CallUserCallback(senderWidget, CustomEvents::ON_MOUSE_MOVE, params);
		/*
		EventHandlerProc userCallback = senderWidget->m_userEventCallbacks[(int)CustomEvents::ON_MOUSE_MOVE];
		if (userCallback) {
			userCallback(params);
		}
		*/
	}


	void PROC_CALL Widget::WidgetOnLMouseDown(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		Widget* senderWidget = mouseStructure->sender;
		//Call user callback function
		CallUserCallback(senderWidget, CustomEvents::ON_MOUSE_LDOWN, params);
		/*
		EventHandlerProc userCallback = senderWidget->m_userEventCallbacks[(int)CustomEvents::ON_MOUSE_LDOWN];
		if (userCallback) {
			userCallback(params);
		}
		*/
	}


	void PROC_CALL Widget::WidgetOnLMouseUp(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		Widget* senderWidget = mouseStructure->sender;
		//Call user callback function
		CallUserCallback(senderWidget, CustomEvents::ON_MOUSE_LUP, params);
		/*
		EventHandlerProc userCallback = senderWidget->m_userEventCallbacks[(int)CustomEvents::ON_MOUSE_LUP];
		if (userCallback) {
			userCallback(params);
		}
		*/
	}

}