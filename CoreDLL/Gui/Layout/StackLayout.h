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
		enum class StackSpace { NO_SPACE_BETWEEN, SEMI_SPACE_AROUND };


		CLASS_METHOD StackLayout(StackType stackType, StackAlignment stackAlignment = StackAlignment::NORMAL, StackSpace stackSpace = StackSpace::NO_SPACE_BETWEEN);

		/*
		* Draw layout widgets
		*/
		CLASS_METHOD void ComputeWidgetsPositions() override;


		CLASS_METHOD ~StackLayout() override;

	private:

		StackType m_stackType;
		StackAlignment m_stackAlignment;
		StackSpace m_stackSpace;
		float m_spaceBetweenWidgets = 0;
	};
}

#endif	//#ifndef STACK_LAYOUT_GUI_DLL

