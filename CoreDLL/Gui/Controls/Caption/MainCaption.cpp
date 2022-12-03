#include "MainCaption.h"
#include "../../EventStructures/MouseStructure.h"
#include "../../../Tools/Cursor/ICursor.h"

namespace Nk {

	void PROC_CALL CaptionDrawProc(Widget* widget, IPainter* painter);
	void PROC_CALL OnMouseDown(void* params);
	void PROC_CALL OnMouseUp(void* params);
	void PROC_CALL OnMouseMove(void* params);

	MainCaption::MainCaption(Widget* parent, const std::wstring& caption) : ICaption{parent, caption} {
		m_parentWidget = GetParentWidget();
		//Basic configs
		int width = m_parentWidget == nullptr ? DEFAULT_CAPTION_WIDTH : m_parentWidget->GetWidgetClientRect().w;
		this->SetWindowGeometry(0, 0, width, DEFAULT_CAPTION_HEIGHT);
		m_elementFont->SetVerticalAlignment(IFont::VerticalAlignment::CENTER);
		this->SetWindowDrawProc(CaptionDrawProc);
		//Events
		m_eventHandler->AddEventHandler(GetEventIndex(Widget::Events::ON_MOUSE_MOVE), { OnMouseMove });
		m_eventHandler->AddEventHandler(GetEventIndex(Widget::Events::ON_MOUSE_LDOWN), { OnMouseDown });
		m_eventHandler->AddEventHandler(GetEventIndex(Widget::Events::ON_MOUSE_LUP), { OnMouseUp });
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
		//Widget::BasicDrawProc(widget, painter);
		MainCaption* caption = (MainCaption*)widget;
		Rect_t capRect = caption->GetWidgetClientRect();
		capRect.x = caption->m_xTextOffset;
		//labelRect.y = caption->m_yTextOffset;
		painter->DrawText(capRect, caption->m_elementText);
	}
}