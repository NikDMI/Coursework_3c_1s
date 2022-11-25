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


	ComPtr<ID2D1RenderTarget> PainterD2D::GetRootParentRenderTarget() {
		if (m_parentPainter == nullptr) {
			CreateBuffer();
			return m_compatibleBitmapRootRenderTarget;
		}
		return m_parentPainter->GetRootParentRenderTarget();
	}


	void PainterD2D::BeginDraw(Coord_t xOffset, Coord_t yOffset) {
		m_xChildOffset = xOffset;
		m_yChildOffset = yOffset;
		if (m_compatibleBitmapRootRenderTarget == nullptr) {
			CreateBuffer();
		}
		//Choose render target
		//m_currentRenderTarget = m_compatibleBitmapRootRenderTarget;
		if (m_parentPainter == nullptr) {
			::BeginPaint(m_hWnd, &m_paintStructure);
		}
		m_compatibleBitmapRootRenderTarget->BeginDraw();
	}


	void PainterD2D::EndDraw() {
		m_compatibleBitmapRootRenderTarget->EndDraw();
		if (m_compatibleBitmapRootRenderTarget->GetBitmap(m_bufferBitmap.GetAddressOf()) != S_OK) {
			throw Exception{};
		}
		if (m_parentPainter) {
			auto rootRenderTarget = GetRootParentRenderTarget();
			D2D1_RECT_F destRect = { m_xChildOffset, m_yChildOffset, m_xChildOffset + m_renderTargetSize.width, m_yChildOffset + m_renderTargetSize.height };
			rootRenderTarget->DrawBitmap(m_bufferBitmap.Get(), destRect);
		}
		else {
			m_hWndRenderTarget->BeginDraw();
			D2D1_RECT_F destRect = { m_xChildOffset, m_yChildOffset, m_xChildOffset + m_renderTargetSize.width, m_yChildOffset + m_renderTargetSize.height };
			m_hWndRenderTarget->DrawBitmap(m_bufferBitmap.Get(), destRect);
			m_hWndRenderTarget->EndDraw();
			::EndPaint(m_hWnd, &m_paintStructure);
		}
		
	}


	void PainterD2D::Resize(UINT32 w, UINT32 h) {
		RECT windowClientRect;
		GetClientRect(m_hWnd, &windowClientRect);
		D2D1_SIZE_U size = { windowClientRect.right - windowClientRect.left, windowClientRect.bottom - windowClientRect.top };

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
			ComPtr<ID2D1RenderTarget> rootRenderTarget;
			if (m_parentPainter != nullptr) {
				rootRenderTarget = GetRootParentRenderTarget();
			}
			else {
				rootRenderTarget = m_hWndRenderTarget;
			}
			D2D_SIZE_F size{ m_renderTargetSize.width, m_renderTargetSize.height };
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
		m_compatibleBitmapRootRenderTarget->Clear({color.r, color.g, color.b, color.a});
	}

}