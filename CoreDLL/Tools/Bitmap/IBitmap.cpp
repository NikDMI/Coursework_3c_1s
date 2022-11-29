#include "IBitmap.h"
#include "../../Application/NkApplication.h"
#include "Windows/WicBitmap.h"

namespace Nk {

	std::vector<IBitmap*> IBitmap::m_loadedBitmaps;


#undef LoadBitmap

	IBitmap::IBitmap() {
		m_loadedBitmaps.push_back(this);
	}

	IBitmap* IBitmap::LoadBitmap(const std::wstring& imagePath) {
		switch (NkApplication::GetNkApplication()->GetPainterType()) {
		case PainterType::DirectX:
			return new WicBitmap{};
		}
		return nullptr;
	}


	void IBitmap::FreeBitmaps() {
		for (auto bitmap : m_loadedBitmaps) {
			delete bitmap;
		}
	}

}