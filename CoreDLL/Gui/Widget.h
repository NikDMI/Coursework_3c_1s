#ifndef WIDGET_GUI_DLL
#define WIDGET_GUI_DLL

#include "../Core/Object.h"
#include "../Bean/Config.h"
#include "../OsGui/IWindow.h"
#include <vector>
#include <list>

namespace Nk {

	class ILayout;

	using WindowDrawProc = void (PROC_CALL*)(Widget* widget, IPainter* painter);

	/*
	* Main class of all graphic elements
	* React on all system events (mouse, keyboard, drag and drop...)
	*/
	CLASS_PARAMS class Widget: public Object {
	public:
		enum Events: int {ON_REPAINT, ON_DRAW, ON_MOUSE_MOVE, ON_MOUSE_RDOWN, _LAST_};

		CLASS_METHOD Widget(Widget* widget, Color_t backgroundColor);
		CLASS_METHOD Widget(Widget* widget = nullptr);
		virtual ~Widget();

		/*
		* Set size and position of the window
		*/
		CLASS_METHOD void SetWindowGeometry(Coord_t x, Coord_t y, Coord_t w, Coord_t h);

		CLASS_METHOD void ShowWindow();
		CLASS_METHOD void HideWindow();


		CLASS_METHOD void SetWindowDrawProc(WindowDrawProc);

		/*
		* Returns corresponding EventIndex on some action
		*/
		CLASS_METHOD EventIndex GetEventIndex(Events eventType) const;

	protected:
		static void BasicDrawProc(Widget* widget, IPainter* painter);

	private:
		void AddChildWidget(Widget* childWidget);
		void RemoveChildWidget(Widget* childWidget);

		//Basic data
		Widget* m_parentWidget;
		std::list<Widget*> m_childWidgetList;
		IWindow* m_windowOs;	//phisical windows of OS
		WindowDrawProc m_userDrawProc;	//Draw proc, that can be replaces by user

		//Helper objects
		ILayout* m_widgetLayout = nullptr;

		//Window data
		Coord_t m_x = 0;	//Coords according to parent window
		Coord_t m_y = 0;
		Coord_t m_w = 0;
		Coord_t m_h = 0;
		bool m_isVisible = false;
		Color_t m_backgroundColor;

		//Event handlers
		void OnRepaintWindow(void* params);
		void OnDrawWindow(void* params);


		static const char* EventsNames[Events::_LAST_];

		//Class id to register class in the global core register
		const static ClassId m_classId;

		//event indexes on basic gui events
		std::vector<EventIndex> m_correspondingEventIndexes;
	};
}

#endif	//#ifndef WIDGET_GUI_DLL