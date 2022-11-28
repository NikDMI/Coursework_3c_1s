#ifndef BRUSH_D2D_PAINTER_DLL
#define BRUSH_D2D_PAINTER_DLL

#include "../IBrush.h"
#include <d2d1.h>
#include <wrl.h>


namespace Nk {

	using Microsoft::WRL::ComPtr;

	class BrushD2D: public IBrush {
	public:
		BrushD2D(ComPtr<ID2D1RenderTarget> renderTarget, Color_t color);
		~BrushD2D() override;
		void SetColor(Color_t color) override;

	private:
		void CreateColorBrush(Color_t color);

		ComPtr<ID2D1RenderTarget> m_renderTarget;
		Color_t m_currentColor;
		ComPtr<ID2D1Brush> m_currentBrush;
		ComPtr<ID2D1SolidColorBrush> m_colorBrush;

	};

}

#endif	//#ifndef IBRUSH_PAINTER_DLL

