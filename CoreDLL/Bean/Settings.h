#ifndef SETTINGS_CORE_DLL
#define SETTINGS_CORE_DLL

#include <d2d1.h>
#include <wrl.h>
#include <dwrite.h>
#include <wincodec.h>


namespace Nk {

	using Microsoft::WRL::ComPtr;
	/*
	* This class is used to init harsware settings
	*/
	class Settings {
	public:
		Settings() = delete;

		/*
		* Initialize COM 
		*/
		static void InitComInterfaces();

		static ComPtr<ID2D1Factory> GetD2D1Factory();

		static ComPtr<IDWriteFactory> GetDWriteFactory();

		static ComPtr<IWICImagingFactory> GetWicImagingFactory();

		static void Dispose();

	private:
		static bool m_isComInit;
		static ComPtr<ID2D1Factory> m_d2d1Factory;
		static ComPtr<IDWriteFactory> m_dWriteFactory;
		static ComPtr<IWICImagingFactory> m_wicImagingFactory;
	};
}

#endif	//#ifndef SETTINGS_CORE_DLL
