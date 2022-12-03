#include "QueueEventManager.h"

namespace Nk {

	QueueEventManager::QueueEventManager() {
		InitializeCriticalSectionAndSpinCount(&m_lockSection, 2000);
	}


	QueueEventManager::~QueueEventManager() {
		DeleteCriticalSection(&m_lockSection);
	}


	void QueueEventManager::PushEvent(Object* reciever, EventIndex eventIndex, void* params) {
		EnterCriticalSection(&m_lockSection);
		m_eventQueue.push_back({ reciever, eventIndex, params });
		LeaveCriticalSection(&m_lockSection);
	}


	bool QueueEventManager::DispatchEvent() {
		EnterCriticalSection(&m_lockSection);
		size_t eventCount = m_eventQueue.size();
		if (eventCount) {
			EventInfo eventInfo = m_eventQueue.front();
			m_eventQueue.pop_front();
			eventInfo.reciever->GetEventHandler()->ExecuteEvent(eventInfo.eventIndex, eventInfo.params);
			LeaveCriticalSection(&m_lockSection);
			return eventCount - 1;
		}
		else {
			LeaveCriticalSection(&m_lockSection);
			return false;
		}
	}
}