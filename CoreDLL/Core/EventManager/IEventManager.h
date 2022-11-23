#ifndef IEVENT_MANAGER_CORE_DLL
#define IEVENT_MANAGER_CORE_DLL

#include "../EventHandler.h"
#include "../Object.h"

namespace Nk {

	/*
	* This interface is used to manage gui message queue
	*/
	class IEventManager {
	public:
		virtual ~IEventManager() {};
		/*
		* Add event to the queue
		*/
		virtual void PushEvent(Object* reciever, EventIndex eventIndex, void* params) = 0;

		/*
		* Pop event from the queue
		* @return: true - if queue is not empry; false - queue is empty
		*/
		virtual bool DispatchEvent() = 0;
	};
}

#endif	//#ifndef EVENT_MANAGER_CORE_DLL
