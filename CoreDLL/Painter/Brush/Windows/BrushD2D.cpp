#include "BrushD2D.h"


namespace Nk {

	BrushD2D::BrushD2D(ComPtr<ID2D1RenderTarget> renderTarget, Color_t color) : m_renderTarget{ renderTarget }, m_currentColor{color}{
		CreateColorBrush(color);
	}


	BrushD2D::~BrushD2D() {

	}


	void BrushD2D::CreateColorBrush(Color_t color){
		if (!(m_currentColor == color)) {
			D2D1_COLOR_F d2dColor{ color.r, color.g, color.b, color.a };
			m_renderTarget->CreateSolidColorBrush(d2dColor, m_colorBrush.ReleaseAndGetAddressOf());
			m_currentColor = color;
		}
		m_currentBrush = m_colorBrush;
	}


	void BrushD2D::SetColor(Color_t color) {
		CreateColorBrush(color);
	}


	ComPtr<ID2D1Brush> BrushD2D::GetD2D1Brush() {
		return m_currentBrush;
	}
}