#ifndef DEFAULT_LAYOUT_GUI_DLL
#define DEFAULT_LAYOUT_GUI_DLL

#include "ILayout.h"
#include <list>

namespace Nk {

	/*
	* Itrerface of widgets layout
	*/
	class DefaultLayout: public ILayout {
	public:
		/*
		* Add widget to the layout
		*/
		void AddWidget(Widget* widget) override;
		void AddWidgets(const std::list<Widget*>& widgets) override;

		void RemoveWidget(Widget* widget) override;

		/*
		* Erase all controlled widgets of layout
		*/
		void ClearLayout() override;

		/*
		* Draw layout widgets
		*/
		void ComputeWidgetsPositions() override;


		~DefaultLayout() override;

	private:
		std::list<Widget*> m_layoutWidgets;
	};
}

#endif	//#ifndef DEFAULT_LAYOUT_GUI_DLL

