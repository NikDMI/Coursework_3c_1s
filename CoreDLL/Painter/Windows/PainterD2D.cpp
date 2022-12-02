#define DLL_EXPORT_SIGNATURE

#include "PainterD2D.h"
#include "../../Bean/Settings.h"
#include "../../Bean/Exception.h"
#include "../Font/Windows/FontD2D.h"
#include "../../Tools/Bitmap/Windows/WicBitmap.h"
#include <algorithm>
#include <fstream>

#pragma comment(lib, "d2d1.lib")

namespace Nk {

	using namespace Microsoft::WRL;

	FontD2D PainterD2D::m_defaultFontObject;
	//BrushD2D PainterD2D::m_defaultBrushObject;


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
			//renderTargetProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_STRAIGHT;
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
		else {
			m_hWndRenderTarget = parentPainter->m_hWndRenderTarget;
		}
		m_currentFont = &this->m_defaultFontObject;
		//Brushes
		m_defaultBrushObject = new BrushD2D{ m_hWndRenderTarget, {0,0,0,1} };
		m_textBrush = m_defaultBrushObject;
		m_backgroundBrush = m_defaultBrushObject;
		m_contureBrush = m_defaultBrushObject;
	}


	PainterD2D::~PainterD2D() {
		for (IFont* font : m_createdFonts) {
			if (font != &m_defaultFontObject) delete font;
		}
		for (IBrush* brush : m_createdBrushes) {
			if(brush != m_defaultBrushObject) delete brush;
		}
		delete m_defaultBrushObject;
	}

	////////////////////////////////////////////////////   SYSTEM METHODS


	IFont* PainterD2D::CreateFontObject() {
		FontD2D* newFont = new FontD2D{};
		m_createdFonts.push_back(newFont);
		return newFont;
	}


	void PainterD2D::SetFont(IFont* registeredFont) {
		//Check if this font was registered by this painter
		auto findIter = std::find(m_createdFonts.begin(), m_createdFonts.end(), registeredFont);
		if (findIter == m_createdFonts.end()) {
			throw Exception{"This font is not registered by this painter"};
		}
		m_currentFont = (FontD2D*)registeredFont;
	}


	IBrush* PainterD2D::CreateBrushObject(const Color_t& color) {
		BrushD2D* userBrush = new BrushD2D{ m_hWndRenderTarget, color };
		m_createdBrushes.push_back(userBrush);
		return userBrush;
	}


	bool PainterD2D::IsPainterBrush(IBrush* userBrush) {
		auto brushIter = std::find(m_createdBrushes.begin(), m_createdBrushes.end(), userBrush);
		if (brushIter == m_createdBrushes.end()) {
			throw Exception{"Brush was created by another painter"};
		}
	}

	void PainterD2D::SetTextBrush(IBrush* brush) {
		IsPainterBrush(brush);
		m_textBrush = (BrushD2D*)brush;
	}


	void PainterD2D::SetBackgroundBrush(IBrush* brush) {
		IsPainterBrush(brush);
		m_backgroundBrush = (BrushD2D*)brush;
	}


	void PainterD2D::SetContureBrush(IBrush* brush) {
		IsPainterBrush(brush);
		m_contureBrush = (BrushD2D*)brush;
	}


	////////////////////////////////////////////////////  CONFIG DRAWING METHODS

	ComPtr<ID2D1RenderTarget> PainterD2D::GetRootParentRenderTarget() {
		if (m_parentPainter == nullptr) {
			CreateBuffer();
			return m_compatibleBitmapRootRenderTarget;
		}
		return m_parentPainter->GetRootParentRenderTarget();
	}


	ComPtr<ID2D1RenderTarget> PainterD2D::GetParentRenderTarget() {
		return m_parentPainter->m_compatibleBitmapRootRenderTarget;
	}


	/*
	void PainterD2D::BeginDraw(const Rect_t& rootClientRect, const Rect_t& bitmapRect) {
		m_rootClientRect = rootClientRect;
		m_bitmapRect = bitmapRect;
		if (m_compatibleBitmapRootRenderTarget == nullptr) {
			CreateBuffer();
		}
		//Choose render target
		if (m_parentPainter == nullptr) {
			::BeginPaint(m_hWnd, &m_paintStructure);
		}
		m_compatibleBitmapRootRenderTarget->BeginDraw();
		m_isBeginCallActive = true;
	}
	*/

	void PainterD2D::BeginDraw(const Rect_t& clientRect) {
		m_clientRect = clientRect;
		if (m_compatibleBitmapRootRenderTarget == nullptr) {
			CreateBuffer();
		}
		//Choose render target
		if (m_parentPainter == nullptr) {
			::BeginPaint(m_hWnd, &m_paintStructure);
		}
		m_compatibleBitmapRootRenderTarget->BeginDraw();
		m_isBeginCallActive = true;
	}


	void PainterD2D::EndDraw() {
		m_compatibleBitmapRootRenderTarget->EndDraw();
		if (m_compatibleBitmapRootRenderTarget->GetBitmap(m_bufferBitmap.ReleaseAndGetAddressOf()) != S_OK) {
			throw Exception{};
		}
		if (m_parentPainter) {	//Child control
			//auto rootRenderTarget = GetRootParentRenderTarget();
			auto rootRenderTarget = GetParentRenderTarget();
			//D2D1_RECT_F destRect = { m_rootClientRect.x, m_rootClientRect.y, m_rootClientRect.x + m_rootClientRect.w, m_rootClientRect.y + m_rootClientRect.h };
			D2D1_RECT_F destRect = { m_clientRect.x, m_clientRect.y, m_clientRect.x + m_clientRect.w, m_clientRect.y + m_clientRect.h };
			D2D1_RECT_F bmpRect = { 0, 0, m_renderTargetSize.width, m_renderTargetSize.height };
			//D2D1_RECT_F bmpRect = { m_bitmapRect.x, m_bitmapRect.y, m_bitmapRect.x + m_bitmapRect.w, m_bitmapRect.y + m_bitmapRect.h };
			rootRenderTarget->DrawBitmap(m_bufferBitmap.Get(), destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, bmpRect);
		}
		else {
			m_hWndRenderTarget->BeginDraw();
			//D2D1_RECT_F destRect = { m_clipRect.x, m_clipRect.y, m_clipRect.w, m_clipRect.h };
			D2D1_SIZE_F bufSize = m_bufferBitmap->GetSize();
			D2D1_RECT_F bufRect = { 0, 0, bufSize.width, bufSize.height };
			//D2D1_RECT_F destRect = { m_rootClientRect.x, m_rootClientRect.y, m_rootClientRect.x + m_rootClientRect.w, m_rootClientRect.y + m_rootClientRect.h };
			//D2D1_RECT_F bmpRect = { m_bitmapRect.x, m_bitmapRect.y, m_bitmapRect.x + m_bitmapRect.w, m_bitmapRect.y + m_bitmapRect.h };
			//m_hWndRenderTarget->DrawBitmap(m_bufferBitmap.Get(), destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, bmpRect);
			m_hWndRenderTarget->DrawBitmap(m_bufferBitmap.Get(), bufRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, bufRect);
			m_hWndRenderTarget->EndDraw();
			::EndPaint(m_hWnd, &m_paintStructure);
		}
		m_isBeginCallActive = false;
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
				//rootRenderTarget = GetRootParentRenderTarget();
				rootRenderTarget = GetParentRenderTarget();
			}
			else {
				rootRenderTarget = m_hWndRenderTarget;
			}
			D2D_SIZE_F size{ m_renderTargetSize.width, m_renderTargetSize.height };
			if (rootRenderTarget->CreateCompatibleRenderTarget(size, m_compatibleBitmapRootRenderTarget.ReleaseAndGetAddressOf()) != S_OK) {
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


	/*
	void PainterD2D::DrawBufferBitmap(const Rect_t& rootClientRect, const Rect_t& bitmapRect) {
		if (m_bufferBitmap == nullptr) {
			throw Exception{ "Back buffer is nullptr" };
		}
		auto hWndRenderTarget = this->GetRootParentRenderTarget();
		bool isBeginDrawCalled = this->IsRootBeginDrawCalled();
		if (!isBeginDrawCalled) hWndRenderTarget->BeginDraw();
		//D2D1_RECT_F bmpRect = { clipRect.x, clipRect.y, clipRect.w, clipRect.h };
		D2D1_RECT_F destRect = { m_rootClientRect.x, m_rootClientRect.y, m_rootClientRect.x + m_rootClientRect.w, m_rootClientRect.y + m_rootClientRect.h };
		D2D1_RECT_F bmpRect = { m_bitmapRect.x, m_bitmapRect.y, m_bitmapRect.x + m_bitmapRect.w, m_bitmapRect.y + m_bitmapRect.h };
		//hWndRenderTarget->DrawBitmap(m_bufferBitmap.Get(), bmpRect);
		hWndRenderTarget->DrawBitmap(m_bufferBitmap.Get(), destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, bmpRect);
		if (!isBeginDrawCalled) hWndRenderTarget->EndDraw();
	}
	*/


	void PainterD2D::DrawBufferBitmap(const Rect_t& clientRect) {
		if (m_bufferBitmap == nullptr) {
			throw Exception{ "Back buffer is nullptr" };
		}
		auto hWndRenderTarget = this->GetRootParentRenderTarget();
		bool isBeginDrawCalled = this->IsRootBeginDrawCalled();
		if (!isBeginDrawCalled) hWndRenderTarget->BeginDraw();
		//D2D1_RECT_F bmpRect = { clipRect.x, clipRect.y, clipRect.w, clipRect.h };
		D2D1_RECT_F destRect = { clientRect.x, clientRect.y, clientRect.x + clientRect.w, clientRect.y + clientRect.h };
		D2D1_RECT_F bmpRect = { 0, 0, m_renderTargetSize.width, m_renderTargetSize.height };
		//D2D1_RECT_F bmpRect = { m_bitmapRect.x, m_bitmapRect.y, m_bitmapRect.x + m_bitmapRect.w, m_bitmapRect.y + m_bitmapRect.h };
		//hWndRenderTarget->DrawBitmap(m_bufferBitmap.Get(), bmpRect);
		hWndRenderTarget->DrawBitmap(m_bufferBitmap.Get(), destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, bmpRect);
		if (!isBeginDrawCalled) hWndRenderTarget->EndDraw();
	}


	bool PainterD2D::IsRootBeginDrawCalled() {
		if (m_parentPainter) {
			return m_parentPainter->IsRootBeginDrawCalled();
		}
		else {
			return m_isBeginCallActive;
		}
	}

	///////////////////////////////// GRPHICS METHODS

	void PainterD2D::ClearTarget(const Color_t& color) {
		m_compatibleBitmapRootRenderTarget->Clear({color.r, color.g, color.b, color.a});
	}

#undef DrawText;

	void PainterD2D::DrawText(Rect_t textRect, const std::wstring& text) {
		auto textLayout = m_currentFont->GetFormattedTextLayout(text, {textRect.x, textRect.y, textRect.x + textRect.w, textRect.y + textRect.h});
		//ID2D1Brush* br = 
		m_compatibleBitmapRootRenderTarget->DrawTextLayout({ textRect.x, textRect.y }, textLayout.Get(), m_textBrush->GetD2D1Brush().Get());
	}


	void PainterD2D::DrawBitmap(IBitmap* bitmap, Rect_t destRect) {
		//IBitmap will be 100% WicBitmap type
		WicBitmap* wicBitmap = (WicBitmap*)bitmap;
		ComPtr<ID2D1Bitmap> d2d1Bitmap = wicBitmap->GetID2D1Bitmap(m_compatibleBitmapRootRenderTarget);//???
		D2D1_RECT_F destRectD2D = { destRect.x, destRect.y, destRect.x + destRect.w, destRect.y + destRect.h };
		m_compatibleBitmapRootRenderTarget->DrawBitmap(d2d1Bitmap.Get(), destRectD2D);
	}

}