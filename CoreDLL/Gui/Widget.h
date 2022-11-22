#ifndef WIDGET_GUI_DLL
#define WIDGET_GUI_DLL

#include "../Core/Object.h"
#include "../Bean/Config.h"
#include <vector>

namespace Nk {
	/*
	* Main class of all graphic elements
	* React on all system events (mouse, keyboard, drag and drop...)
	*/
	CLASS_PARAMS class Widget: public Object {
	public:
		enum Events: int {ON_MOUSE_MOVE, ON_MOUSE_RDOWN, _LAST_};

		Widget();
		virtual ~Widget();

	protected:
		EventIndex GetEventIndex(Events eventType) const;
	private:
		const char* EventsNames[Events::_LAST_] = { "Core_OnMouseMove", "Core_OnMouseRDown"};

		//Class id to register class in the global core register
		const static ClassId m_classId;

		//event indexes on basic gui events
		std::vector<EventIndex> m_correspondingEventIndexes;
	};
}

#endif	//#ifndef WIDGET_GUI_DLL