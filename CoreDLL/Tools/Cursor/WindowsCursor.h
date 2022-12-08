#ifndef WINDOWS_CURSOR_TOOL_H_DLL
#define WINDOWS_CURSOR_TOOL_H_DLL

#include "ICursor.h"
#include <Windows.h>

namespace Nk {

	CLASS_PARAMS class WindowsCursor: public ICursor {
	public:

		WindowsCursor();

		WindowsCursor(IBitmap* cursorBitmap);

		WindowsCursor(SystemCursor systemCursor);

		~WindowsCursor() override;

		CLASS_METHOD void SetCursorImage(IBitmap* image) override;

		CLASS_METHOD void SetSystemCursor(SystemCursor systemCursor) override;

		ICursor* ChooseCursor() override;

		HCURSOR GetHCursor();

	private:
		void LoadSystemCursor();

		bool m_isSystemCursor = false;
		SystemCursor m_systemCursorType;
		Rect_t m_cursorSize = { 0, 0, 30, 30 };
		IBitmap* m_cursorImage = nullptr;
		HCURSOR m_currentCursor;

		static HCURSOR currentCursorInTheSystem;
		static ICursor* currentICursorInTheSystem;
		static ICursor* defaultCursor;
	};
}

#endif	//#ifndef WINDOWS_CURSOR_TOOL_H_DLL
