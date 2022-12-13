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
		* Draw layout widgets
		*/
		CLASS_METHOD void ComputeWidgetsPositions() override;


		~DefaultLayout() override;

	private:
	};
}

#endif	//#ifndef DEFAULT_LAYOUT_GUI_DLL

