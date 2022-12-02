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

		CLASS_PARAMS enum class SystemBitmaps {CLOSE_IMAGE = 0, _LAST_};

		virtual ~IBitmap() {};

		/*
		* Load bitmap (factory method)
		*/
		CLASS_METHOD static IBitmap* LoadBitmap(const std::wstring& imagePath);

		/*
		* Get system bitmaps
		*/
		CLASS_METHOD static IBitmap* GetSystemBitmap(SystemBitmaps bitmapIndex);

		/*
		* Innter method of different bitmap loaders
		*/
		CLASS_METHOD virtual void LoadBitmapFromFile (const std::wstring& fileName) = 0;

		/*
		* Load bitmap from buffer stream
		*/
		virtual void LoadBitmapFromStream(void* imageStream, long streamSize) = 0;

		/*
		* Free bitmap resources
		*/
		static void FreeBitmaps();

	protected:
		IBitmap();

	private:
		static std::vector<IBitmap*> m_loadedBitmaps;
		static std::wstring m_systemImageNames[(int)SystemBitmaps::_LAST_];
		static IBitmap* m_systemBitmaps[(int)SystemBitmaps::_LAST_];

		static const std::wstring SYSTEM_IMAGE_DIRECTORY;
	};
}

#endif	//#ifndef IBITMAP_TOOL_H_DLL
