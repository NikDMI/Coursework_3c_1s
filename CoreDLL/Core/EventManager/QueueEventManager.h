#ifndef QUEUE_EVENT_MANAGER_FACTORY_CORE_DLL
#define QUEUE_EVENT_MANAGER_FACTORY_CORE_DLL

#include "IEventManager.h"
#include <list>

namespace Nk {


	class QueueEventManager final: public IEventManager {
	public:
		QueueEventManager();

		~QueueEventManager() override;

		void PushEvent(Object* reciever, EventIndex eventIndex, void* params) override;

		bool DispatchEvent() override;

	private:
		struct EventInfo {
			Object* reciever;
			EventIndex eventIndex;
			void* params;
		};
		std::list<EventInfo> m_eventQueue;
	};
}

#endif	//#ifndef QUEUE_EVENT_MANAGER_FACTORY_CORE_DLL
