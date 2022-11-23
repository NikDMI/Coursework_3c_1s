#include "Settings.h"
#include "Exception.h"
#include <wrl.h>

namespace Nk {
	bool Settings::m_isComInit = false;
	ComPtr<ID2D1Factory> Settings::m_d2d1Factory;


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