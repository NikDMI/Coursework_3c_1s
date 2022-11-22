#ifndef EVENT_HANDLER_CORE_DLL
#define EVENT_HANDLER_CORE_DLL

#include "../Bean/Config.h"
#include <string>
#include <numeric>
#include <unordered_map>
#include <list>

namespace Nk {

	using EventIndex = int32_t;
	using EventHandlerProc = void (PROC_CALL *)(void* params);

	struct EventHandlerProcInfo {
		EventHandlerProc eventCallback;
		bool isCoreCallback = false;	//If core functions process this event

		bool operator==(const EventHandlerProcInfo eventHandlerInfo) const {
			return (eventCallback == eventHandlerInfo.eventCallback);
		}
	};

	CLASS_PARAMS class EventHandler {
	public:
		/*
		* Register new event in the list
		*/
		EventIndex RegisterEvent(std::string eventName);

		/*
		* Get event index or -1 if error
		*/
		EventIndex GetEventIndex(std::string eventName) const;

		/*
		* Add new event handler to the handler queue
		*/
		void AddEventHandler(EventIndex eventIndex, EventHandlerProcInfo eventHandler);

		/*
		* Removes handler from queue
		*/
		void RemoveEventHandler(EventIndex eventIndex, EventHandlerProc eventHandler) noexcept;

		/*
		* Execute all registered callbacks
		*/
		void ExecuteEvent(EventIndex eventIndex, void* params);

		EventHandler();
		EventHandler(const EventHandler& copy) = delete;
		EventHandler& operator=(const EventHandler& copy) = delete;
		EventHandler(EventHandler&& move) = delete;
		EventHandler& operator=(EventHandler&& move) = delete;

	private:

		struct EventInfo {
			std::list<EventHandlerProcInfo> eventHandlerChain;
		};

		EventIndex m_currentFreeEventIndex = 0;
		std::unordered_map<std::string, EventIndex> m_registeredEvents;
		std::vector<EventInfo> m_eventsInfo;
	};
}

#endif	//#ifndef EVENT_HANDLER_CORE_DLL
