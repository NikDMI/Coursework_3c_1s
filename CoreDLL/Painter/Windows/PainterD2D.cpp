#define DLL_EXPORT_SIGNATURE

#include "PainterD2D.h"
#include "../../Bean/Settings.h"
#include "../../Bean/Exception.h"
#include <fstream>

#pragma comment(lib, "d2d1.lib")

namespace Nk {

	using namespace Microsoft::WRL;

	PainterD2D::PainterD2D(HWND hWnd, PainterD2D* parentPainter) : m_hWnd{ hWnd }, m_parentPainter{parentPainter} {
		Settings::InitComInterfaces();
		RECT windowClientRect;
		GetClientRect(hWnd, &windowClientRect);
		m_renderTargetSize = D2D1::SizeU(windowClientRect.right - windowClientRect.left,
			windowClientRect.bottom - windowClientRect.top);

		if (parentPainter == nullptr) {	//Only root window can have render target
			
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
			hWndRenderTargetProperties.pixelSize = m_renderTargetSize;
			hWndRenderTargetProperties.presentOptions = D2D1_PRESENT_OPTIONS_NONE;
			HRESULT err;
			if ((err = d2d1Factory->CreateHwndRenderTarget(renderTargetProperties, hWndRenderTargetProperties,
				m_hWndRenderTarget.GetAddressOf())) != S_OK) {
				throw Exception{ "Can't create hwnd render target" };
			}
		}
	}


	ComPtr<ID2D1HwndRenderTarget> PainterD2D::GetRootParentRenderTarget() {
		if (m_parentPainter == nullptr) return m_hWndRenderTarget;
		return m_parentPainter->GetRootParentRenderTarget();
	}


	void PainterD2D::BeginDraw(Coord_t xOffset, Coord_t yOffset) {
		m_xChildOffset = xOffset;
		m_yChildOffset = yOffset;
		if (m_parentPainter && m_compatibleBitmapRootRenderTarget == nullptr) {
			CreateBuffer();
		}
		//Choose render target
		if (m_parentPainter != nullptr) {
			m_currentRenderTarget = m_compatibleBitmapRootRenderTarget;
		}
		else {
			::BeginPaint(m_hWnd, &m_paintStructure);
			m_currentRenderTarget = m_hWndRenderTarget;
		}
		m_currentRenderTarget->BeginDraw();
	}


	void PainterD2D::EndDraw() {
		m_currentRenderTarget->EndDraw();
		if (m_parentPainter) {
			if (m_bufferBitmap != nullptr) m_bufferBitmap.Reset();
			if (m_compatibleBitmapRootRenderTarget->GetBitmap(m_bufferBitmap.GetAddressOf()) != S_OK) {
				throw Exception{};
			}
			auto rootRenderTarget = GetRootParentRenderTarget();
			D2D1_RECT_F destRect = { m_xChildOffset, m_yChildOffset, m_renderTargetSize.width, m_renderTargetSize.height };
			rootRenderTarget->DrawBitmap(m_bufferBitmap.Get(), destRect);
			//rootRenderTarget->EndDraw();
		}
		else {
			m_hWndRenderTarget->EndDraw();
			::EndPaint(m_hWnd, &m_paintStructure);
		}
		
	}


	void PainterD2D::Resize(UINT32 w, UINT32 h) {
		D2D1_SIZE_U size = { w, h };
		if (m_parentPainter == nullptr) {//Root window
			if (m_hWndRenderTarget->Resize(size) != S_OK) {
				throw Exception{ "Can't resize hWndRenderTarget" };
			}
		}
		else {//Child window

		}
		m_compatibleBitmapRootRenderTarget = nullptr;
		m_renderTargetSize = size;
		m_bufferBitmap = nullptr;
	}


	void PainterD2D::CreateBuffer() {
		if (m_compatibleBitmapRootRenderTarget == nullptr) {
			ComPtr<ID2D1HwndRenderTarget> rootRenderTarget = GetRootParentRenderTarget();
			D2D_SIZE_F size{ m_renderTargetSize.height, m_renderTargetSize.width };
			if (rootRenderTarget->CreateCompatibleRenderTarget(size, m_compatibleBitmapRootRenderTarget.GetAddressOf()) != S_OK) {
				throw Exception{ "Can't create back buffer" };
			}
		}
	}


	bool PainterD2D::IsValidBackBuffer() {
		if (m_bufferBitmap == nullptr) {
			return false;
		}
		return true;
	}


	void PainterD2D::DrawBufferBitmap(Coord_t xOffset, Coord_t yOffset) {
		if (m_bufferBitmap == nullptr) {
			throw Exception{ "Back buffer is nullptr" };
		}
		auto hWndRenderTarget = this->GetRootParentRenderTarget();
		hWndRenderTarget->BeginDraw();
		D2D1_RECT_F bmpRect = { xOffset, yOffset, m_renderTargetSize.width, m_renderTargetSize.height };
		hWndRenderTarget->DrawBitmap(m_bufferBitmap.Get(), bmpRect);
		hWndRenderTarget->EndDraw();
	}

	///////////////////////////////// GRPHICS METHODS

	void PainterD2D::ClearTarget(const Color_t& color) {
		m_currentRenderTarget->Clear({color.r, color.g, color.b, color.a});
	}

}