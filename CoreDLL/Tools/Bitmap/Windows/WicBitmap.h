#ifndef WICBITMAP_TOOL_H_DLL
#define WICBITMAP_TOOL_H_DLL

#include "../IBitmap.h"
#include <d2d1.h>
#include <wrl.h>
#include <string>
#include <wincodec.h>


namespace Nk {

	class WicBitmap: public IBitmap {
	public:
		WicBitmap(Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTarget = nullptr);

		~WicBitmap() override;

	protected:

		void LoadBitmapFromFile_(const std::wstring& fileName) override;

	private:

		/*
		* Creates bitmap for needed render target
		*/
		void CreatePhisicalBitmap();

		Microsoft::WRL::ComPtr<ID2D1RenderTarget> m_renderTarget;
		Microsoft::WRL::ComPtr<IWICBitmapDecoder> m_bitmapDecoder;
		Microsoft::WRL::ComPtr<ID2D1Bitmap> m_bitmapD2D;
		std::wstring m_pictureFileName;

	};
}

#endif	//#ifndef WICBITMAP_TOOL_H_DLL
