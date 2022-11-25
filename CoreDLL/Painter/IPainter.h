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

		virtual void ClearTarget(const Color_t& color) = 0;

	protected:
		/*
		* Resize render target
		* Back buffer deleted
		*/
		virtual void Resize(UINT32 w, UINT32 h) = 0;

		/*
		* Create buffer of the window target
		*/
		virtual void CreateBuffer() = 0;

		/*
		* Checks if back buffer was created
		*/
		virtual bool IsValidBackBuffer() = 0;

		virtual void BeginDraw(Coord_t xOffset, Coord_t yOffset) = 0;

		virtual void DrawBufferBitmap(Coord_t xOffset, Coord_t yOffset) = 0;

		virtual void EndDraw() = 0;

		friend class WindowWin32;

	};
}

#endif	//#ifndef IPAINTER_OS_GUI_DLL

//У каждого окна должен быть свой связанный "художник"
