#ifndef WICBITMAP_TOOL_H_DLL
#define WICBITMAP_TOOL_H_DLL

#include "../IBitmap.h"
#include <d2d1.h>
#include <wrl.h>
#include <string>
#include <wincodec.h>
#include <map>


namespace Nk {

	class WicBitmap: public IBitmap {
	public:
		WicBitmap(Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTarget = nullptr);

		~WicBitmap() override;

		/*
		* Returns bitmap to correspondence d2d1RenderTarget
		*/
		Microsoft::WRL::ComPtr<ID2D1Bitmap> GetID2D1Bitmap(Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTarget);

		CLASS_METHOD void LoadBitmapFromFile (const std::wstring& fileName) override;

		/*
		* Load bitmap from buffer stream
		*/
		void LoadBitmapFromStream(void* imageStream, long streamSize) override;

	private:

		/*
		* Creates bitmap for needed render target
		*/
		void CreatePhisicalBitmap();

		Microsoft::WRL::ComPtr<ID2D1RenderTarget> m_renderTarget;
		Microsoft::WRL::ComPtr<IWICBitmapDecoder> m_bitmapDecoder;
		Microsoft::WRL::ComPtr<ID2D1Bitmap> m_bitmapD2D;
		std::wstring m_pictureFileName;

		/*
		* This map create correspondece between render target and phisical bitmap
		*/
		std::map<ID2D1RenderTarget*, Microsoft::WRL::ComPtr<ID2D1Bitmap>> m_correspondenceBitmaps;

	};
}

#endif	//#ifndef WICBITMAP_TOOL_H_DLL
