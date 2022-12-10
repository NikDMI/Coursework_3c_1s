#ifndef WIDGET_GUI_DLL
#define WIDGET_GUI_DLL

#include "../Core/Object.h"
#include "../Bean/Config.h"
#include "../OsGui/IWindow.h"
#include "../Tools/Cursor/ICursor.h"
//#include "Layout/DefaultLayout.h"
#include "ResizeManager/IResizeManager.h"
#include <vector>
#include <list>

namespace Nk {

	class ILayout;
	class DefaultLayout;
	class BasicWidgetStructure;
	class IBorder;

	using WindowDrawProc = void (PROC_CALL*)(Widget* widget, IPainter* painter);

	/*
	* Main class of all graphic elements
	* React on all system events (mouse, keyboard, drag and drop...)
	*/
	CLASS_PARAMS class Widget: public Object {
	public:
		//Represents events, that user can override
		enum class CustomEvents : int { ON_MOUSE_MOVE, ON_MOUSE_LDOWN, ON_MOUSE_LUP, ON_PARENT_RESIZE, ON_MOUSE_ENTER, ON_MOUSE_LEAVE, ON_SET_FOCUS, ON_KILL_FOCUS,
			ON_CHAR, ON_KEY_DOWN, ON_KEY_UP, _LAST_ };

		CLASS_METHOD Widget(Widget* widget, Color_t backgroundColor);
		CLASS_METHOD Widget(Widget* widget = nullptr);
		virtual ~Widget();

		/*
		* Set size and position of the window
		*/
		CLASS_METHOD void SetWindowGeometry(Coord_t x, Coord_t y, Coord_t w, Coord_t h);

		CLASS_METHOD void OffsetWindow(Coord_t dx, Coord_t dy);

		CLASS_METHOD void ShowWindow();

		CLASS_METHOD void HideWindow();

		CLASS_METHOD void Repaint();

		CLASS_METHOD void SetBackgroundColor(Color_t bkColor);

		CLASS_METHOD void SetWindowDrawProc(WindowDrawProc drawProc);

		CLASS_METHOD IPainter* GetPainter();

		CLASS_METHOD Rect_t GetWidgetClientRect();

		CLASS_METHOD Rect_t GetWidgetRect();

		/*
		* Set cursor to the widget
		* Can get nullptr (cursor can't changed when user move to this control)
		*/
		CLASS_METHOD void SetCursor(ICursor* cursor);

		/*
		* returns rect, that can be used by header
		*/
		CLASS_METHOD Rect_t ComputeHeaderRect();

		void SetBorder(IBorder* border);

		/*
		* All mouse events go throught this control
		*/
		CLASS_METHOD void SetMouseCapture();

		CLASS_METHOD void ReleaseMouseCapture();

		CLASS_METHOD Widget* GetParentWidget();

		/*
		* Set widget as header(offset vieport at the height value)
		* virtual: some classes can restrict setting of header widgets and throw exception
		*/
		CLASS_METHOD void SetHeaderWidget(Widget* headerWidget);

		/*
		* Change flag to get parent notification (on resize, ...)
		*/
		CLASS_METHOD void SetParentNotification();


		/*
		* Sets resize manager to this widget
		* Can be nullptr
		*/
		CLASS_METHOD void SetResizeManager(IResizeManager* resizeManager);

		/*
		* Set focus to this window
		*/
		CLASS_METHOD void SetFocus();



		void SendRepaintEvent();

		/*
		* Set user's callback to specialized event 
		*/
		CLASS_METHOD void SetCustomEvent(CustomEvents eventType, EventHandlerProc callback);

	private:
		enum Events : int { ON_REPAINT, ON_DRAW, ON_MOUSE_MOVE, ON_MOUSE_LDOWN, ON_MOUSE_LUP, ON_MOUSE_ENTER, ON_MOUSE_LEAVE, ON_GET_FOCUS, ON_LEAVE_FOCUS,
			ON_CHAR, ON_KEY_DOWN, ON_KEY_UP, _LAST_ };

		void AddChildWidget(Widget* childWidget);
		void RemoveChildWidget(Widget* childWidget);
		void NotifyChilds(CustomEvents eventType, BasicWidgetStructure* params);	//Call child callbacks if needed

		/*
		* Computes coordinates without header widget and borders
		*/
		void ComputeCursorPointWithoutHelpWidgets(Point_t& inOutPoint);
		void ComputeOriginPointWithoutHelpWidgets(Point_t& inOutPoint);

		/*
		* Manage resizing states
		*/
		void CheckResizingOnMouseMove(Point_t cursorPos);

		static inline void CallUserCallback(Widget*, CustomEvents, void* params);

		/*
		* Basic options of drawing ()
		*/
		static void BeginBasicDrawProc(Widget* widget, IPainter* painter);
		static void EndBasicDrawProc(Widget* widget, IPainter* painter);
		static void CheckIsHeaderWidget(Widget* widget);
		static void ResumeIfHeaderWidget(Widget* widget);

		//Basic event functions
		static void PROC_CALL WidgetOnMouseMove(void* params);
		static void PROC_CALL WidgetOnLMouseDown(void* params);
		static void PROC_CALL WidgetOnLMouseUp(void* params);
		static void PROC_CALL WidgetOnMouseEnter(void* params);
		static void PROC_CALL WidgetOnMouseLeave(void* params);
		static void PROC_CALL WidgetOnSetFocus(void* params);
		static void PROC_CALL WidgetOnKillFocus(void* params);
		static void PROC_CALL WidgetOnChar(void* params);
		static void PROC_CALL WidgetOnKeyUp(void* params);
		static void PROC_CALL WidgetOnKeyDown(void* params);

		//Basic data
		Widget* m_parentWidget;
		std::list<Widget*> m_childWidgetList;
		IWindow* m_windowOs;	//phisical windows of OS
		WindowDrawProc m_userDrawProc;	//Draw proc, that can be replaces by user
		volatile bool m_isBackBufferActive = false;	//Show, does this widget has back buffer
		volatile bool m_isNeedTotalRedraw = false;	//Show, that all inner controls must be redrawed
		bool m_isNeedParentEventsMessages = false;	//Does parent should callback child, when parent's state was changed

		//Layout objects
		Widget* m_headerWidget;	//Caption box, tool bar, menu (this element can't be included in calculetion of coords)
		Point_t m_viewportPoint;
		IBorder* m_topBorder = nullptr;
		IBorder* m_bottomBorder = nullptr;
		IBorder* m_leftBorder = nullptr;
		IBorder* m_rightBorder = nullptr;

		//Helper objects
		DefaultLayout* m_defaultLayout = nullptr;
		ILayout* m_widgetLayout = nullptr;
		CRITICAL_SECTION m_drawLockObject;

		//Config objects
		ICursor* m_currentCursor = nullptr;	//Cursor that must be set when mouse move to the area
		ICursor* m_leaveCursor = nullptr;	//Cursor that should be set, when leave the area (last cursor)

		//Resizing
		IResizeManager* m_resizeManager = nullptr;
		IResizeManager::ResizeType m_lastResizeType = IResizeManager::ResizeType::NONE;
		bool m_isResizing = false;	//does widget resizing at the current time
		Point_t m_lastResizingPosition;
		ICursor* m_lastUserCursorAtResizingProcess;

		//Window data
		volatile Coord_t m_x = 0;	//Coords according to parent window
		volatile Coord_t m_y = 0;
		volatile Coord_t m_w = 0;
		volatile Coord_t m_h = 0;
		bool m_isVisible = false;
		Color_t m_backgroundColor;

		//Event handlers
		static void OnRepaintWindow(void* widget);
		static void OnDrawWindow(void* widget);


		static const char* EventsNames[Events::_LAST_];

		//Class id to register class in the global core register
		const static ClassId m_classId;

		//event indexes on basic gui events
		std::vector<EventIndex> m_correspondingEventIndexes;
		std::vector<EventHandlerProc> m_userEventCallbacks { (int)CustomEvents::_LAST_};

		//global data
		static Widget* m_captureWidget;

		/*
		* Returns corresponding EventIndex on some action
		*/
		CLASS_METHOD EventIndex GetEventIndex(Events eventType) const;

		friend LRESULT CALLBACK Win32WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		friend void ChangeMouseStructCoord(void* param);
	};
}

#endif	//#ifndef WIDGET_GUI_DLL