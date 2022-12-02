#include "WicBitmap.h"
#include <wincodec.h>
#include <wrl.h>
#include "../../../Bean/Settings.h"
#include "../../../Bean/Exception.h"

namespace Nk {

	using Microsoft::WRL::ComPtr;


	WicBitmap::WicBitmap(ComPtr<ID2D1RenderTarget> renderTarget) : m_renderTarget{renderTarget} {
		//LoadBitmapFromFile(fileName);
	}

	WicBitmap::~WicBitmap() {

	}


	void WicBitmap::LoadBitmapFromFile(const std::wstring& fileName) {
		if (fileName == m_pictureFileName) {
			return;
		}
		m_bitmapD2D = nullptr;
		m_correspondenceBitmaps.clear();	//clear cache of bitmaps
		m_pictureFileName = fileName;
		ComPtr<IWICImagingFactory> imagingFactory = Settings::GetWicImagingFactory();
		//Create image decoder
		HRESULT hr = imagingFactory->CreateDecoderFromFilename(fileName.c_str(), NULL, GENERIC_READ,
			WICDecodeMetadataCacheOnLoad, m_bitmapDecoder.GetAddressOf());
		if (!SUCCEEDED(hr)) {
			throw Exception {"Can't create image decoder"};
		}
	}


	void WicBitmap::LoadBitmapFromStream(void* imageStream, long streamSize) {
		m_pictureFileName = L"";
		m_bitmapD2D = nullptr;
		m_correspondenceBitmaps.clear();	//clear cache of bitmaps
		//Create wicStream
		ComPtr<IWICImagingFactory> wicFactory = Settings::GetWicImagingFactory();
		ComPtr<IWICStream> wicStream;
		HRESULT hr = wicFactory->CreateStream(wicStream.GetAddressOf());
		if (!SUCCEEDED(hr)) {
			throw Exception {"Can't create wicStream"};
		}
		//Init stream from memory
		hr = wicStream->InitializeFromMemory((WICInProcPointer)imageStream, streamSize);
		if (!SUCCEEDED(hr)) {
			throw Exception{ "Can't read image stream" };
		}
		hr = wicFactory->CreateDecoderFromStream(wicStream.Get(), NULL, WICDecodeMetadataCacheOnLoad, m_bitmapDecoder.GetAddressOf());
		if (!SUCCEEDED(hr)) {
			throw Exception{ "Can't create bitmap decoder" };
		}
	}


	ComPtr<ID2D1Bitmap> WicBitmap::GetID2D1Bitmap(ComPtr<ID2D1RenderTarget> renderTarget) {
		if (m_renderTarget == renderTarget && m_bitmapD2D != nullptr) {
			return m_bitmapD2D;
		}
		m_renderTarget = renderTarget;
		if (m_bitmapD2D == nullptr) {//If render targert wasn't set later
			CreatePhisicalBitmap();
			m_correspondenceBitmaps.insert({ renderTarget.Get(), m_bitmapD2D });
		}
		else {
			//Try get bitmap from cache
			auto iterBitmap = m_correspondenceBitmaps.find(renderTarget.Get());
			if (iterBitmap != m_correspondenceBitmaps.end()) {
				m_bitmapD2D = (*iterBitmap).second;
			}
			else {
				CreatePhisicalBitmap();
				m_correspondenceBitmaps.insert({ renderTarget.Get(), m_bitmapD2D });
			}
		}
		return m_bitmapD2D;
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