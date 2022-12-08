#ifndef ICURSOR_TOOL_H_DLL
#define ICURSOR_TOOL_H_DLL

#include "../../Bean/Config.h"
#include "../Bitmap/IBitmap.h"
#include <string>

namespace Nk {

	CLASS_PARAMS class ICursor {
	public:
		enum class SystemCursor {ARROW, CROSS};


		/*
		* Creates default arrow cursor
		*/
		CLASS_METHOD static ICursor* CreateCursor();

		/*
		* Reset global cursor from the system
		*/
		CLASS_METHOD static void ResetGlobalCursor();

		/*
		* Check if there is global cursor in the system
		*/
		CLASS_METHOD static bool IsGlobalCursorSet();

		/*
		* Set global cursor to the system
		*/
		CLASS_METHOD static void SetGlobalCursor(ICursor* cursor);

		CLASS_METHOD virtual void SetCursorImage(IBitmap* image) = 0;

		CLASS_METHOD virtual void SetSystemCursor(SystemCursor systemCursor) = 0;

		static ICursor* GetDefaultCursor();

		/*
		* Load this cursor as current to the system
		* Return last cursor in the system
		*/
		CLASS_METHOD virtual ICursor* ChooseCursor() = 0;

		CLASS_METHOD static Point_t GetGlobalMouseCoord();

	protected:
		ICursor() {};

		virtual ~ICursor() {};

		static ICursor* currenGlobalCursor;
	};
}

#endif	//#ifndef ICURSOR_TOOL_H_DLL
