#ifndef EVENT_MANAGER_FACTORY_CORE_DLL
#define EVENT_MANAGER_FACTORY_CORE_DLL

#include "IEventManager.h"

namespace Nk {

	
	class EventManagerFactory {
	public:
		enum class ManagerType {QUEUE};

		static IEventManager* GetEventManager(ManagerType managerType);

		EventManagerFactory() = delete;
	};
}

#endif	//#ifndef EVENT_MANAGER_FACTORY_CORE_DLL
