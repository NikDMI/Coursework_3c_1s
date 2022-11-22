#define DLL_EXPORT_SIGNATURE

#include "EventHandler.h"
#include "../Bean/Exception.h"

namespace Nk {

	using namespace std;

	EventHandler::EventHandler() {

	}


	EventIndex EventHandler::RegisterEvent(std::string eventName) {
		if (m_registeredEvents.find(eventName) != m_registeredEvents.end()) {
			throw Exception{ eventName + " was registered earlyer" };
		}
		m_registeredEvents.insert({ eventName, m_currentFreeEventIndex });
		m_eventsInfo.push_back({});
		return m_currentFreeEventIndex++;
	}


	EventIndex EventHandler::GetEventIndex(std::string eventName) const {
		unordered_map<std::string, EventIndex>::const_iterator eventIter = m_registeredEvents.find(eventName);
		if (eventIter == m_registeredEvents.end()) {
			return -1;
		}
		return (*eventIter).second;
	}


	void EventHandler::AddEventHandler(EventIndex eventIndex, EventHandlerProcInfo eventHandler) {
		if (eventIndex >= m_currentFreeEventIndex) {
			throw Exception("Invalid event index");
		}
		m_eventsInfo[eventIndex].eventHandlerChain.push_back(eventHandler);
	}


	void EventHandler::RemoveEventHandler(EventIndex eventIndex, EventHandlerProc eventHandler) noexcept {
		if (eventIndex < m_currentFreeEventIndex) {
			try {
				m_eventsInfo[eventIndex].eventHandlerChain.remove({ eventHandler });
			}
			catch (...) {

			}
		}
	}


	void EventHandler::ExecuteEvent(EventIndex eventIndex, void* params) {
		if (eventIndex >= m_currentFreeEventIndex) {
			throw Exception("Invalid event index");
		}
		const EventInfo& eventInfo = m_eventsInfo[eventIndex];
		for (EventHandlerProcInfo eventCallbackInfo : eventInfo.eventHandlerChain) {
			eventCallbackInfo.eventCallback(params);
		}
	}


}