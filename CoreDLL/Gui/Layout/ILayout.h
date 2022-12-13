#ifndef ILAYOUT_GUI_DLL
#define ILAYOUT_GUI_DLL

#include "../Widget.h"
#include <list>

namespace Nk {
	
	/*
	* Itrerface of widgets layout
	*/
	CLASS_PARAMS class ILayout {
	public:
		/*
		* Add widget to the layout
		*/
		CLASS_METHOD void AddWidget(Widget* widget);

		CLASS_METHOD void AddWidgets(const std::list<Widget*>& widgets);

		CLASS_METHOD void RemoveWidget(Widget* widget);


		/*
		* Erase all controlled widgets of layout
		*/
		CLASS_METHOD void ClearLayout();


		/*
		* Count positions and size of internal widgets
		*/
		virtual void ComputeWidgetsPositions() = 0;


		virtual ~ILayout() {};

	protected:
		ILayout();

		std::list<Widget*> m_layoutWidgets;
	};
}

#endif	//#ifndef ILAYOUT_GUI_DLL

