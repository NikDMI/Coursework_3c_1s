#ifndef IBITMAP_TOOL_H_DLL
#define IBITMAP_TOOL_H_DLL

#include "../../Bean/Config.h"
#include <vector>
#include <string>

namespace Nk {

	/*
	* Represent abstraction of bitmaps in different formats
	* Note: USER MUSN'T DELETE THIS OBJECTS MANUALLY
	*/
	CLASS_PARAMS class IBitmap {
	public:

		IBitmap();

		virtual ~IBitmap() {};

		/*
		* Load bitmap (factory method)
		*/
		CLASS_METHOD static IBitmap* LoadBitmap(const std::wstring& imagePath);

		static void FreeBitmaps();

	protected:

		/*
		* Innter method of different bitmap loaders
		*/
		virtual void LoadBitmapFromFile_(const std::wstring& fileName) = 0;

	private:
		static std::vector<IBitmap*> m_loadedBitmaps;

	};
}

#endif	//#ifndef IBITMAP_TOOL_H_DLL
