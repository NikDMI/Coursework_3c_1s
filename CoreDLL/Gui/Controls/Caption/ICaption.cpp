#include "ICaption.h"
#include "../../EventStructures/MouseStructure.h"

namespace Nk {

	void PROC_CALL OnMouseDown(void* params);
	void PROC_CALL OnMouseMove(void* params);
	void PROC_CALL OnMouseUp(void* params);


	ICaption::ICaption(Widget* parent, const std::wstring& text) :Widget{ parent }, ITextElement{this, text} {
		SetCustomEvent(CustomEvents::ON_MOUSE_MOVE, OnMouseMove);
		SetCustomEvent(CustomEvents::ON_MOUSE_LDOWN, OnMouseDown);
		SetCustomEvent(CustomEvents::ON_MOUSE_LUP, OnMouseUp);
	}


	void PROC_CALL OnMouseDown(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		ICaption* caption = (ICaption*)mouseStructure->sender;
		caption->SetMouseCapture();
		caption->m_isCaptured = true;
		caption->m_lastCursorPosition = ICursor::GetGlobalMouseCoord();
	}


	void PROC_CALL OnMouseUp(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		ICaption* caption = (ICaption*)mouseStructure->sender;
		caption->ReleaseMouseCapture();
		caption->m_isCaptured = false;
	}


	void PROC_CALL OnMouseMove(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		ICaption* caption = (ICaption*)mouseStructure->sender;
		if (caption->m_isCaptured) {
			Point_t currentCursorPos = ICursor::GetGlobalMouseCoord();
			Coord_t dx = currentCursorPos.x - caption->m_lastCursorPosition.x;
			Coord_t dy = currentCursorPos.y - caption->m_lastCursorPosition.y;
			Widget* parent = caption->GetParentWidget();
			if (parent != nullptr) {
				parent->OffsetWindow(dx, dy);
			}
			else {
				caption->OffsetWindow(dx, dy);
			}
			caption->m_lastCursorPosition = currentCursorPos;
		}
	}

}