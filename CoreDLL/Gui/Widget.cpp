#define DLL_EXPORT_SIGNATURE

#include "Widget.h"
#include "../Bean/Config.h"
#include "../Bean/Exception.h"
#include "../OsGui/WindowFactory.h"
#include "Layout/DefaultLayout.h"
#include "../Application/NkApplication.h"
#include "EventStructures/MouseStructure.h"
#include "EventStructures/BasicWidgetStructure.h"
#include "EventStructures/ResizeWindowStructure.h"
#include "EventStructures/KeyBoard.h"
#include "Border/IBorder.h"

namespace Nk {
	using namespace std;

	Widget* Widget::m_captureWidget = nullptr;


	const ClassId Widget::m_classId = Object::RegisterNewClass("Gui::Widget");
	const char* Widget::EventsNames[Events::_LAST_] = { "Core_Repaint_Window_Buffer", "Core_Draw_Window_Buffer",
		"Core_OnMouseMove", "Core_OnMouseLDown", "Core_OnMouseLUp", "Core_OnMouseEnter", "Core_OnMouseLeave",
		"Core_OnFocus", "Core_OnKillFocus", "Core_OnChar", "Core_OnKeyDown" , "Core_OnKeyUp" , "Core_OnWindowResize" };



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
		widgetEventHandler->AddEventHandler(m_correspondingEventIndexes[Events::ON_MOUSE_ENTER], { WidgetOnMouseEnter, true });
		widgetEventHandler->AddEventHandler(m_correspondingEventIndexes[Events::ON_MOUSE_LEAVE], { WidgetOnMouseLeave, true });
		widgetEventHandler->AddEventHandler(m_correspondingEventIndexes[Events::ON_GET_FOCUS], { WidgetOnSetFocus, true });
		widgetEventHandler->AddEventHandler(m_correspondingEventIndexes[Events::ON_LEAVE_FOCUS], { WidgetOnKillFocus, true });
		widgetEventHandler->AddEventHandler(m_correspondingEventIndexes[Events::ON_CHAR], { WidgetOnChar, true });
		widgetEventHandler->AddEventHandler(m_correspondingEventIndexes[Events::ON_KEY_DOWN], { WidgetOnKeyDown, true });
		widgetEventHandler->AddEventHandler(m_correspondingEventIndexes[Events::ON_KEY_UP], { WidgetOnKeyUp, true });
		widgetEventHandler->AddEventHandler(m_correspondingEventIndexes[Events::ON_WINDOW_RESIZE], { WidgetOnWindowResize, true });
		//Add widget to parent list
		if (parent != nullptr) {
			parent->AddChildWidget(this);
		}
		//Create default widget layout
		//m_widgetLayout = new DefaultLayout{};
		//Create window
		IWindow* parentWindow = parent == nullptr ? nullptr : parent->m_windowOs;
		m_windowOs = WindowFactory::CreateWindow(this, WindowType::OVERLAPPED_WINDOW, parentWindow);
		m_defaultLayout = new DefaultLayout{};
		m_widgetLayout = m_defaultLayout;
		m_currentCursor = ICursor::GetDefaultCursor();
	}

	Widget::Widget(Widget* widget) :Widget{widget, { 0.7, 0.7, 0.7, 1.0 } } {

	}


	Widget::~Widget() {
		if (m_parentWidget != nullptr) {
			m_parentWidget->RemoveChildWidget(this);
		}
		delete m_windowOs;
		//delete m_widgetLayout;
		delete m_defaultLayout;
		DeleteCriticalSection(&m_drawLockObject);
	}


	void Widget::AddChildWidget(Widget* childWidget) {
		if (find(m_childWidgetList.cbegin(), m_childWidgetList.cend(), childWidget) != m_childWidgetList.end()) {
			throw Exception{"Can't register child window twice"};
		}
		m_childWidgetList.push_back(childWidget);
		//if (m_widgetLayout == m_defaultLayout) {
			m_widgetLayout->AddWidget(childWidget);
		//}
	}


	void Widget::RemoveChildWidget(Widget* childWidget) {
		auto childWidgetIter = find(m_childWidgetList.cbegin(), m_childWidgetList.cend(), childWidget);
		if (childWidgetIter == m_childWidgetList.cend()) {
			throw Exception{ "No such child widget" };
		}
		m_childWidgetList.erase(childWidgetIter);
		//if (m_widgetLayout == m_defaultLayout) {
			m_widgetLayout->RemoveWidget(childWidget);
		//}
	}


	void Widget::AddNewLayout(ILayout* layout, bool isDeleteLast) {
		if (layout == nullptr) {
			throw Exception{ "nullptr arg" };
		}
		if (layout == m_widgetLayout) return;
		if (isDeleteLast && m_widgetLayout != m_defaultLayout) {
			delete m_widgetLayout;
		}
		m_widgetLayout = layout;
		for (Widget* child : m_childWidgetList) {
			if (child != m_headerWidget && child != m_leftBorder && child != m_topBorder && child != m_rightBorder && child != m_bottomBorder) {
				layout->AddWidget(child);
			}
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


	void Widget::ComputeCursorPointWithoutHelpWidgets(Point_t& inOutPoint) {
		if (m_headerWidget) {
			inOutPoint.y -= m_headerWidget->GetWidgetRect().h;
		}
		if (m_leftBorder) {
			inOutPoint.x -= m_leftBorder->GetBorderWidth();
		}
		if (m_topBorder) {
			inOutPoint.y -= m_topBorder->GetBorderWidth();
		}
	}


	void Widget::ComputeOriginPointWithoutHelpWidgets(Point_t& inOutPoint) {
		if (m_headerWidget) {
			inOutPoint.y += m_headerWidget->GetWidgetRect().h;
		}
		if (m_leftBorder) {
			inOutPoint.x += m_leftBorder->GetBorderWidth();
		}
		if (m_topBorder) {
			inOutPoint.y += m_topBorder->GetBorderWidth();
		}
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
		Point_t originPoint = {0, 0};
		if (m_parentWidget && this != m_parentWidget->m_headerWidget && this != m_parentWidget->m_bottomBorder && 
			this != m_parentWidget->m_topBorder && this != m_parentWidget->m_leftBorder && this != m_parentWidget->m_rightBorder) {
			m_parentWidget->ComputeOriginPointWithoutHelpWidgets(originPoint);
		}
		++m_customResizeCount;
		m_windowOs->SetWindowGeometry(x, y, w, h, originPoint);
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


	void Widget::SetCursor(ICursor* cursor) {
		m_currentCursor = cursor;
		if (m_leaveCursor != nullptr) {	//mouse in the area of widget
			cursor->ChooseCursor();
		}
	}


	void Widget::SetResizeManager(IResizeManager* resizeManager) {
		m_resizeManager = resizeManager;
	}


	void Widget::SetFocus() {
		m_windowOs->SetFocus();
	}


	void Widget::SetResizingSystemMode(bool canResize) {
		m_canResizeBySystem = canResize;
	}


	bool Widget::GetResizingSystemMode() {
		return m_canResizeBySystem;
	}

	////////////////////////////////CUSTOM EVENT HANDLERS

	void Widget::OnRepaintWindow(void* widget) {	//Event from the system (like WM_PAINT) - try redraw buffer
		Widget* senderWidget = (Widget*)widget;
		EnterCriticalSection(&senderWidget->m_drawLockObject);
		if (senderWidget->m_parentWidget == nullptr && senderWidget->m_isBackBufferActive) { //Repaint only root elements
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
		if (widget == parent->m_headerWidget || widget == parent->m_topBorder ||
			widget == parent->m_bottomBorder || widget == parent->m_leftBorder || widget == parent->m_rightBorder) {

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


	void Widget::CheckResizingOnMouseMove(Point_t cursorPos) {
		if (m_resizeManager) {
			if (m_isResizing) {//User capture window and resize it
				Point_t globalCursorCoord = ICursor::GetGlobalMouseCoord();
				Coord_t dx = globalCursorCoord.x - m_lastResizingPosition.x;
				Coord_t dy = globalCursorCoord.y - m_lastResizingPosition.y;
				switch (m_lastResizeType) {
				case IResizeManager::ResizeType::DOWN:
					SetWindowGeometry(m_x, m_y, m_w, m_h + dy);
					break;
				case IResizeManager::ResizeType::UP:
					SetWindowGeometry(m_x, m_y + dy, m_w, m_h - dy);
					break;
				case IResizeManager::ResizeType::LEFT:
					SetWindowGeometry(m_x + dx, m_y, m_w - dx, m_h);
					break;
				case IResizeManager::ResizeType::RIGHT:
					SetWindowGeometry(m_x, m_y, m_w + dx, m_h);
					break;
				case IResizeManager::ResizeType::DOWN_LEFT:
					SetWindowGeometry(m_x + dx, m_y, m_w - dx, m_h + dy);
					break;
				case IResizeManager::ResizeType::DOWN_RIGHT:
					SetWindowGeometry(m_x, m_y, m_w + dx, m_h + dy);
					break;
				case IResizeManager::ResizeType::UP_LEFT:
					SetWindowGeometry(m_x + dx, m_y + dy, m_w - dx, m_h - dy);
					break;
				case IResizeManager::ResizeType::UP_RIGHT:
					SetWindowGeometry(m_x, m_y + dy, m_w + dx, m_h - dy);
					break;
				}
				m_lastResizingPosition = globalCursorCoord;
			}
			else {
				auto currentResizeType = m_resizeManager->GetResizeType(GetWidgetClientRect(), cursorPos);
				if (currentResizeType != m_lastResizeType) {//Cahnge cursor
					auto lastUserCursorAtResizingProcess = m_currentCursor;
					switch (currentResizeType) {
					case IResizeManager::ResizeType::DOWN:
						SetCursor(ICursor::GetDefaultCursor(ICursor::SystemCursor::ARROW));
						break;
					case IResizeManager::ResizeType::UP:
						SetCursor(ICursor::GetDefaultCursor(ICursor::SystemCursor::ARROW));
						break;
					case IResizeManager::ResizeType::LEFT:
						SetCursor(ICursor::GetDefaultCursor(ICursor::SystemCursor::ARROW));
						break;
					case IResizeManager::ResizeType::RIGHT:
						SetCursor(ICursor::GetDefaultCursor(ICursor::SystemCursor::ARROW));
						break;
					case IResizeManager::ResizeType::DOWN_LEFT:
						SetCursor(ICursor::GetDefaultCursor(ICursor::SystemCursor::ARROW));
						break;
					case IResizeManager::ResizeType::DOWN_RIGHT:
						SetCursor(ICursor::GetDefaultCursor(ICursor::SystemCursor::ARROW));
						break;
					case IResizeManager::ResizeType::UP_LEFT:
						SetCursor(ICursor::GetDefaultCursor(ICursor::SystemCursor::ARROW));
						break;
					case IResizeManager::ResizeType::UP_RIGHT:
						SetCursor(ICursor::GetDefaultCursor(ICursor::SystemCursor::ARROW));
						break;
					case IResizeManager::ResizeType::NONE:
						SetCursor(m_lastUserCursorAtResizingProcess);
						break;
					}
					m_lastUserCursorAtResizingProcess = lastUserCursorAtResizingProcess;
					m_lastResizeType = currentResizeType;
				}
			}
		}
	}


	////////////////////////////////////////////   BASIC EVENT HANDLERS

	void ChangeMouseStructCoord(void* param) {
		MouseStructure* mouseStructure = (MouseStructure*)param;
		Widget* sender = mouseStructure->sender;
		Point_t origin = { 0, 0 };
		sender->ComputeOriginPointWithoutHelpWidgets(origin);
		mouseStructure->xCoord_Px -= origin.x;
		mouseStructure->yCoord_Px -= origin.y;
	}

	void PROC_CALL Widget::WidgetOnMouseMove(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		Widget* senderWidget = mouseStructure->sender;
		//Check resize state
		if (senderWidget->m_resizeManager) {
			Point_t cursorPoint	{ mouseStructure->xCoord_Px, mouseStructure->yCoord_Px };
			senderWidget->ComputeCursorPointWithoutHelpWidgets(cursorPoint);
			senderWidget->CheckResizingOnMouseMove(cursorPoint);
			senderWidget->Repaint();
		}
		//Call user callback function
		ChangeMouseStructCoord(params);
		CallUserCallback(senderWidget, CustomEvents::ON_MOUSE_MOVE, params);
	}


	void PROC_CALL Widget::WidgetOnLMouseDown(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		Widget* senderWidget = mouseStructure->sender;
		//Check resize state
		if (senderWidget->m_resizeManager) {
			if (senderWidget->m_lastResizeType != IResizeManager::ResizeType::NONE) {
				senderWidget->m_isResizing = true;
				senderWidget->m_lastResizingPosition = ICursor::GetGlobalMouseCoord();
				senderWidget->SetMouseCapture();
			}
		}
		//Call user callback function
		ChangeMouseStructCoord(params);
		CallUserCallback(senderWidget, CustomEvents::ON_MOUSE_LDOWN, params);
	}


	void PROC_CALL Widget::WidgetOnLMouseUp(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		Widget* senderWidget = mouseStructure->sender;
		//Check resize state
		if (senderWidget->m_resizeManager) {
			if (senderWidget->m_isResizing) {
				senderWidget->m_isResizing = false;
				senderWidget->ReleaseMouseCapture();
			}
		}
		//Call user callback function
		ChangeMouseStructCoord(params);
		CallUserCallback(senderWidget, CustomEvents::ON_MOUSE_LUP, params);
	}


	void PROC_CALL Widget::WidgetOnMouseEnter(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		Widget* senderWidget = mouseStructure->sender;
		if (senderWidget->m_currentCursor) {
			senderWidget->m_leaveCursor = senderWidget->m_currentCursor->ChooseCursor();
		}
		senderWidget->m_lastResizeType = IResizeManager::ResizeType::NONE;
		senderWidget->m_isResizing = false;
		ChangeMouseStructCoord(params);
		CallUserCallback(senderWidget, CustomEvents::ON_MOUSE_ENTER, params);
	}


	void PROC_CALL Widget::WidgetOnMouseLeave(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		Widget* senderWidget = mouseStructure->sender;
		if (senderWidget->m_leaveCursor) {
			senderWidget->m_leaveCursor->ChooseCursor();
			if (senderWidget->m_lastResizeType != IResizeManager::ResizeType::NONE) {	//resizing process
				senderWidget->m_currentCursor = senderWidget->m_lastUserCursorAtResizingProcess;
			}
			else {
				//senderWidget->m_lastUserCursorAtResizingProcess->ChooseCursor();
			}
			senderWidget->m_leaveCursor = nullptr;
		}
		ChangeMouseStructCoord(params);
		CallUserCallback(senderWidget, CustomEvents::ON_MOUSE_LEAVE, params);
	}


	void PROC_CALL Widget::WidgetOnSetFocus(void* params) {
		BasicWidgetStructure* basicStructure = (BasicWidgetStructure*)params;
		Widget* senderWidget = basicStructure->sender;
		CallUserCallback(senderWidget, CustomEvents::ON_SET_FOCUS, params);
	}


	void PROC_CALL Widget::WidgetOnKillFocus(void* params) {
		BasicWidgetStructure* basicStructure = (BasicWidgetStructure*)params;
		Widget* senderWidget = basicStructure->sender;
		CallUserCallback(senderWidget, CustomEvents::ON_KILL_FOCUS, params);
	}


	void PROC_CALL Widget::WidgetOnChar(void* params) {
		KeyboardStructure* basicStructure = (KeyboardStructure*)params;
		Widget* senderWidget = basicStructure->sender;
		CallUserCallback(senderWidget, CustomEvents::ON_CHAR, params);
	}


	void PROC_CALL Widget::WidgetOnKeyUp(void* params) {
		KeyboardStructure* basicStructure = (KeyboardStructure*)params;
		Widget* senderWidget = basicStructure->sender;
		CallUserCallback(senderWidget, CustomEvents::ON_KEY_UP, params);
	}


	void PROC_CALL Widget::WidgetOnKeyDown(void* params) {
		KeyboardStructure* basicStructure = (KeyboardStructure*)params;
		Widget* senderWidget = basicStructure->sender;
		CallUserCallback(senderWidget, CustomEvents::ON_KEY_DOWN, params);
	}

	
	void PROC_CALL Widget::WidgetOnWindowResize(void* params) {
		ResizeWindowStructure* resizeStructure = (ResizeWindowStructure*)params;
		Widget* senderWidget = resizeStructure->sender;
		senderWidget->SetWindowGeometry(resizeStructure->xCoord_Px, resizeStructure->yCoord_Px, resizeStructure->wCoord_Px, resizeStructure->hCoord_Px);
		senderWidget->Repaint();
	}

}