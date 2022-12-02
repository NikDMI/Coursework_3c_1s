#include "IBitmap.h"
#include "../../Application/NkApplication.h"
#include "Windows/WicBitmap.h"
#include "../../Bean/Exception.h"
#include <string>

namespace Nk {

	std::vector<IBitmap*> IBitmap::m_loadedBitmaps;

	std::wstring IBitmap::m_systemImageNames[(int)SystemBitmaps::_LAST_] =
	{L"Core_CloseIcon.png"};

	IBitmap* IBitmap::m_systemBitmaps[(int)SystemBitmaps::_LAST_];

	const std::wstring IBitmap::SYSTEM_IMAGE_DIRECTORY = L"SystemImages\\";

#undef LoadBitmap

	IBitmap::IBitmap() {
		m_loadedBitmaps.push_back(this);
	}

	IBitmap* IBitmap::LoadBitmap(const std::wstring& imagePath) {
		IBitmap* createdBitmap = nullptr;
		switch (NkApplication::GetNkApplication()->GetPainterType()) {
		case PainterType::DirectX:
			createdBitmap = new WicBitmap{};
		}
		createdBitmap->LoadBitmapFromFile(imagePath);
		return createdBitmap;
	}


	 IBitmap* IBitmap::GetSystemBitmap(SystemBitmaps bitmapIndex) {
		 if ((int)bitmapIndex < 0 || (int)bitmapIndex >= (int)SystemBitmaps::_LAST_){
			 throw Exception{ "Invalid index" };
		 }
		 if (m_systemBitmaps[(int)bitmapIndex] == nullptr) {
			 //Load bitmap from system directory
			 m_systemBitmaps[(int)bitmapIndex] = LoadBitmap(SYSTEM_IMAGE_DIRECTORY + m_systemImageNames[(int)bitmapIndex]);
		 }
		 return m_systemBitmaps[(int)bitmapIndex];
	 }


	void IBitmap::FreeBitmaps() {
		for (auto bitmap : m_loadedBitmaps) {
			delete bitmap;
		}
	}

}