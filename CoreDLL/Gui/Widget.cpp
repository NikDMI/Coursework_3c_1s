#include "Widget.h"
#include "../Bean/Config.h"
#include "../Bean/Exception.h"

namespace Nk {
	using namespace std;

	const ClassId Widget::m_classId = Object::RegisterNewClass("Gui::Widget");


	Widget::Widget() {
		m_correspondingEventIndexes.reserve((int)Events::_LAST_);
		for (int i = 0; i < Events::_LAST_; ++i) {
			m_correspondingEventIndexes[i] = Object::m_eventHandler->RegisterEvent(EventsNames[i]);
		}
	}


	Widget::~Widget() {

	}

	EventIndex Widget::GetEventIndex(Events eventType) const {
		if (eventType < 0 || eventType >= Events::_LAST_) {
			throw Exception("Invalid eventType");
		}
	}
}