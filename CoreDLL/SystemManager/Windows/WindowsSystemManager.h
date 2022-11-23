#ifndef WINDOWS_SYSTEM_MANAGER_OS_DLL
#define WINDOWS_SYSTEM_MANAGER_OS_DLL

#include "../ISystemManager.h"

namespace Nk {


	/*
	* This interface is used to control system activities (mouse, keyboard)
	* Dispatches this activities throught events to correspondings controls
	*/
	class WindowsSystemManager: public ISystemManager {
	public:
		/*
		* Monitor system events and dispatches messages to the widghets
		*/
		void DispatchSystemMessage(bool canBlock) override;

		WindowsSystemManager();
		~WindowsSystemManager() override;
	};
}

#endif	//#ifndef WINDOWS_SYSTEM_MANAGER_OS_DLL

