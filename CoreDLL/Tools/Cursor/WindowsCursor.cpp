#include "WindowsCursor.h"
#include <Windows.h>

#include "../../Bean/Exception.h"

namespace Nk {

	ICursor* WindowsCursor::defaultCursor = new WindowsCursor{ WindowsCursor::SystemCursor::ARROW };
	ICursor* WindowsCursor::currentICursorInTheSystem = WindowsCursor::defaultCursor;
	HCURSOR WindowsCursor::currentCursorInTheSystem = 0;



	WindowsCursor::WindowsCursor(IBitmap* cursorBitmap) :m_isSystemCursor{ false }, m_cursorImage{cursorBitmap} {
		if (cursorBitmap == nullptr) {
			throw Exception{ "Invalid bitmap address" };
		}
	}


	WindowsCursor::WindowsCursor(SystemCursor systemCursor): m_isSystemCursor{ true }, m_systemCursorType{ systemCursor } {
		LoadSystemCursor();
	}


	WindowsCursor::WindowsCursor() : WindowsCursor{ SystemCursor ::ARROW} {

	}


	WindowsCursor::~WindowsCursor() {
		if (!m_isSystemCursor) {
			//DeleteObject(m_currentCursor);
		}
	}


	void WindowsCursor::SetCursorImage(IBitmap* image) {

	}


	void WindowsCursor::SetSystemCursor(SystemCursor systemCursor) {
		m_isSystemCursor = true;
		m_systemCursorType = systemCursor;
		LoadSystemCursor();
	}


	ICursor* WindowsCursor::ChooseCursor() {
		if (!IsGlobalCursorSet()) {
			ICursor* lastCursor = currentICursorInTheSystem;
			if (currentCursorInTheSystem != m_currentCursor) {
				::SetCursor(m_currentCursor);
				currentCursorInTheSystem = m_currentCursor;
				currentICursorInTheSystem = this;
			}
			return lastCursor;
		}
		else {
			return ICursor::currenGlobalCursor;
		}
	}


	void WindowsCursor::LoadSystemCursor() {
		switch (m_systemCursorType) {
		case SystemCursor::ARROW:
			m_currentCursor = LoadCursor(NULL, IDC_ARROW);
			break;
		case SystemCursor::CROSS:
			m_currentCursor = LoadCursor(NULL, IDC_CROSS);
			break;
		default:
			throw Exception{"Invalid system cursor type"};
		}
	}


	HCURSOR WindowsCursor::GetHCursor() {
		return m_currentCursor;
	}

}