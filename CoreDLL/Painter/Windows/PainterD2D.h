#ifndef PAINTER_D2D_OS_GUI_DLL
#define PAINTER_D2D_OS_GUI_DLL

#include "../IPainter.h"
#include "../Font/Windows/FontD2D.h"
#include "../Brush/Windows/BrushD2D.h"
#include <Windows.h>
#include <wrl.h>
#include <d2d1.h>
#include <vector>

namespace Nk {

	using Microsoft::WRL::ComPtr;
	
	class PainterD2D final: public IPainter {
	public:
		PainterD2D(HWND hWnd, PainterD2D* parentPainter);
		~PainterD2D() override;

		void ClearTarget(const Color_t& color) override;

		IFont* CreateFontObject() override;

		void SetFont(IFont*) override;

		void DrawText(Rect_t textRect, const std::wstring& text) override;

		void DrawBitmap(IBitmap* bitmap, Rect_t destRect) override;
		void FillRectangle(Rect_t destRect) override;


		IBrush* CreateBrushObject(const Color_t& color) override;
		void SetTextBrush(IBrush* brush) override;
		void SetBackgroundBrush(IBrush* brush) override;
		void SetContureBrush(IBrush* brush) override;

		void SetStartViewportPoint(Point_t viewportPoint) override;
		
	protected:
		void Resize(UINT32 w, UINT32 h) override;
		void CreateBuffer() override;
		bool IsValidBackBuffer() override;
		//void BeginDraw(const Rect_t& rootClientRect, const Rect_t& bitmapRect) override;
		void BeginDraw(const Rect_t& clientRect) override;
		void EndDraw() override;
		void DrawBufferBitmap(const Rect_t& clientRect) override;


	private:
		/*
		* Get the most top parent's render target
		*/
		ComPtr<ID2D1RenderTarget> GetRootParentRenderTarget();
		ComPtr<ID2D1RenderTarget> GetParentRenderTarget();
		bool IsRootBeginDrawCalled();

		/*
		* Checks if user brush was created by this painter
		*/
		bool IsPainterBrush(IBrush* userBrush);

		/*
		* Sets affine transformation to render target
		*/
		void SetNewAffineTransform();

		//Configs
		const int DPI_X = 96;
		const int DPI_Y = 96;
		HWND m_hWnd;
		PainterD2D* m_parentPainter = nullptr;
		D2D1_SIZE_U m_renderTargetSize;
		ComPtr<ID2D1HwndRenderTarget> m_hWndRenderTarget;
		ComPtr<ID2D1Bitmap> m_bufferBitmap = nullptr;
		ComPtr<ID2D1BitmapRenderTarget> m_compatibleBitmapRootRenderTarget;
		PAINTSTRUCT m_paintStructure;
		bool m_isBeginCallActive = false;

		//Drawing configs
		Rect_t m_clientRect;

		//AffineTransform + viewPort (logical zero)
		Point_t m_viewportPoint = {0, 0};
		D2D1_MATRIX_3X2_F m_affineTransformationMatrix;

		std::vector<FontD2D*> m_createdFonts;
		static FontD2D m_defaultFontObject;
		std::vector<BrushD2D*> m_createdBrushes;
		BrushD2D* m_defaultBrushObject;

		//Drawing tools
		FontD2D* m_currentFont;
		BrushD2D* m_textBrush;
		BrushD2D* m_backgroundBrush;
		BrushD2D* m_contureBrush;
	};
}

#endif	//#ifndef PAINTER_D2D_OS_GUI_DLL

