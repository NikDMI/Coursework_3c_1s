#include "MainCaption.h"
#include "../../EventStructures/MouseStructure.h"
#include "../../../Tools/Cursor/ICursor.h"
#include "../Button/ImageButton.h"

namespace Nk {

	const std::wstring MainCaption::DEFAULT_CAPTION_FONT_FAMILY = L"Arial";
	const Color_t MainCaption::DEFAULT_CAPTION_COLOR = { 0.8, 0.8, 0.8, 1.0 };
	const Color_t MainCaption::HOVER_BUTTON_COLOR = { 0.85, 0.85, 0.85, 1.0 };
	const Color_t MainCaption::PUSH_BUTTON_COLOR = { 0.73, 0.73, 0.73, 1.0 };


	void PROC_CALL CaptionDrawProc(Widget* widget, IPainter* painter);
	void PROC_CALL OnMouseDown(void* params);
	void PROC_CALL OnMouseUp(void* params);
	void PROC_CALL OnMouseMove(void* params);
	void PROC_CALL MainCaption_OnParentResize(void* params);

	MainCaption::MainCaption(Widget* parent, const std::wstring& caption) : ICaption{parent, caption} {
		m_parentWidget = GetParentWidget();
		//Basic configs
		if (m_parentWidget != nullptr) {
			m_parentWidget->SetHeaderWidget(this);
			auto computedHeaderRect = m_parentWidget->ComputeHeaderRect();
			this->SetWindowGeometry(computedHeaderRect.x, computedHeaderRect.y, computedHeaderRect.w, DEFAULT_CAPTION_HEIGHT);
		}
		else {
			int width = m_parentWidget == nullptr ? DEFAULT_CAPTION_WIDTH : m_parentWidget->GetWidgetClientRect().w;
			this->SetWindowGeometry(0, 0, width, DEFAULT_CAPTION_HEIGHT);
		}
		m_elementFont->SetVerticalAlignment(IFont::VerticalAlignment::CENTER);
		m_elementFont->SetFamily(DEFAULT_CAPTION_FONT_FAMILY);
		m_elementFont->SetSizeInPixels(DEFAULT_CAPTION_FONT_WIDTH);
		this->SetBackgroundColor(DEFAULT_CAPTION_COLOR);
		this->SetWindowDrawProc(CaptionDrawProc);
		//Events
		SetCustomEvent(CustomEvents::ON_MOUSE_MOVE, OnMouseMove);
		SetCustomEvent(CustomEvents::ON_MOUSE_LDOWN, OnMouseDown);
		SetCustomEvent(CustomEvents::ON_MOUSE_LUP, OnMouseUp);
		SetCustomEvent(CustomEvents::ON_PARENT_RESIZE, MainCaption_OnParentResize);
		SetParentNotification();
		//Create buttons
		m_closeButton = new ImageButton{IBitmap::GetSystemBitmap(IBitmap::SystemBitmaps::CLOSE_IMAGE), this};
		auto rect = GetWidgetClientRect();
		m_closeButton->SetWindowGeometry(rect.w - DEFAULT_BUTTON_WIDTH, 0, DEFAULT_BUTTON_WIDTH, rect.h);
		m_closeButton->SetButtonColor(IColorElement::ElementState::STATIC, DEFAULT_CAPTION_COLOR);
		m_closeButton->SetButtonColor(IColorElement::ElementState::HOVER, HOVER_BUTTON_COLOR);
		m_closeButton->SetButtonColor(IColorElement::ElementState::PUSH, PUSH_BUTTON_COLOR);
		m_closeButton->SetBackgroundColor(DEFAULT_CAPTION_COLOR);
		m_closeButton->ShowWindow();
	}



	void PROC_CALL MainCaption_OnParentResize(void* params) {
		BasicWidgetStructure* bs = (BasicWidgetStructure*)params;
		MainCaption* caption = (MainCaption*)bs->sender;
		auto computedHeaderRect = caption->GetParentWidget()->ComputeHeaderRect();
		caption->SetWindowGeometry(computedHeaderRect.x, computedHeaderRect.y, computedHeaderRect.w, caption->m_captionHeight);
		//Change button positions 
		auto btnRect = caption->m_closeButton->GetWidgetRect();
		caption->m_closeButton->SetWindowGeometry(computedHeaderRect.w - btnRect.w, btnRect.y, btnRect.w, btnRect.h);

 	}

	void PROC_CALL OnMouseDown(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		MainCaption* caption = (MainCaption*)mouseStructure->sender;
		caption->SetMouseCapture();
		caption->m_isCaptured = true;
		caption->m_lastCursorPosition = ICursor::GetGlobalMouseCoord();
	}


	void PROC_CALL OnMouseUp(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		MainCaption* caption = (MainCaption*)mouseStructure->sender;
		caption->ReleaseMouseCapture();
		caption->m_isCaptured = false;
	}


	void PROC_CALL OnMouseMove(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		MainCaption* caption = (MainCaption*)mouseStructure->sender;
		if (caption->m_isCaptured) {
			Point_t currentCursorPos = ICursor::GetGlobalMouseCoord();
			Coord_t dx = currentCursorPos.x - caption->m_lastCursorPosition.x;
			Coord_t dy = currentCursorPos.y - caption->m_lastCursorPosition.y;
			if (caption->m_parentWidget != nullptr) {
				caption->m_parentWidget->OffsetWindow(dx, dy);
			}
			else {
				caption->OffsetWindow(dx, dy);
			}
			caption->m_lastCursorPosition = currentCursorPos;
		}
	}


	void PROC_CALL CaptionDrawProc(Widget* widget, IPainter* painter) {
		MainCaption* caption = (MainCaption*)widget;
		Rect_t capRect = caption->GetWidgetClientRect();
		capRect.x += caption->m_xTextOffset;
		painter->DrawText(capRect, caption->m_elementText);
	}
}