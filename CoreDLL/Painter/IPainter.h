#ifndef IPAINTER_OS_GUI_DLL
#define IPAINTER_OS_GUI_DLL

#include "../Bean/Config.h"
#include <Windows.h>

namespace Nk {
	/*
	* Types of painters (hardware dependency)
	*/
	enum class PainterType { Common, DirectX };

	/*
	* This interface reperesents abstraction of phisical painter device in different OS
	*/
	class IPainter {
	public:
		virtual ~IPainter() {};

		virtual void Resize(UINT32 w, UINT32 h) = 0;
		//DrawLine
		//DrawEllipse
		//....
	};
}

#endif	//#ifndef IPAINTER_OS_GUI_DLL

//У каждого окна должен быть свой связанный "художник"
