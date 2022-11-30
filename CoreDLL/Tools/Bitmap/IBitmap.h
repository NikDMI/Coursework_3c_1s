#ifndef IBITMAP_TOOL_H_DLL
#define IBITMAP_TOOL_H_DLL

#include "../../Bean/Config.h"
#include <vector>
#include <string>

namespace Nk {
#undef LoadBitmap

	/*
	* Represent abstraction of bitmaps in different formats
	* Note: USER MUSN'T DELETE THIS OBJECTS MANUALLY
	*/
	CLASS_PARAMS class IBitmap {
	public:


		virtual ~IBitmap() {};

		/*
		* Load bitmap (factory method)
		*/
		CLASS_METHOD static IBitmap* LoadBitmap(const std::wstring& imagePath);

		/*
		* Innter method of different bitmap loaders
		*/
		CLASS_METHOD virtual void LoadBitmapFromFile (const std::wstring& fileName) = 0;

		static void FreeBitmaps();

	protected:
		IBitmap();

	private:
		static std::vector<IBitmap*> m_loadedBitmaps;

	};
}

#endif	//#ifndef IBITMAP_TOOL_H_DLL
