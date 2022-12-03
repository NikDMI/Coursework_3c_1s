#include "PenD2D.h"
#include "../../../Bean/Settings.h"

namespace Nk {

	PenD2D::PenD2D(ComPtr<ID2D1RenderTarget> renderTarget, Color_t color) : m_renderTarget{renderTarget} {
		//Settings::GetD2D1Factory()->
	}
}