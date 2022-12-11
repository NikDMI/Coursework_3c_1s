#include "ScrollBar.h"
#include "../../EventStructures/MouseStructure.h"

namespace Nk {

	const Color_t ScrollBar::DEFAULT_COLOR = { 0.4, 0.4, 0.4, 1.0 };

	void PROC_CALL OnMouseDownScrollElement(void* params);
	void PROC_CALL OnMouseUpScrollElement(void* params);
	void PROC_CALL OnMouseMoveScrollElement(void* params);
	void PROC_CALL OnParentResizeScrollElement(void* params);

	ScrollBar::ScrollBar(Widget* parent, int32_t lowerValue, int32_t upperValue) : ISlider{ parent, lowerValue, upperValue } {
		SetBackgroundColor(DEFAULT_COLOR);
		m_scrollElement = new ImageButton{ nullptr, this };
		m_scrollElement->SetCustomEvent(CustomEvents::ON_MOUSE_LDOWN, OnMouseDownScrollElement);
		m_scrollElement->SetCustomEvent(CustomEvents::ON_MOUSE_LUP, OnMouseUpScrollElement);
		m_scrollElement->SetCustomEvent(CustomEvents::ON_MOUSE_MOVE, OnMouseMoveScrollElement);
		m_scrollElement->SetCustomEvent(CustomEvents::ON_PARENT_RESIZE, OnParentResizeScrollElement);
		m_scrollElement->SetParentNotification();
		m_scrollElement->ShowWindow();
	}


	ScrollBar::~ScrollBar() {
		delete m_scrollElement;
	}


	ImageButton* ScrollBar::GetScrollElement() const noexcept {
		return m_scrollElement;
	}


	void ScrollBar::ProcessScrollEvent() {

	}


	void PROC_CALL OnParentResizeScrollElement(void* params) {
		BasicWidgetStructure* mouseStructure = (BasicWidgetStructure*)params;
		ImageButton* btn = (ImageButton*)mouseStructure->sender;
		ScrollBar* scroll = (ScrollBar*)btn->GetParentWidget();
		Rect_t scrollArea = scroll->GetWidgetClientRect();
		btn->SetWindowGeometry(0, 0, scrollArea.w, 30);
	}


	void PROC_CALL OnMouseDownScrollElement(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		ImageButton* btn = (ImageButton*)mouseStructure->sender;
		btn->Button_OnMouseLDown(params);
		ScrollBar* scroll = (ScrollBar*)btn->GetParentWidget();
		scroll->m_isScrolled = true;
		scroll->m_fixCursorPoint = ICursor::GetGlobalMouseCoord();
		scroll->m_startScrollElementPosition = btn->GetWidgetRect();
		btn->SetMouseCapture();
	}


	void PROC_CALL OnMouseUpScrollElement(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		ImageButton* btn = (ImageButton*)mouseStructure->sender;
		btn->Button_OnLMouseLUp(params);
		ScrollBar* scroll = (ScrollBar*)btn->GetParentWidget();
		scroll->m_isScrolled = false;
		btn->ReleaseMouseCapture();
	}


	void PROC_CALL OnMouseMoveScrollElement(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		ImageButton* btn = (ImageButton*)mouseStructure->sender;
		btn->Button_OnMouseMove(params);
		ScrollBar* scroll = (ScrollBar*)btn->GetParentWidget();
		if (scroll->m_isScrolled) {
			scroll->MoveScrollElement();
			scroll->SendScrollEvent();
		}
	}


	void ScrollBar::MoveScrollElement() {
		Rect_t scrollElementRect = m_scrollElement->GetWidgetRect();
		Rect_t scrollArea = GetWidgetClientRect();
		Coord_t freeSpace = scrollArea.h - scrollElementRect.h;
		if (freeSpace < 0) {
			m_currentValue = m_lowerValue;
			return;
		}
		//Count new y position of scroll element
		auto cursorPoint = ICursor::GetGlobalMouseCoord();
		Coord_t dy = cursorPoint.y - m_fixCursorPoint.y;
		Coord_t predefinedCoord = m_startScrollElementPosition.y + dy;
		if (predefinedCoord < 0) {
			scrollElementRect.y = 0;
			m_currentValue = m_lowerValue;
		}
		else if (predefinedCoord > scrollArea.h - scrollElementRect.h) {
			scrollElementRect.y = scrollArea.h - scrollElementRect.h;
			m_currentValue = m_upperValue;
		}
		else {
			scrollElementRect.y = predefinedCoord;
			m_currentValue = (m_upperValue - m_lowerValue) * (predefinedCoord / freeSpace);
		}
		m_scrollElement->SetWindowGeometry(scrollElementRect.x, scrollElementRect.y, scrollElementRect.w, scrollElementRect.h);
		m_scrollElement->Repaint();
	}
}