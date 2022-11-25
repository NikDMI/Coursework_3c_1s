#include "DefaultLayout.h"
#include "../../Bean/Exception.h"

namespace Nk {

	void DefaultLayout::AddWidget(Widget* widget) {
		if (widget == nullptr) {
			throw Exception{"Try to add nullptr widget to layout"};
		}
		m_layoutWidgets.push_back(widget);
	}


	void DefaultLayout::AddWidgets(const std::list<Widget*>& widgets) {
		for (Widget* widget : widgets) {
			if (widget == nullptr) throw Exception{ "Try to add nullptr widget to layout" };
			m_layoutWidgets.push_back(widget);
		}
	}


	void DefaultLayout::ClearLayout() {
		m_layoutWidgets.clear();
	}


	void DefaultLayout::DrawLayout() {

	}


	DefaultLayout::~DefaultLayout() {

	}

}