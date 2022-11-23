#include "QueueEventManager.h"

namespace Nk {

	QueueEventManager::QueueEventManager() {

	}


	QueueEventManager::~QueueEventManager() {

	}


	void QueueEventManager::PushEvent(Object* reciever, EventIndex eventIndex, void* params) {
		m_eventQueue.push_back({ reciever, eventIndex, params });
	}


	bool QueueEventManager::DispatchEvent() {
		size_t eventCount = m_eventQueue.size();
		if (eventCount) {
			EventInfo eventInfo = m_eventQueue.front();
			m_eventQueue.pop_front();
			eventInfo.reciever->m_eventHandler->ExecuteEvent(eventInfo.eventIndex, eventInfo.params);
			return eventCount - 1;
		}
		else {
			return false;
		}
	}
}