#include "ICursor.h"
#include <Windows.h>
#include "../../Application/NkApplication.h"
#include "../../Bean/Exception.h"
#include "WindowsCursor.h"

namespace Nk {


	ICursor* ICursor::currenGlobalCursor = nullptr;
	ICursor* defaultCursorWin = new WindowsCursor{ ICursor::SystemCursor::ARROW };


	ICursor* ICursor::CreateCursor() {
		switch (NkApplication::GetNkApplication()->GetOsType()) {
		case OsType::Windows:
			return new WindowsCursor{};
		default:
			throw Exception{"Can't support this type of OS"};
		}
	}


	/*
	* Reset global cursor from the system
	*/
	void ICursor::ResetGlobalCursor() {
		currenGlobalCursor = nullptr;
	}


	/*
	* Set cursor to the system
	*/
	void SetCursor(ICursor* cursor) {

	}

	/*
	* Check if there is global cursor in the system
	*/
	bool ICursor::IsGlobalCursorSet() {
		if (currenGlobalCursor) {
			return true;
		}
		else {
			return false;
		}
	}

	/*
	* Set global cursor to the system
	*/
	void ICursor::SetGlobalCursor(ICursor* cursor) {
		currenGlobalCursor = nullptr;
		cursor->ChooseCursor();
		currenGlobalCursor = cursor;
	}


	Point_t ICursor::GetGlobalMouseCoord() {
		//Now only for win32
		POINT p;
		GetCursorPos(&p);
		return { (float)p.x, (float)p.y };
	}


	ICursor* ICursor::GetDefaultCursor() {
		switch (NkApplication::GetNkApplication()->GetOsType()) {
		case OsType::Windows:
			return defaultCursorWin;
		default:
			throw Exception{ "Can't support this type of OS" };
		}
	}

}