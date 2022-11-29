#include "Settings.h"
#include "Exception.h"
#include <wrl.h>

namespace Nk {
	bool Settings::m_isComInit = false;
	ComPtr<ID2D1Factory> Settings::m_d2d1Factory;
	ComPtr<IDWriteFactory> Settings::m_dWriteFactory;
	ComPtr<IWICImagingFactory> Settings::m_wicImagingFactory;


	void Settings::InitComInterfaces() {
		if (!m_isComInit) {
			if (CoInitialize(NULL) != S_OK) {
				throw Exception{ "Can't initialize COM library" };
			}
			m_isComInit = true;
		}
	}


	ComPtr<ID2D1Factory> Settings::GetD2D1Factory() {
		if (m_d2d1Factory == nullptr) {
			if (D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_PPV_ARGS(m_d2d1Factory.GetAddressOf())) != S_OK) {
				throw Exception{ "Can't create D2D1Factory" };
			}
		}
		return m_d2d1Factory;
	}


	ComPtr<IDWriteFactory> Settings::GetDWriteFactory() {
		if (m_dWriteFactory == nullptr) {
			HRESULT hRes = DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown**>(m_dWriteFactory.GetAddressOf())
			);
			if (hRes != S_OK) {
				throw Exception{ "Can't create IDWriteFactory" };
			}
		}
		return m_dWriteFactory;
	}


	ComPtr<IWICImagingFactory> Settings::GetWicImagingFactory() {
		if (m_wicImagingFactory == nullptr) {
			HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(m_wicImagingFactory.GetAddressOf()));
			if (!SUCCEEDED(hr)) {
				throw Exception{ "WICFactory: bad alloc" };
			}
		}
		return m_wicImagingFactory;
	}


	void Settings::Dispose() {
		if (m_isComInit) {
			CoUninitialize();
			m_isComInit = false;
		}
		if (m_d2d1Factory != nullptr) {
			m_d2d1Factory = nullptr;
		}
	}
}