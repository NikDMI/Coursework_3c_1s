#ifndef SYSTEM_MANAGER_OS_DLL
#define SYSTEM_MANAGER_OS_DLL

namespace Nk {

	/*
	* This interface is used to control system activities (mouse, keyboard)
	* Dispatches this activities throught events to correspondings controls
	*/
	class ISystemManager {
	public:
		/*
		* Monitor system events and dispatches messages to the widghets
		*/
		virtual void DispatchSystemMessage() = 0;

		virtual ~ISystemManager() {};
	};
}

#endif	//#ifndef SYSTEM_MANAGER_OS_DLL

