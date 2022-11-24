#ifndef PAINTER_D2D_OS_GUI_DLL
#define PAINTER_D2D_OS_GUI_DLL

#include "../IPainter.h"
#include <Windows.h>
#include <wrl.h>
#include <d2d1.h>

namespace Nk {

	using Microsoft::WRL::ComPtr;
	
	class PainterD2D final: public IPainter {
	public:
		PainterD2D(HWND hWnd);

		void Resize(UINT32 w, UINT32 h) override;
		//DrawLine
		//DrawEllipse
		//....
	private:
		const int DPI_X = 96;
		const int DPI_Y = 96;
		HWND m_hWnd;
		ComPtr<ID2D1HwndRenderTarget> m_hWndRenderTarget;
	};
}

#endif	//#ifndef PAINTER_D2D_OS_GUI_DLL

