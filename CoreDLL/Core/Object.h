#ifndef CORE_OBJECT_H_DLL
#define CORE_OBJECT_H_DLL

#include <unordered_map>
#include <string>
#include <numeric>
#include "../Bean/Config.h"
#include "EventHandler.h"

namespace Nk {

	using ClassId = int32_t;
	using FabricMethod = void* (*)();

	/*
	* Main class of all classes in this library
	* Provides event handling
	* Meta-data
	*/
	CLASS_PARAMS class Object {
	public:
		/*
		* This function register new class in the class registy (to get access in a C-style programming)
		*/
		static ClassId RegisterNewClass(std::string className);
		static ClassId RegisterNewClass(std::string className, FabricMethod fabricMethod);	//For classes outside core

		Object();
		virtual ~Object();

		//EventHandler
		EventHandler* m_eventHandler;
	private:
		//Registered classes in Object hierarcy
		static std::unordered_map<std::string, ClassId> m_registeredClasses;
		static ClassId m_currentFreeClassId;
	};

	/*
	**** When we create new class in global class hierarcy, we must register it in the class registry
	*/
}

#endif	//#ifndef CORE_OBJECT_H_DLL
