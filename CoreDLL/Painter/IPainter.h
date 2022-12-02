#ifndef IPAINTER_OS_GUI_DLL
#define IPAINTER_OS_GUI_DLL

#include "../Bean/Config.h"
#include "Font/IFont.h"
#include "Brush/IBrush.h"
#include <Windows.h>
#include "../Application/NkApplication.h"
#include "../Tools/Bitmap/IBitmap.h"

namespace Nk {

#undef DrawText

	
	//enum class PainterType 

	/*
	* This interface reperesents abstraction of phisical painter device in different OS
	*/
	CLASS_PARAMS class IPainter {
	public:
		virtual ~IPainter() {};

		CLASS_METHOD virtual void ClearTarget(const Color_t& color) = 0;

		/*
		* Give pointer of corresponding font interface to user
		* User mustn't delete this object by delete operator
		* All created fonts by painter deleted at the end of the program
		*/
		CLASS_METHOD virtual IFont* CreateFontObject() = 0;

		/*
		* Set current font to the painter
		* Note: user can set only font, that created by this painter
		*/
		CLASS_METHOD virtual void SetFont(IFont*) = 0;

		/*
		* Display text in corresponding recrangle
		* All text settings was configured in the font object
		*/
		CLASS_METHOD virtual void DrawText(Rect_t textRect, const std::wstring& text) = 0;


		/*
		* Returns pointer to the brush object
		* User musn't delete this object manually
		*/
		CLASS_METHOD virtual IBrush* CreateBrushObject(const Color_t& color) = 0;


		/*
		* Set brush, that can be used when draw text
		*/
		CLASS_METHOD virtual void SetTextBrush(IBrush* brush) = 0;


		/*
		* Set brush, that can be used while drawing background layer
		*/
		CLASS_METHOD virtual void SetBackgroundBrush(IBrush* brush) = 0;


		/*
		* Set brush, that can be used while drawing conture elements
		*/
		CLASS_METHOD virtual void SetContureBrush(IBrush* brush) = 0;


		/*
		* Draws specialized bitmap in the rectangle
		*/
		CLASS_METHOD virtual void DrawBitmap(IBitmap* bitmap, Rect_t destRect) = 0;


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

		//virtual void BeginDraw(const Rect_t& rootClientRect, const Rect_t& bitmapRect) = 0;
		virtual void BeginDraw(const Rect_t& clientRect) = 0;

		//virtual void DrawBufferBitmap(const Rect_t& rootClientRect, const Rect_t& bitmapRect) = 0;
		virtual void DrawBufferBitmap(const Rect_t& clientRect) = 0;

		virtual void EndDraw() = 0;

		friend class WindowWin32;

	};
}

#endif	//#ifndef IPAINTER_OS_GUI_DLL

//У каждого окна должен быть свой связанный "художник"
