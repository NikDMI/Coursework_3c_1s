#ifndef CONFIG_CORE_DLL
	#define CONFIG_CORE_DLL

	#ifdef DLL_EXPORT_SIGNATURE
		#define METHOD_SIGNATURE extern "C" __declspec(dllexport)
		#define CLASS_PARAMS __declspec(dllexport)
	#else
		#define METHOD_SIGNATURE extern "C" __declspec(dllimport)
		#define CLASS_PARAMS __declspec(dllimport)
	#endif // DLL_EXPORT_SIGNATURE

	#define PROC_CALL __cdecl

#endif	//#ifndef CONFIG_CORE_DLL