#ifndef ILAYOUT_GUI_DLL
#define ILAYOUT_GUI_DLL

#include "../Widget.h"
#include <list>

namespace Nk {
	
	/*
	* Itrerface of widgets layout
	*/
	class ILayout {
	public:
		/*
		* Add widget to the layout
		*/
		virtual void AddWidget(Widget* widget) = 0;
		virtual void AddWidgets(const std::list<Widget*>& widgets) = 0;

		virtual void RemoveWidget(Widget* widget) = 0;


		/*
		* Erase all controlled widgets of layout
		*/
		virtual void ClearLayout() = 0;

		/*
		* Count positions and size of internal widgets
		*/
		virtual void ComputeWidgetsPositions() = 0;


		virtual ~ILayout() {};
	};
}

#endif	//#ifndef ILAYOUT_GUI_DLL

