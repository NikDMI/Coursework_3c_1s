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
		PainterD2D(HWND hWnd, PainterD2D* parentPainter);

		void ClearTarget(const Color_t& color) override;
		
	protected:
		void Resize(UINT32 w, UINT32 h) override;
		void CreateBuffer() override;
		bool IsValidBackBuffer() override;
		void BeginDraw(Coord_t xOffset, Coord_t yOffset) override;
		void EndDraw() override;
		void DrawBufferBitmap(Coord_t xOffset, Coord_t yOffset) override;


	private:
		/*
		* Get the most top parent's render target
		*/
		ComPtr<ID2D1HwndRenderTarget> GetRootParentRenderTarget();

		const int DPI_X = 96;
		const int DPI_Y = 96;
		HWND m_hWnd;
		PainterD2D* m_parentPainter = nullptr;
		D2D1_SIZE_U m_renderTargetSize;
		ComPtr<ID2D1HwndRenderTarget> m_hWndRenderTarget;
		ComPtr<ID2D1Bitmap> m_bufferBitmap = nullptr;
		ComPtr<ID2D1BitmapRenderTarget> m_compatibleBitmapRootRenderTarget;
		PAINTSTRUCT m_paintStructure;

		Coord_t m_xChildOffset;
		Coord_t m_yChildOffset;
		ComPtr<ID2D1RenderTarget> m_currentRenderTarget;

	};
}

#endif	//#ifndef PAINTER_D2D_OS_GUI_DLL

