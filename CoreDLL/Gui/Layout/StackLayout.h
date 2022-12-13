#ifndef STACK_LAYOUT_GUI_DLL
#define STACK_LAYOUT_GUI_DLL

#include "ILayout.h"

namespace Nk {

	/*
	* Itrerface of widgets layout
	*/
	CLASS_PARAMS class StackLayout : public ILayout {
	public:
		enum class StackType {VERTICAL, HORIZONTAL};
		enum class StackAlignment { CENTER, NORMAL, REVERSE, ALL_SPACE };


		CLASS_METHOD StackLayout(StackType stackType, StackAlignment stackAlignment = StackAlignment::NORMAL);

		/*
		* Draw layout widgets
		*/
		void ComputeWidgetsPositions() override;


		CLASS_METHOD ~StackLayout() override;

	private:

		StackType m_stackType;
		StackAlignment m_stackAlignment;
		float m_spaceBetweenWidgets = 0;
	};
}

#endif	//#ifndef STACK_LAYOUT_GUI_DLL

