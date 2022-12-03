#ifndef PEND2D_PAINTER_DLL
#define PEND2D_PAINTER_DLL

#include "../IPen.h"
#include <d2d1.h>
#include <wrl.h>

namespace Nk {

	using Microsoft::WRL::ComPtr;

	class PenD2D : public IPen {
	public:
		PenD2D(ComPtr<ID2D1RenderTarget> renderTarget, Color_t color);
		PenD2D();

		~PenD2D() {};

		/*
		* Set current brush color
		*/
		//virtual void SetColor(Color_t color) = 0;
	private:
		ComPtr<ID2D1RenderTarget> m_renderTarget;
		ComPtr<ID2D1StrokeStyle> m_strokeStyle;
	};

}

#endif	//#ifndef PEND2D_PAINTER_DLL

