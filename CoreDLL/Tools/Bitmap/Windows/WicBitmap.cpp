#include "WicBitmap.h"
#include <wincodec.h>
#include <wrl.h>
#include "../../../Bean/Settings.h"
#include "../../../Bean/Exception.h"

namespace Nk {

	using Microsoft::WRL::ComPtr;


	WicBitmap::WicBitmap(ComPtr<ID2D1RenderTarget> renderTarget) : m_renderTarget{renderTarget} {

	}

	WicBitmap::~WicBitmap() {

	}


	void WicBitmap::LoadBitmapFromFile_(const std::wstring& fileName) {
		if (fileName == m_pictureFileName) {
			return;
		}
		ComPtr<IWICImagingFactory> imagingFactory = Settings::GetWicImagingFactory();
		//ComPtr<IWICBitmapDecoder> bmpDecoder;
		//Create image decoder
		HRESULT hr = imagingFactory->CreateDecoderFromFilename(fileName.c_str(), NULL, GENERIC_READ,
			WICDecodeMetadataCacheOnLoad, m_bitmapDecoder.GetAddressOf());
		if (!SUCCEEDED(hr)) {
			throw Exception {"Can't create image decoder"};
		}
	}


	void WicBitmap::CreatePhisicalBitmap() {
		if (m_renderTarget == nullptr || m_bitmapDecoder == nullptr) {
			throw Exception{ "Bitmap can't be created" };
		}
		ComPtr<IWICImagingFactory> imagingFactory = Settings::GetWicImagingFactory();
		ComPtr<IWICBitmapFrameDecode> pSource;
		//Get image frame
		HRESULT hr = m_bitmapDecoder->GetFrame(0, pSource.GetAddressOf());
		if (!SUCCEEDED(hr)) {
			throw Exception{ "Can't get image frame" };
		}
		//Get format converter
		ComPtr<IWICFormatConverter> pConverter;
		hr = imagingFactory->CreateFormatConverter(pConverter.GetAddressOf());
		if (!SUCCEEDED(hr)) {
			throw Exception{ "Can't create image converter" };
		}
		hr = pConverter->Initialize(pSource.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone,
			NULL, 0, WICBitmapPaletteTypeMedianCut);
		if (!SUCCEEDED(hr)) {
			throw Exception{ "Can't initialize image converter" };
		}
		hr = m_renderTarget->CreateBitmapFromWicBitmap(pConverter.Get(), NULL, m_bitmapD2D.GetAddressOf());
		if (!SUCCEEDED(hr)) {
			throw Exception{ "Can't load bitmap" };
		}
	}
}