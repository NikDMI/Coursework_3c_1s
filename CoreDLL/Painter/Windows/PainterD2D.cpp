#define DLL_EXPORT_SIGNATURE

#include "PainterD2D.h"
#include "../../Bean/Settings.h"
#include "../../Bean/Exception.h"

#pragma comment(lib, "d2d1.lib")

namespace Nk {

	using namespace Microsoft::WRL;

	PainterD2D::PainterD2D(HWND hWnd) : m_hWnd{hWnd} {
		Settings::InitComInterfaces();
		RECT windowClientRect;
		GetClientRect(hWnd, &windowClientRect);
		ComPtr<ID2D1Factory> d2d1Factory = Settings::GetD2D1Factory();
		D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties = D2D1::RenderTargetProperties();
		renderTargetProperties.dpiX = DPI_X;
		renderTargetProperties.dpiY = DPI_Y;
		renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
		//renderTargetProperties.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_R32G32B32A32_FLOAT, D2D1_ALPHA_MODE_STRAIGHT);
		renderTargetProperties.usage = D2D1_RENDER_TARGET_USAGE_NONE;
		renderTargetProperties.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
		D2D1_HWND_RENDER_TARGET_PROPERTIES hWndRenderTargetProperties;
		hWndRenderTargetProperties.hwnd = hWnd;
		hWndRenderTargetProperties.pixelSize = D2D1::SizeU(windowClientRect.right - windowClientRect.left,
			windowClientRect.bottom - windowClientRect.top);
		hWndRenderTargetProperties.presentOptions = D2D1_PRESENT_OPTIONS_NONE;
		HRESULT err;
		if ((err = d2d1Factory->CreateHwndRenderTarget(renderTargetProperties, hWndRenderTargetProperties,
			m_hWndRenderTarget.GetAddressOf())) != S_OK) {
			throw Exception{ "Can't create hwnd render target" };
		}
	}


	void PainterD2D::Resize(UINT32 w, UINT32 h) {
		if (m_hWndRenderTarget->Resize({ w, h }) != S_OK) {
			throw Exception{"Can't resize hWndRenderTarget"};
		}
	}
}