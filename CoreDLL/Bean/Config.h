#ifndef CONFIG_CORE_DLL
	#define CONFIG_CORE_DLL

	#ifdef DLL_EXPORT_SIGNATURE
		#define METHOD_SIGNATURE extern "C" __declspec(dllexport)
		#define CLASS_PARAMS __declspec(dllexport)
		#define CLASS_METHOD __declspec(dllexport)
	#else
		#define METHOD_SIGNATURE extern "C" __declspec(dllimport)
		#define CLASS_PARAMS __declspec(dllimport)
		#define CLASS_METHOD __declspec(dllimport)
	#endif // DLL_EXPORT_SIGNATURE

	#define PROC_CALL __cdecl

namespace Nk {
	using Coord_t = float;	//Describes pixels values
}

#endif	//#ifndef CONFIG_CORE_DLL
