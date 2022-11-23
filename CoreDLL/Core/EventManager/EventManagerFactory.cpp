#include "EventManagerFactory.h"
#include "QueueEventManager.h"

namespace Nk {

	IEventManager* EventManagerFactory::GetEventManager(ManagerType managerType) {
		switch (managerType) {
		case ManagerType::QUEUE:
			return new QueueEventManager{};
		}
		return nullptr;
	}
}